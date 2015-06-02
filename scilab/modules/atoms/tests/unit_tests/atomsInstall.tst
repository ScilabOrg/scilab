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
atomsRepositorySetOfl("http://scene10.test.atoms.scilab.org");

// Install the toolbox 5
// =============================================================================

atomsInstall("toolbox_5");

// Check if the module is really installed
if ~atomsIsInstalled("toolbox_5")           then pause, end
if ~atomsIsInstalled(["toolbox_5" "1.0"])   then pause, end
if ~atomsIsInstalled(["toolbox_5" "1.0-1"]) then pause, end
if ~atomsIsInstalled("toolbox_4")           then pause, end
if ~atomsIsInstalled("toolbox_2")           then pause, end
if ~atomsIsInstalled("toolbox_1")           then pause, end

// Remove the module
atomsRemove("toolbox_5");

// Install the toolbox 5 (version is mentioned)
// =============================================================================

atomsInstall(["toolbox_5" "1.0"]);

// Check if the module is really installed
if ~atomsIsInstalled("toolbox_5")           then pause, end
if ~atomsIsInstalled(["toolbox_5" "1.0"])   then pause, end
if ~atomsIsInstalled(["toolbox_5" "1.0-1"]) then pause, end
if ~atomsIsInstalled("toolbox_4")           then pause, end
if ~atomsIsInstalled("toolbox_2")           then pause, end
if ~atomsIsInstalled("toolbox_1")           then pause, end

// Remove the module
atomsRemove("toolbox_5");

// Install the toolbox 5 (version + packaging version are mentioned)
// =============================================================================

atomsInstall(["toolbox_5" "1.0-1"]);

// Check if the module is really installed
if ~atomsIsInstalled("toolbox_5")           then pause, end
if ~atomsIsInstalled(["toolbox_5" "1.0"])   then pause, end
if ~atomsIsInstalled(["toolbox_5" "1.0-1"]) then pause, end
if ~atomsIsInstalled("toolbox_4")           then pause, end
if ~atomsIsInstalled("toolbox_2")           then pause, end
if ~atomsIsInstalled("toolbox_1")           then pause, end

// Remove the module
atomsRemove("toolbox_5");

// Install the toolbox 5 (user section)
// =============================================================================

atomsInstall("toolbox_5","user");

if ~atomsIsInstalled("toolbox_5","user") then pause, end
if ~atomsIsInstalled("toolbox_4","user") then pause, end
if ~atomsIsInstalled("toolbox_2","user") then pause, end
if ~atomsIsInstalled("toolbox_1","user") then pause, end

if atomsIsInstalled("toolbox_5","allusers") then pause, end
if atomsIsInstalled("toolbox_4","allusers") then pause, end
if atomsIsInstalled("toolbox_2","allusers") then pause, end
if atomsIsInstalled("toolbox_1","allusers") then pause, end

atomsRemove("toolbox_5","user");

if atomsIsInstalled("toolbox_5","user") then pause, end
if atomsIsInstalled("toolbox_4","user") then pause, end
if atomsIsInstalled("toolbox_2","user") then pause, end
if atomsIsInstalled("toolbox_1","user") then pause, end

// Install the toolbox 5 (allusers section)
// =============================================================================

atomsInstall("toolbox_5","allusers");

if ~atomsIsInstalled("toolbox_5","allusers") then pause, end
if ~atomsIsInstalled("toolbox_4","allusers") then pause, end
if ~atomsIsInstalled("toolbox_2","allusers") then pause, end
if ~atomsIsInstalled("toolbox_1","allusers") then pause, end

if atomsIsInstalled("toolbox_5","user") then pause, end
if atomsIsInstalled("toolbox_4","user") then pause, end
if atomsIsInstalled("toolbox_2","user") then pause, end
if atomsIsInstalled("toolbox_1","user") then pause, end

atomsRemove("toolbox_5","allusers");

if atomsIsInstalled("toolbox_5","allusers") then pause, end
if atomsIsInstalled("toolbox_4","allusers") then pause, end
if atomsIsInstalled("toolbox_2","allusers") then pause, end
if atomsIsInstalled("toolbox_1","allusers") then pause, end

// Install the toolbox 5 (Both section)
// =============================================================================

atomsInstall("toolbox_5","allusers");
atomsInstall("toolbox_5","user");

if ~atomsIsInstalled("toolbox_5","allusers") then pause, end
if ~atomsIsInstalled("toolbox_4","allusers") then pause, end
if ~atomsIsInstalled("toolbox_2","allusers") then pause, end
if ~atomsIsInstalled("toolbox_1","allusers") then pause, end

if ~atomsIsInstalled("toolbox_5","user") then pause, end
if ~atomsIsInstalled("toolbox_4","user") then pause, end
if ~atomsIsInstalled("toolbox_2","user") then pause, end
if ~atomsIsInstalled("toolbox_1","user") then pause, end

atomsRemove("toolbox_5","allusers");
if atomsIsInstalled("toolbox_5","allusers") then pause, end
if atomsIsInstalled("toolbox_4","allusers") then pause, end
if atomsIsInstalled("toolbox_2","allusers") then pause, end
if atomsIsInstalled("toolbox_1","allusers") then pause, end

if ~atomsIsInstalled("toolbox_5","user") then pause, end
if ~atomsIsInstalled("toolbox_4","user") then pause, end
if ~atomsIsInstalled("toolbox_2","user") then pause, end
if ~atomsIsInstalled("toolbox_1","user") then pause, end

atomsInstall("toolbox_5","allusers");
atomsRemove("toolbox_5","user");

if atomsIsInstalled("toolbox_5","user") then pause, end
if atomsIsInstalled("toolbox_4","user") then pause, end
if atomsIsInstalled("toolbox_2","user") then pause, end
if atomsIsInstalled("toolbox_1","user") then pause, end

if ~atomsIsInstalled("toolbox_5","allusers") then pause, end
if ~atomsIsInstalled("toolbox_4","allusers") then pause, end
if ~atomsIsInstalled("toolbox_2","allusers") then pause, end
if ~atomsIsInstalled("toolbox_1","allusers") then pause, end

atomsRemove("toolbox_5","allusers");

// Install the toolbox 7 (Local package)
// =============================================================================

atomsInstall(SCI+"/modules/atoms/tests/unit_tests/toolbox_7_1.0-1.bin.zip","allusers");
atomsInstall(SCI+"/modules/atoms/tests/unit_tests/toolbox_7_1.0-1.bin.zip","user");

if ~atomsIsInstalled("toolbox_7","allusers") then pause, end
if ~atomsIsInstalled("toolbox_7","user")     then pause, end

atomsRemove("toolbox_7","allusers");

if atomsIsInstalled("toolbox_7","allusers")  then pause, end
if ~atomsIsInstalled("toolbox_7","user")     then pause, end

atomsInstall(SCI+"/modules/atoms/tests/unit_tests/toolbox_7_1.0-1.bin.zip","allusers");
atomsRemove("toolbox_7","user");

if atomsIsInstalled("toolbox_7","user")      then pause, end
if ~atomsIsInstalled("toolbox_7","allusers") then pause, end

atomsRemove("toolbox_7","allusers");

// Restore original values
// =============================================================================
atomsRestoreConfig(%T);

atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));
