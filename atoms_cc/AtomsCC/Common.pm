package AtomsCC::Common;

use strict;
use Exporter;
use Cwd;
use Time::HiRes;

our @ISA = ('Exporter');
our @EXPORT = qw(&common_log &common_enter_stage &common_leave_stage &common_die
                 &common_exec &common_exec_scilab &common_hook);

BEGIN {
	open OLD_STDOUT, ">&STDOUT";
	open OLD_STDERR, ">&STDERR";
	open OLD_STDIN, "<&STDIN";
	open LOGFILE, ">build.log";
}

END {
	# $? is our return code in a END bloc (see $ in perlvar man page). But
	# wait() modify it, so we have to backup it
	my $exit_code = $?;
	
	close OLD_STDERR;
	close OLD_STDOUT;
	close OLD_STDIN;
	close LOGFILE;
	1 while(wait() != -1);
	
	$? = $exit_code;
}

# common_log(message, type):
#    Print a log message. Second argument is the type of the
#    message:
#     " " for a normal message
#     "!" for an error
#     ">" when starting a stage
#     "<" when terminating a stage
#     "$" when running a command
#     "?" for the return code of previous command
sub common_log {
	my $message = shift;
	my $type = shift || " ";
	my ($sec, $msec) = Time::HiRes::gettimeofday();
	
	# Check message format: any newline must start by a space,
	# no new line at end of message
	$message =~ s/(?<=\n)(?!\s|$)/ /g;
	chomp $message;
	
	print LOGFILE "[$sec.$msec]${type}${message}\n";
	print "[$type] $message \n";
}

# common_enter_stage(stage):
#    Common stuff while starting a new stage
sub common_enter_stage {
	$::STAGE = shift;
	common_log($::STAGE, ">");
	common_hook("startstage");
}

# common_leave_stage:
#    Common stuff while ending a stage
sub common_leave_stage {
	common_hook("endstage");
	common_log($::STAGE, "<");
}

# common_die(message):
#    Called when a problem happens.
sub common_die {
	my $message = shift;
	
	common_log($message, "!");
	common_hook("error");
	
	exit(1);
}

# common_exec(command, args..., [opts]):
#    Execute given command, places its outputs to log files. If last argument
#    is a reference to a hash, it's considered as options for the function.
#    Available options:
#     - "stderr_to_stdout": same as 2>&1 in shell.
#     - "working_dir": set the working directory of the suprocess
#    Returns a file handle on STDOUT.
#    Die if return code is non-zero or if standard error is non-empty.
sub common_exec {
	# pretty_arg:
	#     Human-readable form of the arguments array
	sub pretty_arg {
		my $_ = shift;
		if(/\s|["']/) {
			s/"/\\"/g;
			s/^/"/;
			s/$/"/;
		}
		return $_;
	}
	
	my $refopts = pop if ref($_[-1]) eq "HASH";
	my %opts;
	   %opts = %$refopts if defined($refopts);
	
	my $cmd = join(" ", map { pretty_arg $_ } @_);
	my $commandnum = 1;
	
	# Find commandnum: log files are (stage)-1.out for first
	# command of (stage), (stage)-2.out for second command of stage,
	# and so on
	$commandnum++ while(-e "$::STAGE-$commandnum.out");
	
	my $stdout = "$::STAGE-$commandnum.out";
	my $stderr = "$::STAGE-$commandnum.err";
	
	common_log("$cmd\nstdout=$stdout\nstderr=$stderr", "\$");
	
	# Setup I/O for subprocess
	open STDOUT, ">$stdout";
	open STDERR, ">$stderr";
	
	if(defined($opts{"stderr_to_stdout"})) {
		close STDERR;
		open STDERR, ">&STDOUT";
	}
	
	close STDIN;
	
	# Change working directory if needed
	my $pwd = cwd();
	chdir $opts{"working_dir"} if defined($opts{"working_dir"});
	
	# Exec suprocess
	system { $_[0] } @_;
	
	# Restore working directory
	chdir $pwd;
	
	# Restore I/O
	open STDIN, "<&OLD_STDIN";
	open STDOUT, ">&OLD_STDOUT";
	open STDERR, ">&OLD_STDERR";

	common_log("$?", "?");
	common_die("\"$cmd\" failed (non-zero exit code)") if($? != 0);
	common_die("\"$cmd\" failed (non-empty error output)") if(-s $stderr);
	
	open my ($fd), $stdout;
	return $fd;
}

# common_exec_scilab(script):
#     Execute scilab script
sub common_exec_scilab {
	my $script = shift;
	$script = "try; $script; catch; write(%io(2), lasterror()); end; quit;";
	
	my $scilab = "scilex" if($^O =~ /mswin/i);
	   $scilab = "scilab" unless(defined($scilab));
	
	return common_exec($scilab, "-nwni", "-nb", "-e", $script);
}

# common_hook(hook):
#     Call a hook if defined in configuration file
sub common_hook {
	my $hook = shift;
	my $cmd = $::CONFIG->val("compilation", "hook-". $hook);
	
	# Since we are using common_exec, which can call common_die, which can call
	# common_hook("error"), we may encounter an infinite loop if the "error" hook
	# fails. To avoid this, a re-entrance into common_hook("error") is logged as
	# an error but without effectively calling the error hook.
	# Detection of such a re-entrance is done by a dynamically-scoped variable,
	# see "temporary values via local()" section of perlsub(1perl)
	our $in_error_hook;
	if($hook eq "error" && defined($in_error_hook)) {
		common_log("Error while executing error hook", "!");
		return;
	}
	local $in_error_hook = 1 if $hook eq "error";
	
	# We can run the hook without fearing an infinite loop
	if(defined($cmd) && $cmd) {
		my $fd;
		
		close common_exec(split(" ", $cmd),
		               "--stage=$::STAGE",
		               "--source=$::TOOLBOXFILE",
		               "--toolbox=$::TOOLBOXNAME",
		               "--config=".$::CONFIG->GetFileName());
	}
}

1;
