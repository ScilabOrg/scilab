#!/usr/bin/perl -w

# buildtoolbox.pl
# Usage: buildtoolbox.pl toolbox-archive [config file [stage]]

use strict;
use Config::IniFiles;

use AtomsCC::Common;
use AtomsCC::Utils;
use AtomsCC::Stages;

my ($TOOLBOXFILE, # Toolbox archive to compile
    $TOOLBOXNAME, # Name of the toolbox
    $STAGE, # Current stage
    $CONFIG); # Configuration (see Config::IniFiles)

# Init global vars, check arguments
$::STAGE = "pre";

$::TOOLBOXFILE = shift;
if(!defined($::TOOLBOXFILE)) {
	common_die "Toolbox source file required";
}

if(! -r $::TOOLBOXFILE) {
	common_die "$TOOLBOXFILE doesn't exists or can't be read";
}

$::TOOLBOXNAME = $1 if($::TOOLBOXFILE =~ m#([^/]+)$#); # toolboxname = basename(toolboxfile)
$::TOOLBOXNAME = $1 if($::TOOLBOXNAME =~ m#^([^.]+)#);

$::CONFIG = Config::IniFiles->new(-file => shift);

common_log "Toolbox: $::TOOLBOXNAME";
common_log "Source file: $::TOOLBOXFILE";

common_hook("start");
stage_check;
stage_unpack;
#~ stage_makeenv;
stage_tbdeps;
#~ stage_sysdeps;
stage_build;
stage_pack;
#~ stage_cleanenv;
#~ stage_test_makeenv;
#~ stage_test_tbdeps;
#~ stage_test_sysdeps;
#~ stage_test_setuptb;
#~ stage_test_runtests;
#~ stage_test_cleanenv;

$::STAGE = "post";
common_hook("end");
