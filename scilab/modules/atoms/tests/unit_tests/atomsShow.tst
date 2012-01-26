// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

// If previous test did not end properly, restore, else backup config file
config_downloadTool = atomsGetConfig("downloadTool");
if isfile(atomsPath("system","user")+"config.bak") then
        movefile(atomsPath("system","user")+"config.bak",atomsPath("system","user")+"config");
end
if isfile(atomsPath("system","user")+"config") then
        movefile(atomsPath("system","user")+"config",atomsPath("system","user")+"config.bak");
end

// Do not use the autoload system
atomsSetConfig("autoloadAddAfterInstall","False");
atomsSetConfig("Verbose" ,"False");

// Load the 1st scenario : See scene10.test.atoms.scilab.org.txt
// =============================================================================
atomsRepositorySetOfl("http://scene10.test.atoms.scilab.org");

// Install the toolbox 5
// =============================================================================

atomsInstall("toolbox_3");
atomsShow("toolbox_3");
atomsRemove("toolbox_3");

// Restore original values
// =============================================================================
if isfile(atomsPath("system","user")+"config.bak") then
        movefile(atomsPath("system","user")+"config.bak",atomsPath("system","user")+"config");
else
        deletefile(atomsPath("system","user")+"config");
end
if isfile(atomsPath("system","user")+"config.bak") then
        deletefile(atomsPath("system","user")+"config.bak");
end

atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));