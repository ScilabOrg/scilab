// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

// If previous test did not end properly, restore, else backup config file
atomsRestoreConfig(%T);
atomsSaveConfig();

// Do not use the autoload system
atomsSetConfig("autoloadAddAfterInstall","False");
atomsSetConfig("Verbose" ,"False");

// Load the 1st scenario : See scene10.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene10.6.0.test.atoms.scilab.org");

// Install the toolbox 5
// =============================================================================

atomsInstall("toolbox_5V6");

// Check if the module is really installed
if ~atomsIsInstalled("toolbox_5V6")           then pause, end
if ~atomsIsInstalled(["toolbox_5V6" "1.0"])   then pause, end
if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"]) then pause, end
if ~atomsIsInstalled("toolbox_4V6")           then pause, end
if ~atomsIsInstalled("toolbox_2V6")           then pause, end
if ~atomsIsInstalled("toolbox_1V6")           then pause, end

// Remove the module
atomsRemove("toolbox_5V6");

// Install the toolbox 5 (version is mentioned)
// =============================================================================

atomsInstall(["toolbox_5V6" "1.0"]);

// Check if the module is really installed
if ~atomsIsInstalled("toolbox_5V6")           then pause, end
if ~atomsIsInstalled(["toolbox_5V6" "1.0"])   then pause, end
if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"]) then pause, end
if ~atomsIsInstalled("toolbox_4V6")           then pause, end
if ~atomsIsInstalled("toolbox_2V6")           then pause, end
if ~atomsIsInstalled("toolbox_1V6")           then pause, end

// Remove the module
atomsRemove("toolbox_5V6");

// Install the toolbox 5 (version + packaging version are mentioned)
// =============================================================================

atomsInstall(["toolbox_5V6" "1.0-1"]);

// Check if the module is really installed
if ~atomsIsInstalled("toolbox_5V6")           then pause, end
if ~atomsIsInstalled(["toolbox_5V6" "1.0"])   then pause, end
if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"]) then pause, end
if ~atomsIsInstalled("toolbox_4V6")           then pause, end
if ~atomsIsInstalled("toolbox_2V6")           then pause, end
if ~atomsIsInstalled("toolbox_1V6")           then pause, end

// Remove the module
atomsRemove("toolbox_5V6");

// Install the toolbox 5 (user section)
// =============================================================================

atomsInstall("toolbox_5V6","user");

if ~atomsIsInstalled("toolbox_5V6","user") then pause, end
if ~atomsIsInstalled("toolbox_4V6","user") then pause, end
if ~atomsIsInstalled("toolbox_2V6","user") then pause, end
if ~atomsIsInstalled("toolbox_1V6","user") then pause, end

if atomsIsInstalled("toolbox_5V6","allusers") then pause, end
if atomsIsInstalled("toolbox_4V6","allusers") then pause, end
if atomsIsInstalled("toolbox_2V6","allusers") then pause, end
if atomsIsInstalled("toolbox_1V6","allusers") then pause, end

atomsRemove("toolbox_5V6","user");

if atomsIsInstalled("toolbox_5V6","user") then pause, end
if atomsIsInstalled("toolbox_4V6","user") then pause, end
if atomsIsInstalled("toolbox_2V6","user") then pause, end
if atomsIsInstalled("toolbox_1V6","user") then pause, end

// Install the toolbox 5 (allusers section)
// =============================================================================

atomsInstall("toolbox_5V6","allusers");

if ~atomsIsInstalled("toolbox_5V6","allusers") then pause, end
if ~atomsIsInstalled("toolbox_4V6","allusers") then pause, end
if ~atomsIsInstalled("toolbox_2V6","allusers") then pause, end
if ~atomsIsInstalled("toolbox_1V6","allusers") then pause, end

if atomsIsInstalled("toolbox_5V6","user") then pause, end
if atomsIsInstalled("toolbox_4V6","user") then pause, end
if atomsIsInstalled("toolbox_2V6","user") then pause, end
if atomsIsInstalled("toolbox_1V6","user") then pause, end

atomsRemove("toolbox_5V6","allusers");

if atomsIsInstalled("toolbox_5V6","allusers") then pause, end
if atomsIsInstalled("toolbox_4V6","allusers") then pause, end
if atomsIsInstalled("toolbox_2V6","allusers") then pause, end
if atomsIsInstalled("toolbox_1V6","allusers") then pause, end

// Install the toolbox 5 (Both section)
// =============================================================================

atomsInstall("toolbox_5V6","allusers");
atomsInstall("toolbox_5V6","user");

if ~atomsIsInstalled("toolbox_5V6","allusers") then pause, end
if ~atomsIsInstalled("toolbox_4V6","allusers") then pause, end
if ~atomsIsInstalled("toolbox_2V6","allusers") then pause, end
if ~atomsIsInstalled("toolbox_1V6","allusers") then pause, end

if ~atomsIsInstalled("toolbox_5V6","user") then pause, end
if ~atomsIsInstalled("toolbox_4V6","user") then pause, end
if ~atomsIsInstalled("toolbox_2V6","user") then pause, end
if ~atomsIsInstalled("toolbox_1V6","user") then pause, end

atomsRemove("toolbox_5V6","allusers");
if atomsIsInstalled("toolbox_5V6","allusers") then pause, end
if atomsIsInstalled("toolbox_4V6","allusers") then pause, end
if atomsIsInstalled("toolbox_2V6","allusers") then pause, end
if atomsIsInstalled("toolbox_1V6","allusers") then pause, end

if ~atomsIsInstalled("toolbox_5V6","user") then pause, end
if ~atomsIsInstalled("toolbox_4V6","user") then pause, end
if ~atomsIsInstalled("toolbox_2V6","user") then pause, end
if ~atomsIsInstalled("toolbox_1V6","user") then pause, end

atomsInstall("toolbox_5V6","allusers");
atomsRemove("toolbox_5V6","user");

if atomsIsInstalled("toolbox_5V6","user") then pause, end
if atomsIsInstalled("toolbox_4V6","user") then pause, end
if atomsIsInstalled("toolbox_2V6","user") then pause, end
if atomsIsInstalled("toolbox_1V6","user") then pause, end

if ~atomsIsInstalled("toolbox_5V6","allusers") then pause, end
if ~atomsIsInstalled("toolbox_4V6","allusers") then pause, end
if ~atomsIsInstalled("toolbox_2V6","allusers") then pause, end
if ~atomsIsInstalled("toolbox_1V6","allusers") then pause, end

atomsRemove("toolbox_5V6","allusers");

// Install the toolbox 7 (Local package)
// =============================================================================

atomsInstall(SCI+"/modules/atoms/tests/unit_tests/toolbox_7V6_1.0-1.bin.zip","allusers");
atomsInstall(SCI+"/modules/atoms/tests/unit_tests/toolbox_7V6_1.0-1.bin.zip","user");

if ~atomsIsInstalled("toolbox_7V6","allusers") then pause, end
if ~atomsIsInstalled("toolbox_7V6","user")     then pause, end

atomsRemove("toolbox_7V6","allusers");

if atomsIsInstalled("toolbox_7V6","allusers")  then pause, end
if ~atomsIsInstalled("toolbox_7V6","user")     then pause, end

atomsInstall(SCI+"/modules/atoms/tests/unit_tests/toolbox_7V6_1.0-1.bin.zip","allusers");
atomsRemove("toolbox_7V6","user");

if atomsIsInstalled("toolbox_7V6","user")      then pause, end
if ~atomsIsInstalled("toolbox_7V6","allusers") then pause, end

atomsRemove("toolbox_7V6","allusers");

// Restore original values
// =============================================================================
atomsRestoreConfig(%T);

atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));
