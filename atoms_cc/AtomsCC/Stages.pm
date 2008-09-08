package AtomsCC::Stages;

use strict;
use Exporter;
use Cwd;

use AtomsCC::Utils;
use AtomsCC::Common;

our @ISA = ('Exporter');
our @EXPORT = qw(&stage_check &stage_unpack &stage_makeenv &stage_tbdeps
                 &stage_sysdeps &stage_build &stage_pack &stage_cleanenv
				 &stage_test_makeenv &stage_test_tbdeps &stage_test_sysdeps
				 &stage_test_setuptb &stage_test_runtests &stage_test_cleanenv);

# stage_check:
#   Perform basic checks
sub stage_check {
	common_enter_stage("check");
	
	if(is_zip()) {
		common_log("Detected ZIP format");
	}
	else {
		common_log("Detected TAR+GZIP format");
	}
	
	# Check tree
	common_log("Checking archive structure");
	my %tree = get_tree();
	common_log("Archive files:\n" . join("\n", sort keys %tree));
	check_tree(%tree);
	
	# Check DESCRIPTION
	common_log("Checking DESCRIPTION");
	my $fd = read_file_from_archive("DESCRIPTION");
	my %desc = read_description($fd);
	common_log("Computed DESCRIPTION:\n" .
		join("\n", map { "$_: $desc{$_}" } sort keys %desc));
	
	# Check toolbox name
	common_die("Invalid toolbox name") unless
		$::TOOLBOXNAME =~ /^[a-zA-Z0-9_-]+$/;
	
	if($::TOOLBOXNAME ne $desc{"Toolbox"}) {
		common_die "Detected toolbox name ($::TOOLBOXNAME) different from ".
		    "DESCRIPTION version ($desc{Toolbox})";
	}
	
	# Check version
	my $version = $1 if ($::TOOLBOXFILE =~ /[^.]+\.([^-]+)\.(tar\.gz|zip)/);
	if(!defined($version)) {
		common_die "Can't detect version from archive name ($::TOOLBOXFILE)";
	}
	
	if($version ne $desc{"Version"}) {
		common_die "Detected version ($version) different from DESCRIPTION ".
		    "version ($desc{Version})";
	}
	
	# Check DESCRIPTION-FUNCTIONS
	common_log("Checking DESCRIPTION-FUNCTIONS");
	$fd = read_file_from_archive("DESCRIPTION-FUNCTIONS");
	my %funcs = read_description_functions($fd);
	common_log("Computed DESCRIPTION-FUNCTIONS:\n" .
		join("\n", map { "$_: $funcs{$_}" } sort keys %funcs));
	
	common_leave_stage();
}

# stage_unpack:
#     Extract the archive
sub stage_unpack {
	common_enter_stage("unpack");
	
	if(is_zip()) {
		close common_exec("unzip", "-o", $::TOOLBOXFILE);
	}
	else {
		close common_exec("tar", "-xzvf", $::TOOLBOXFILE,
			{'stderr_to_stdout' => 1});
	}
	
	common_leave_stage();
}

# stage_makeenv:
#    Build up the environment
sub stage_makeenv {
	common_enter_stage("makeenv");
	# TODO
	common_leave_stage();
}

# stage_tbdeps:
#    Install toolbox dependencies
sub stage_tbdeps {
	my $fd;
	my @depsarray;
	my (%deps, %desc);
	
	common_enter_stage("tbdeps");
	
	# We alreay made the check, reading description should be OK
	open $fd, "$::TOOLBOXNAME/DESCRIPTION";
	%desc = read_description($fd);
	close($fd);
	
	# Make a hash depname => depvers
	@depsarray = split(/\s*,\s*/, $desc{"Depends"} || "");
	foreach (@depsarray) {
		if(/^(\S+?)\s*\(([<>]?=)\s*([^)]+)\)$/) { # toolbox-name (version-comparator version)
			$deps{$1} = "$2$3";
		}
		else {
			$deps{$_} = "*";
		}
	}
	
	common_log("Dependencies: " . join(",", map { "$_ $deps{$_}" } keys %deps));
	
	# Install dependencies
	close(common_exec_scilab("installToolbox('$_',1,'$deps{$_}')")) foreach(keys %deps);
	
	# Check if required toolboxes are installed
	my $tbpath = get_toolboxes_directory();
	foreach my $dep (keys %deps) {
		common_log("Checking $dep");
		if(! -r "$tbpath/$dep/DESCRIPTION") {
			common_die("Needed toolbox \"$dep\" is not installed");
		}
		
		next if($deps{$dep} eq "*");
		
		open $fd, "$tbpath/$dep/DESCRIPTION";
		my %desc2 = read_description($fd);
		close $fd;
		
		$deps{$dep} =~ /^([<>]?=)(.+)$/;
		
		# You can see this as "installed_version - required_version"
		my $cmp = compare_versions($desc2{"Version"}, $2);
		
		if($1 eq ">=" && $cmp == -1) { # <=> !($cmp >= 0) <=> !(installed >= required)
			common_die("We need \"$2\" >= $1, but version $desc2{Version} installed");
		}
		elsif($1 eq "=" && $cmp != 0) {
			common_die("We need \"$2\" == $1, but version $desc2{Version} installed");
		}
		elsif($cmp == 1) {  # <=> !($cmp <= 0) <=> !(installed <= required)
			common_die("We need \"$2\" <= $1, but version $desc2{Version} installed");
		}
	}
	
	common_leave_stage();
}

# stage_sysdeps:
#    Install system dependencies
sub stage_sysdeps {
	common_enter_stage("sysdeps");
	# TODO
	common_leave_stage();
}

# stage_build:
#     Run the build script
sub stage_build {
	common_enter_stage("build");
	
	# Generate ccbuilder.sce (see __DATA__ section)
	common_log("Generating ccbuilder.sce");
	my $ccbuilder;
	$ccbuilder .= $_ while(<DATA>);
	open CCBUILDER, ">ccbuilder.sce";
	print CCBUILDER $ccbuilder;
	close CCBUILDER;
	common_log("Generated ccbuilder.sce:\n$ccbuilder");
	
	# Run build script
	common_log("Running ccbuilder.sce");
	my $fd = common_exec_scilab("chdir('$::TOOLBOXNAME'); exec('../ccbuilder.sce')");
	
	# Check libraries
	common_log("Checking built libraries");
	my $done = 0;
	
	while(<$fd>) {
		$done = 1 if(/^atoms_cc_builder:done\r?$/);
		if(/^atoms_cc_ilib_compile:\s*(.+?)\s*$/) {
			common_log("Found built library $1");
			common_die("Generated library \"$1\" is invalid") unless($1 && ! -d $1 && (-x $1 || $^O =~ /win/i));
		}
	}
	
	# Check macros
	common_log("Checking built macros");
	open my $fd_fundesc, "$::TOOLBOXNAME/DESCRIPTION-FUNCTIONS";
	my %allowed_funcs = read_description_functions($fd_fundesc);
	close $fd_fundesc;
	
	seek($fd, 0, 0);
	while(<$fd>) {
		common_die("Error while building macros") if(/^atoms_cc_genlib:0\s*$/);
		if(/^atoms_cc_genlib_funcs:(\S+)\s*$/) {
			foreach my $func (split(/,/, $1)) {
				common_log("Found macro function $func");
				common_die("Generated function \"$func\" (from macros) not described in ".
				           "DESCRIPTION-FUNCTIONS file.")
					unless grep { $_ eq $func } keys %allowed_funcs;
			}
		}
	}
	
	# TODO: check doc
	
	close $fd;
	
	common_die("builder.sce script didn't terminate normally") unless($done);
	common_leave_stage();
}

# stage_pack:
#     Make the archive
sub stage_pack {
	common_enter_stage("pack");
	
	my @files = qw(readme.txt license.txt changelog.txt DESCRIPTION-FUNCTIONS
		DESCRIPTION macros src help sci_gateway demos tests locales includes loader.sce);
	push(@files, "etc/$::TOOLBOXNAME.start");
	push(@files, "etc/$::TOOLBOXNAME.quit");
	
	my @realfiles;
	foreach (@files) {
		push(@realfiles, "$::TOOLBOXNAME/$_") if -r "$::TOOLBOXNAME/$_";
	}
	
	my $output = get_output_filename();
	
	common_log("Making binary .tar.gz archive ($output.tar.gz)");
	close common_exec("tar", "-czvf", "$output.tar.gz", @realfiles,
		{"stderr_to_stdout" => 1});
	common_log("Making binary .zip archive ($output.zip)");
	close common_exec("zip", "-r", "$output.zip", @realfiles);
	
	common_leave_stage();
}

# stage_cleanenv:
#     Clean up the environment
sub stage_cleanenv {
	common_enter_stage("cleanenv");
	# TODO
	common_leave_stage();
}

# stage_test_makeenv
#     Build up the testing environment
sub stage_test_makeenv {
	common_enter_stage("test_makeenv");
	# TODO
	common_leave_stage();
}

# stage_test_tbdeps
#     Install toolbox dependencies in the testing environment
sub stage_test_tbdeps {
	common_enter_stage("test_tbdeps");
	# TODO ; since cleanenv is not implemented there's no need to re-install dependencies
	common_leave_stage();
}

# stage_test_sysdeps
#     Install system dependencies in the testing environment
sub stage_test_sysdeps {
	common_enter_stage("test_sysdeps");
	# TODO ; since cleanenv is not implemented there's no need to re-install dependencies
	common_leave_stage();
}

# stage_test_setuptb
#     Install toolbox in the testing environment
sub stage_test_setuptb {
	common_enter_stage("test_setuptb");
	
	close common_exec("tar", "-xzvf", cwd() . "/" . get_output_filename() . ".tar.gz",
		{"working_dir" => get_toolboxes_directory(),
		 "stderr_to_stdout" => 1});
	
	common_leave_stage();
}

# stage_test_runtests
#     Run the tests
sub stage_test_runtests {
	common_enter_stage("test_runtests");
	# TODO
	common_leave_stage();
}

# stage_test_cleanenv
#     Clean up the testing environment
sub stage_test_cleanenv {
	common_enter_stage("test_cleanenv");
	# TODO
	common_leave_stage();
}

# Overwrite some scilab functions to get its return value and extra infos
__DATA__
predef(0);
funcprot(0);

old_ilib_compile = ilib_compile;
function libn = ilib_compile(lib_name,makename,files,ldflags,cflags,fflags)
    libn = old_ilib_compile(lib_name,makename,files,ldflags,cflags,fflags);
    mprintf("\natoms_cc_ilib_compile:%s/%s\n", pwd(), libn);
endfunction

old_genlib = genlib;
function [success,funcs,success_files,failed_files] = genlib(nam,path,force,verbose,names)
	if exists('names', 'local') then
		[success,funcs,success_files,failed_files] = old_genlib(nam,path,force,verbose,names)
	elseif exists('verbose', 'local') then
		[success,funcs,success_files,failed_files] = old_genlib(nam,path,force,verbose)
	elseif exists('force', 'local') then
		[success,funcs,success_files,failed_files] = old_genlib(nam,path,force)
	elseif exists('path', 'local') then
		[success,funcs,success_files,failed_files] = old_genlib(nam,path)
	else
		[success,funcs,success_files,failed_files] = old_genlib(nam)
	end	
	
	mprintf("\natoms_cc_genlib:%i\n", int32(success));
	mprintf("atoms_cc_genlib_funcs:%s\n", strcat(funcs,","));
endfunction

exec("builder.sce");
mprintf("\natoms_cc_builder:done\n");

