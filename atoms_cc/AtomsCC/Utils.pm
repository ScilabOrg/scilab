package AtomsCC::Utils;

use strict;
use Exporter;

use AtomsCC::Common;

our @ISA = ('Exporter');
our @EXPORT = qw(&get_tree &get_toolboxes_directory &read_description &is_zip
                 &read_description_functions &read_file_from_archive &check_tree
				 &get_output_filename &compare_versions);

# is_zip:
#    Return true if toolbox file extension is zip
sub is_zip {
	return $::TOOLBOXFILE =~ /\.zip$/;
}

# get_tree_from_tgz:
#   Get all files (names) of the compressed (in tar.gz) sources
sub get_tree_from_tgz {
	my %files;
	
	my $fd = common_exec("tar", "-tzf", $::TOOLBOXFILE);
	
	while(<$fd>) {
		chomp;
		$files{$_} = 1;
	}
	
	close $fd;
	return %files;
}

# get_tree_from_zip:
#   Get all files (names) of the compressed (in zip) sources
sub get_tree_from_zip {
	my (%files, $line);
	
	# tail & head are here to skip header & footer
	my $fd = common_exec("unzip", "-l", $::TOOLBOXFILE);
	
	while(<$fd>) {
		if(((/^\s*-+/)...(/^\s*-+/)) && !/^\s*-+/) { # Delete header & footer
			# zip output format: size date time filename
			/\s*\d+\s+\d+-\d+-\d+\s+\d+:\d+\s+(.+)/ or common_die "Bad output of unzip";
			chomp $1;
			$files{$1} = 1;
		}
	}
	
	close $fd;
	return %files;
}

# get_tree:
#   Get all files (names) of the compressed sources, in a hash
#   (hash values are meaningless, set to 1)
sub get_tree {
	if(is_zip()) {
		return get_tree_from_zip();
	}
	else {
		return get_tree_from_tgz();
	}
}

# read_file_from_tgz(filename):
#    Extract given file from the .tar.gz archive
sub read_file_from_tgz {
	my $filename = shift;
	return common_exec("tar", "-xzOf", $::TOOLBOXFILE, "$::TOOLBOXNAME/$filename");
}

# read_file_from_zip(filename):
#    Extract given file from the .zip archive
sub read_file_from_zip {
	my $filename = shift;
	return common_exec("unzip", "-p", $::TOOLBOXFILE, "$::TOOLBOXNAME/$filename");
}

# read_file_from_archive(filename):
#   Extract given file from the archive
sub read_file_from_archive {
	if(is_zip()) {
		return read_file_from_zip(@_);
	}
	else {
		return read_file_from_tgz(@_);
	}
}

# read_description(*description):
#   Check if DESCRIPTION file is correct, and parse it (return a hash
#   field => value).
#   First argument is a file descriptor for the DESCRIPTION file (see
#   get_description)
sub read_description {
	my $fd = shift;
	my @required = qw(Toolbox Version Title Author Maintainer License
	                  Description ScilabVersion Category);
	my @optional = qw(Date Depends URL Entity);
	my (%infos, $key, $val);
	my (%lines, %correct);
	
	# Populate hash
	while(<$fd>) {
		common_die "\":\" not followed by a space at line $." if(/:(?! )/);
		if(/:/) { # New field
			($key, $val) = split(/: /, $_, 2);
			$infos{$key} = $val;
			$lines{$key} = $.;
			$correct{$key} = 0;
		}
		else { # Continuation of previous field
			$infos{$key} .= $_;
		}
	}
	
	# Check presence of required fields, mark them as correct
	foreach (@required) {
		if(!defined($infos{$_})) {
			common_die "Mandatory field \"$_\" not defined";
		}
		else {
			$correct{$_} = 1;
		}
	}
	
	# Mark optional fields as correct
	foreach (@optional) {
		if(defined($infos{$_})) {
			$correct{$_} = 1;
		}
	}
	
	# Check that there's no incorrect (= unknown) fields
	foreach (keys(%infos)) {
		if($correct{$_} == 0) {
			common_die "Unknown field \"$_\" (defined at line $lines{$_})";
		}
	}
	
	chomp %infos;
	return %infos;
}

# read_description_functions(*description_functions):
#   Parse DESCRIPTION-FUNCTIONS file (and check it, too). Like DESCRIPTION,
#   first argument is a file descriptor. Returns a hash function name =>
#   function description
sub read_description_functions {
	my $fd = shift;
	my (%funcs, $func, $desc);
	
	while(<$fd>) {
		if(/-/ && !/ - /) {
			common_die "\"-\" not surrounded by spaces at line $.";
		}
		
		if(/-/) { # New field
			($func, $desc) = split(/ - /, $_, 2);
			$funcs{$func} = $desc;
		}
		else { # Previous function description continuation
			$funcs{$func} .= $_;
		}
	}
	
	chomp %funcs;
	
	return %funcs;
}

# get_toolboxes_directory:
#   Get path where Scilab toolboxes are installed
sub get_toolboxes_directory {
	my $fd = common_exec_scilab("printf('path: %s\\n', cd(atomsToolboxDirectory()))");
	my $tbpath;
	
	while(<$fd>) {
		if(/^path: (.+?)\r?$/) {
			$tbpath = $1;
			last;
		}
	}
	
	close $fd;
	
	if(!defined($tbpath)) {
		common_die("Can't find toolboxes directory");
	}
	
	common_log("Toolboxes directory: $tbpath\n");
	
	return $tbpath;
}

# get_output_filename:
#     Get output filename, without the extension
sub get_output_filename {
	my $output = $::TOOLBOXFILE;
	$output =~ s/(\.zip|\.tar.gz)$//;
	$output .= "-bin";
	return $output;
}

# check_tree(%tree):
#   Given a source tree of a toolbox (see get_tree), check if it is correct
#   (required files are present, files are at their right place, and so on...)
sub check_tree {
	my %tree = @_;
	my %newtree;
	
	# Check that all files are under a root which has the same name as the toolbox
	# Delete this root to simplify other tests
	foreach (keys %tree) {
		if(s#^\Q$::TOOLBOXNAME\E(/|$)##) {
			$newtree{$_} = 1 if $_;
		}
		else {
			common_die "Incorrect archive: \"$_\" is not a child of \"$::TOOLBOXNAME\"";
		}
	}
	%tree = %newtree;
	
	# Check that basic files are here
	my @required = qw(DESCRIPTION DESCRIPTION-FUNCTIONS readme.txt license.txt
	                  changelog.txt builder.sce);
	push(@required, "etc/$::TOOLBOXNAME.start");
	push(@required, "etc/$::TOOLBOXNAME.quit");
	
	foreach (@required) {
		if(!defined($tree{$_})) {
			common_die "Incorrect archive: required file \"$_\" not present";
		}
	}
	
	# macros/ must contain only .sci and .sce files
	foreach (grep { $_ =~ m#^macros/# } keys %tree) {
		if(!/(\.sc[ie]|\/)$/) {
			common_die "Incorrect archive: macros/ must contain only .sci and .sce files".
			    " (\"$_\" found)";
		}
	}
	
	# All fortran files must be in src/fortran
	foreach (grep { $_ =~ /\.f$/} keys %tree) {
		if(!m#^src/fortran/#) {
			common_die "Incorrect archive: \"$_\" is a fortran source and hence has to be in ".
			    "src/fortran";
		}
	}

	# All c files must be in src/c or sci_gateway/{c,fortran}
	foreach (grep { $_ =~ /\.[ch]$/} keys %tree) {
		if(!m#^(src/c|sci_gateway/(c|fortran))/#) {
			common_die "Incorrect archive: \"$_\" is a C source and hence has to be in ".
			    "src/c, sci_gateway/c or sci_gateway/fortran";
		}
	}
	
	# Constraints: if $key exists, $constraints{$key} must exist
	my %constraints = (
		qr#^help/([a-z][a-z]_[A-Z][A-Z])/[^/]+\.xml$# => sub{ "help/$1/build_help.sce" },
		qr#^help/([a-z][a-z]_[A-Z][A-Z])/build_help.sce$# => sub{ "help/builder_help.sce" },
		qr#^macros/.+\.sc[ie]$# => sub{ "macros/buildmacros.sce" });
	
	# Build constraints for allowed languages
	my %languages = (
		"c" => qr/[ch]/,
		"fortran" => qr/f/);
	
	foreach (keys %languages) {
		# if src/(lang) has source files, src/(lang)/builder_(lang).sce must exist
		$constraints{qr#^src/($_)/.+\.$languages{$_}$#} = sub{ "src/$1/builder_$1.sce" };
		
		# if sci_gateway/(lang) has C sources, sci_gateway/(lang)/builder_gateway_(lang).sce
		# must exist
		$constraints{qr#^sci_gateway/($_)/.+[ch]$#} = sub{ "sci_gateway/$1/builder_gateway_$1.sce" };
		
		# if src/(lang)/builder_(lang).sce exist, src/builder_src.sce must exist
		$constraints{qr#^src/$_/builder_$_.sce$#} = sub{ "src/builder_src.sce" };
		
		# if sci_gateway/(lang)/builder_gateway_(lang).sce exist, sci_gateway/builder_gateway.sce must exist
		$constraints{qr#^sci_gateway/$_/builder_gateway_$_.sce$#} = sub{ "sci_gateway/builder_gateway.sce" };
	}
	
	# Check constraints
	foreach my $constraint (keys %constraints) {
		foreach my $file (keys %tree) {
			if($file =~ $constraint) {
				my $required = $constraints{$constraint}();
				common_die "Invalid archive: \"$&\" needs \"$required\", which isn't in the archive"
					unless(defined($tree{$required}));
			}
		}
	}
}

# compare_versions:
#    Returns -1 if version a < version b, 0 if equals, 1 else
sub compare_versions {
	my $versa = shift;
	my $versb = shift;
	my @va = split(/\./, $versa);
	my @vb = split(/\./, $versb);
	
	if($#va < $#vb) {
		return -compare_versions($versb, $versa);
	}
	
	for(my $i = 0; $i < $#vb; ++$i) {
		return  1 if $va[$i] > $vb[$i];
		return -1 if $va[$i] < $vb[$i];
	}
	
	return 1 if($#va > $#vb);
	return 0;
}
