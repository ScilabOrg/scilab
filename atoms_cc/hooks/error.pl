#!/usr/bin/perl -w

use strict;
use File::Basename;

my $hook =  basename($0);
   $hook =~ s/\.pl$//;

exec "perl", "-w", dirname($0)."/hook", $hook, @ARGV;
