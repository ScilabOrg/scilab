#!/usr/bin/perl -w

# clean.pl
# Usage: clean.pl config-file
# Delete Compilation, Stage, CommandReport, ReportMessage records associated to this Host

use strict;
use Config::IniFiles;
use DBI;

my %SQL;
$SQL{'FindCompilation'} = <<EOQ
SELECT IdCompilation FROM ToolboxCompilation WHERE Host = ?
EOQ
;
$SQL{'FindStage'} = <<EOQ
SELECT IdStage FROM Stage WHERE IdCompilation = ?
EOQ
;
$SQL{'DeleteCommandReport'} = <<EOQ
DELETE FROM CommandReport WHERE IdStage = ?
EOQ
;
$SQL{'DeleteReportMessage'} = <<EOQ
DELETE FROM ReportMessage WHERE IdStage = ?
EOQ
;
$SQL{'DeleteStage'} = <<EOQ
DELETE FROM Stage WHERE IdStage = ?
EOQ
;
$SQL{'DeleteCompilation'} = <<EOQ
DELETE FROM ToolboxCompilation WHERE IdCompilation = ?
EOQ
;

my $config = Config::IniFiles->new(-file => $ARGV[0]);
my $dbh = DBI->connect($config->val('sql','datasource'),
                       $config->val('sql','user'),
                       $config->val('sql','password'),
                       { RaiseError => 1 });
my $sthC = $dbh->prepare($SQL{'FindCompilation'});
   $sthC->execute($config->val('general', 'host'));
while(my @comp = $sthC->fetchrow_array) {
	my $sthS = $dbh->prepare($SQL{'FindStage'});
	   $sthS->execute($comp[0]);
	while(my @stage = $sthS->fetchrow_array) {
		$dbh->do($SQL{'DeleteCommandReport'}, undef, $stage[0]);
		$dbh->do($SQL{'DeleteReportMessage'}, undef, $stage[0]);
		$dbh->do($SQL{'DeleteStage'}, undef, $stage[0]);
	}
	$dbh->do($SQL{'DeleteCompilation'}, undef, $comp[0]);
}
