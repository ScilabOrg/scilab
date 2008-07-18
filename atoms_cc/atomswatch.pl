#!/usr/bin/perl -w

# atomswatch.pl
# Usage: atomswatch.pl config-file

use strict;
use Config::IniFiles;
use DBI;
use Net::FTP;
use File::Path; # for rmtree()
use Time::Local; # for timelocal()
use sigtrap qw(die normal-signals error-signals);

my ($config, # Configuration
    $tmpdir, # Where to compile toolboxes
    $statef, # File where is saved last time this script was called
	$dbh,    # Connection to the database
	$ftp,    # Connection to the FTP server
	%SQL);   # All SQL queries

# SQL queries
$SQL{'CreateCompilation'} = <<EOQ
INSERT INTO ToolboxCompilation
       (IdSource, Host, Target, Environ, StartTime, Stage)
VALUES (       ?,    ?,      ?,       ?,     NOW(),     ?)
EOQ
;
$SQL{'UpdateCompilation'} = <<EOQ
UPDATE ToolboxCompilation SET Stage = ?, EndTime = ? WHERE IdCompilation = ?
EOQ
;
$SQL{'CreateStage'} = <<EOQ
INSERT INTO Stage
       (IdCompilation, StageName, StartTime)
VALUES (            ?,         ?,         ?)
EOQ
;
$SQL{'UpdateStage'} = <<EOQ
UPDATE Stage SET Success = ?, EndTime = ? WHERE IdStage = ?
EOQ
;
$SQL{'CreateReportMessage'} = <<EOQ
INSERT INTO ReportMessage
       (IdStage, Time, Type, Message)
VALUES (      ?,    ?,    ?,       ?);
EOQ
;
$SQL{'CreateCommandReport'} = <<EOQ
INSERT INTO CommandReport
       (IdStage, StartTime, EndTime, CommandLine, ReturnCode, stdout, stderr)
VALUES (      ?,         ?,       ?,           ?,          ?,      ?,      ?)
EOQ
;
$SQL{'FindRecentToolboxes'} = <<EOQ
SELECT IdSource, SourceFile, UploadTime FROM ToolboxSource WHERE UploadTime > ?
EOQ
;

# timestamp_to_sql(ts):
#     Returns a datetime in the YYYY-MM-DD HH:MM:SS format
sub timestamp_to_sql {
	my $ts = shift;
	my @ti = localtime($ts);
	
	return sprintf("%04d-%02d-%02d %02d:%02d:%02d", $ti[5]+1900, $ti[4],
	               $ti[3], $ti[2], $ti[1], $ti[0]);
}

# sql_to_timestamp(sql):
#     Returns a timestamp from a datetime in the YYYY-MM-DD HH:MM:SS format
sub sql_to_timestamp {
	my $sql = shift;
	return timelocal(reverse split(/\D/, $sql));
}

# readf(filename):
#     Read "filename" and returns its content as a string
sub readf {
	my $file = shift;
	open my($fd), $file;
	local $/;
	my $res = <$fd>;
	close $fd;
	return $res;
}

# open_ftp:
#     Connect to the FTP server
sub open_ftp {
	$ftp = Net::FTP->new($config->val('ftp', 'host'))
	          or die("Can't connect to FTP server: $@");
	$ftp->login($config->val('ftp', 'user'), $config->val('ftp', 'password'))
              or die("Cannot login to FTP server: ".$ftp->message);
}

# create_compilation(source_id, host, target, environ)
#     Create a new record in the ToolboxCompilation table and return its
#     IdCompilation field.
sub create_compilation {
	my $source_id = shift;
	my $host = shift;
	my $target = shift;
	my $environ = shift;
	
	$dbh->do($SQL{'CreateCompilation'}, undef, $source_id, $host,  $target,
	         $environ, "pre");
	
	return $dbh->last_insert_id(undef, undef, "ToolboxCompilation",
	                                          "IdCompilation");
}

# update_compilation(comp_id, stage, endtime)
#     Modify Stage and EndTime fields of a ToolboxCompilation record identified
#     by comp_id.
sub update_compilation {
	my $comp_id = shift;
	my $stage = shift;
	my $endtime = shift;
	
	$dbh->do($SQL{'UpdateCompilation'}, undef, $stage,
	         timestamp_to_sql($endtime), $comp_id);
}

# create_stage(compilation_id, stage_name, start_time):
#     Create a new record in the Stage table and return its IdStage field.
sub create_stage {
	my $comp_id = shift;
	my $stage = shift;
	my $start = shift;
	
	$dbh->do($SQL{'CreateStage'}, undef, $comp_id, $stage,
	         timestamp_to_sql($start));
	
	return $dbh->last_insert_id(undef, undef, "Stage", "IdStage");
}

# update_stage(stage_id, end_time, success)
#     Modify Success and EndTime fields of a Stage record identified by
#     stage_id.
sub update_stage {
	my $stage_id = shift;
	my $end = shift;
	my $success = shift;
	
	$dbh->do($SQL{'UpdateStage'}, undef, $success, timestamp_to_sql($end),
	         $stage_id);
}

# create_report_message(stage_id, time, type, msg)
#     Create a new ReportMessage record. Returns nothing.
sub create_report_message {
	my $stage_id = shift;
	my $time = shift;
	my $type = shift;
	my $msg = shift;
	
	$dbh->do($SQL{'CreateReportMessage'}, undef, $stage_id,
	         timestamp_to_sql($time), $type, $msg);
}

# create_command_report(stage_id, start_time, end_time, command, exit_code,
#                       stdout, stderr)
#     Create a new CommandReport record. Returns nothing.
sub create_command_report {
	my $stage_id = shift;
	my $start_time = shift;
	my $end_time = shift;
	my $command = shift;
	my $exit_code = shift;
	my $stdout = shift;
	my $stderr = shift;
	
	# Create the record
	$dbh->do($SQL{'CreateCommandReport'}, undef,
	         $stage_id, timestamp_to_sql($start_time),
	         timestamp_to_sql($end_time), $command, $exit_code,
	         $stdout, $stderr);
}

# Load config, init global variables, connect to DB, and so on...
$config = Config::IniFiles->new(-file => $ARGV[0]);
$tmpdir = $config->val("general", "tmpdir");
$statef = $config->val("general", "statefile");
$dbh    = DBI->connect($config->val('sql','datasource'),
                       $config->val('sql','user'),
                       $config->val('sql','password'),
                       { RaiseError => 1 });

open_ftp;

my $environ  = "Perl $^V running on $^O\n";
   $environ .= "OS: ".`uname -a` unless $^O =~ /win/i;
   $environ .= "Environment variables:\n";
   $environ .= "$_ = $ENV{$_}\n" foreach (keys %ENV);

# Check last time we visited the sources base
open my ($fd), $statef;
my $last_visited = <$fd>;
close $fd;
chomp $last_visited;

# Search for new toolboxes
my $sth = $dbh->prepare($SQL{'FindRecentToolboxes'});
$sth->execute(timestamp_to_sql($last_visited));

my %subprocesses;
my $next_last_visited = $last_visited;
while(my @recent = $sth->fetchrow_array) {
	# Are we already compiling this toolbox ? If so, skip it
	# TODO: check that the process is still alive (and not in an infinite loop)
	my $toolbox = $1 if $recent[1] =~ /^(.+)\.(tar\.gz|zip)$/;
	next if -d "$tmpdir/$toolbox";
	
	# Create a compilation record in the database
	my $comp_id;
	$comp_id = create_compilation($recent[0], $config->val('general', 'host'),
	                              $config->val('general', 'target'), $environ);
	
	# Fetch sources
	mkdir "$tmpdir/$toolbox" or die("Can't create $tmpdir/$toolbox");
	$ftp->get($config->val('ftp','sourcesdir')."/$recent[1]",
	          "$tmpdir/$toolbox/$recent[1]")
	      or die("Can't fetch source: " . $ftp->message);
	
	# Run the compilation in a subprocess
	my $pid = fork();
	if($pid == 0) {
		chdir "$tmpdir/$toolbox" or die("Can't chdir()");
		exec("buildtoolbox", $recent[1], $ARGV[0]) or
		die("Can't run the compilation process");
	}
	$subprocesses{$pid} = [$recent[0], $recent[1], $toolbox, $comp_id, time()];
	
	my $upltime = sql_to_timestamp($recent[2]);
	$next_last_visited = $upltime if $upltime > $next_last_visited;
}

# Now the toolboxes are compiling. This may take some take, disconnect from FTP
# before the timeout occurs
$ftp->quit;

while((my $pid = wait()) > 0) {
	my $success = ($? == 0);
	my ($tbid, $tbsrcfile, $toolbox, $comp_id, $st) = @{$subprocesses{$pid}};
	my $tbdir = "$tmpdir/$toolbox/";
	
	# Read build.log
	my $buildlog = readf("$tmpdir/$toolbox/build.log");
	
	# Create a "global" stage for messages which doesn't belong to any stage
	my $global_stage_id = create_stage($comp_id, "global", $st);
	my $stage_id = $global_stage_id;
	
	# Parse build.log, fill Stage, ReportMessage and CommandReport tables
	my @last_cmd_message;
	my $stage_success = 1;
	my $last_time = 0;
	foreach (split(/\n+(?!\s)/, $buildlog)) {
		s/(?<=\n)\s//g; # Delete LWS
		/^\[(\d+)\](.)(.+)$/s or die("Invalid build.log format");
		
		$last_time = $1;
		
		if($2 eq "\$") {
			@last_cmd_message = ($1, $2, $3);
		}
		elsif($2 eq "?") {
			my @cmd = split(/\n/, $last_cmd_message[2]);
			my @stdout = grep { /^stdout=/ } @cmd;
			my @stderr = grep { /^stderr=/ } @cmd;
			my $stdout_data = readf("$tbdir/".substr($stdout[0], 7));
			my $stderr_data = readf("$tbdir/".substr($stderr[0], 7));
			
			create_command_report($stage_id, $last_cmd_message[0], $1, $cmd[0],
			                      $3, $stdout_data, $stderr_data);
		}
		else {
			update_stage($stage_id, $1, $stage_success) if $2 eq "<";
			
			$stage_id = create_stage($comp_id, $3, $1) if $2 eq ">";
			create_report_message($stage_id, $1, $2, $3);
			$stage_id = $global_stage_id if $2 eq "<";
			
			$stage_success = 0 if $2 eq "!";
			$stage_success = 1 if $2 eq "<";
			$success = 0 if $2 eq "!";
		}
	}
	
	update_stage($global_stage_id, $last_time, $success);
	
	# Update ToolboxCompilation table
	my $status = "done-ok" if $success;
	   $status = "done-failed" if ! $success;
	
	update_compilation($comp_id, $status, $last_time);
	
	undef $subprocesses{$pid};
	
	# Upload resulting files 
	if($success) {
		open_ftp;
		
		my $tbprefix =  $tbsrcfile;
		   $tbprefix =~ s/\.(tar\.gz|zip)$//;
		my $bindir = $config->val('ftp', 'bindir');
		
		$ftp->put("$tbdir/$tbprefix-bin.zip", "$bindir/$tbprefix.zip")
		   or die("Can't send $tbprefix-bin.zip");
		$ftp->put("$tbdir/$tbprefix-bin.tar.gz", "$bindir/$tbprefix.tar.gz")
		   or die("Can't send $tbprefix-bin.tar.gz");
		
		$ftp->quit;
	}
	
	# Clean everything
	rmtree($tbdir);
	die("Can't delete $tbdir") if -d $tbdir;
}

# Update state file
open my($state_fd), ">$statef";
print $state_fd $next_last_visited;
close $state_fd;

END {
	if($? != 0) {
		if(%subprocesses) {
			kill(2, $_) foreach (keys %subprocesses);
			sleep(5);
			kill(9, $_) foreach (keys %subprocesses);
			1 while(wait() > 0);
		}
		
		# TODO: send a mail to the admin
	}
	
	$dbh->disconnect;
	$ftp->quit;
}
