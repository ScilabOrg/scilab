// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->


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

// Load the 2nd scenario : See scene11.test.atoms.scilab.org.txt
// =============================================================================
atomsRepositorySetOfl("http://scene10.test.atoms.scilab.org");

// Install toolbox N°2
atomsInstall(["toolbox_2","2.0"],"user");

// Check installation
if ~ atomsIsInstalled(["toolbox_1","1.0","user"]) then pause, end
if ~ atomsIsInstalled(["toolbox_2","2.0","user"]) then pause, end

atomsLoad(["toolbox_2","2.0","user"]);

if ~ atomsIsLoaded("toolbox_2")                  then pause, end
if ~ atomsIsLoaded(["toolbox_2","2.0"])          then pause, end
if ~ atomsIsLoaded(["toolbox_2","2.0","user"])   then pause, end

if ~ atomsIsLoaded("toolbox_1")                  then pause, end
if ~ atomsIsLoaded(["toolbox_1","1.0"])          then pause, end
if ~ atomsIsLoaded(["toolbox_1","1.0","user"])   then pause, end

if atomsIsLoaded(["toolbox_1","1.0","allusers"]) then pause, end
if atomsIsLoaded(["toolbox_2","2.0","allusers"]) then pause, end

A = [ "toolbox_1" "1.0" "user"     ; ..
      "toolbox_2" "2.0" "user"     ; ..
      "toolbox_2" ""    "user"     ; ..
      "toolbox_2" "2.0" ""         ; ..
      "toolbox_2" ""    ""         ; ..
      "toolbox_2" "2.0" "allusers" ];

if or(atomsIsLoaded(A) <> [%T ; %T ; %T ; %T ; %T ; %F]) then pause, end

// Remove toolbox_2
// =============================================================================
atomsRemove("toolbox_2","user");

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