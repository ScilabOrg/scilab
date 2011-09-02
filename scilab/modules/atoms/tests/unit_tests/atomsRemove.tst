// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

// Do not use the autoload system
config_autoload = atomsGetConfig("autoloadAddAfterInstall");
config_Verbose  = atomsGetConfig("Verbose");
atomsSetConfig("autoloadAddAfterInstall","False");
atomsSetConfig("Verbose" ,"False");

// Load the 1st scenario : See scene10.test.atoms.scilab.org.txt
// =============================================================================
atomsRepositorySetOfl("http://scene1.test.atoms.scilab.org");

// REMOVING PART

// REMOVING an Automatic installed toolbox deletes the toolbox it has been 
// installed by
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
ref = [ "toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_2/1.0-1"  "A" ;
        "toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_5/1.0-1"  "I" ;
        "toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_4/1.0-1"  "A" ;
        "toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_1/1.0-1"  "A" ];
assert_checkequal(atomsRemove(["toolbox_2" "1.0"],"user"),ref);

// REMOVING an Automatic toolbox deletes all the toolboxes depending on it
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
atomsInstall(["toolbox_3" "1.0"],"user");
ref = [ "toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_1/1.0-1"  "A" ;
        "toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_5/1.0-1"  "I" ;
        "toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_4/1.0-1"  "A" ;
        "toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_2/1.0-1"  "A" ;
        "toolbox_3"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_3/1.0-1"  "I" ;        
        "toolbox_2"  "2.0-1"  "user"  "SCIHOME/atoms/toolbox_2/2.0-1"  "A" ];
assert_checkequal(atomsRemove(["toolbox_1"]      ,"user"),ref);

// REMOVING an Intentionnaly installed toolbox won't delete an Automatically
// installed toolbox if it is needed by another toolbox
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
atomsInstall(["toolbox_3" "1.0"],"user");
ref = [ "toolbox_3"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_3/1.0-1"  "I" ;        
        "toolbox_2"  "2.0-1"  "user"  "SCIHOME/atoms/toolbox_2/2.0-1"  "A" ];
assert_checkequal(atomsRemove(["toolbox_3" "1.0"],"user"),ref);

// REMOVING a toolbox in a section won't delete it in other sections
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
atomsInstall(["toolbox_5" "1.0"],"allusers");
ref = [ "toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_5/1.0-1"  "I" ;
        "toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_4/1.0-1"  "A" ;
        "toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_2/1.0-1"  "A" ;
        "toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_1/1.0-1"  "A" ];
ref_all = [ "toolbox_5"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_5/1.0-1"  "I" ;
        "toolbox_4"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_4/1.0-1"  "A" ;
        "toolbox_2"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_2/1.0-1"  "A" ;
        "toolbox_1"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_1/1.0-1"  "A" ];
assert_checkequal(atomsRemove(["toolbox_5" "1.0"],"user"),ref);
assert_checkequal(atomsRemove(["toolbox_5" "1.0"],"allusers"),ref_all);

// REMOVING a toolbox with section "all" will remove this toolbox in sections 
// "alluser" AND "user"
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
atomsInstall(["toolbox_5" "1.0"],"allusers");
ref = [ "toolbox_5"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_5/1.0-1"  "I" ;
        "toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_5/1.0-1"  "I" ;
        "toolbox_4"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_4/1.0-1"  "A" ;
        "toolbox_2"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_2/1.0-1"  "A" ;
        "toolbox_1"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_1/1.0-1"  "A" ;
        "toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_4/1.0-1"  "A" ;
        "toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_2/1.0-1"  "A" ;
        "toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_1/1.0-1"  "A" ;];
assert_checkequal(atomsRemove(["toolbox_5" "1.0"],"all"),ref);

// REMOVING a toolbox from a section where it doesn't exist won't remove it from
// its existing section
// REMOVING an Intentionnaly installed toolbox deletes all the Automatically
// installed toolbox that were installed for this.
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
ref_empty = [];
ref = [ "toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_5/1.0-1"  "I" ;
        "toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_4/1.0-1"  "A" ;
        "toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_2/1.0-1"  "A" ;
        "toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_1/1.0-1"  "A" ];
assert_checkequal(atomsRemove(["toolbox_5" "1.0"],"allusers"),ref_empty);
assert_checkequal(atomsRemove(["toolbox_5" "1.0"],"user"),ref);

// ============================================================================
// DELETING PART
rmdir(SCIHOME+"/atoms/archives/",'s');
mkdir(SCIHOME+"/atoms/archives");
rmdir(SCI+"/contrib/archives/",'s');
mkdir(SCI+"/contrib/archives");
// ============================================================================

// REMOVING AND DELETING a toolbox will delete its archives and archives of the
// toolboxes it depends on
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
ref = [ "toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_5/1.0-1"  "I" ;
        "toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_4/1.0-1"  "A" ;
        "toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_2/1.0-1"  "A" ;
        "toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_1/1.0-1"  "A" ];
ref_empty = [];
assert_checkequal(atomsRemove(["toolbox_5" "1.0"],"user",%T),ref);
assert_checkequal(ls(atomsPath("install","user")+'archives/'),ref_empty);
rmdir(SCIHOME+"/atoms/archives/",'s');
mkdir(SCIHOME+"/atoms/archives");
rmdir(SCI+"/contrib/archives/",'s');
mkdir(SCI+"/contrib/archives");

// REMOVING AND DELETING a toolbox will not delete other archives than the ones
// it depends on
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
atomsInstall(["toolbox_6" "1.0"],"user");
ref = [ "toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_5/1.0-1"  "I" ;
        "toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_4/1.0-1"  "A" ;
        "toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_2/1.0-1"  "A" ];
ref_ls = [ "toolbox_1_1.0-1.bin.i686.linux.tar.gz";
           "toolbox_2_2.0-1.bin.i686.linux.tar.gz";
           "toolbox_6_1.0-1.bin.i686.linux.tar.gz" ];
ref_rem_6 = [ "toolbox_6"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_6/1.0-1"  "I" ;
              "toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/toolbox_1/1.0-1"  "A" ;
              "toolbox_2"  "2.0-1"  "user"  "SCIHOME/atoms/toolbox_2/2.0-1"  "A" ];
assert_checkequal(atomsRemove(["toolbox_5" "1.0"],"user",%T),ref);
assert_checkequal(ls(atomsPath("install","user")+'archives/'),ref_ls);

// cleaning
assert_checkequal(atomsRemove(["toolbox_6" "1.0"],"user",%T),ref_rem_6);
rmdir(atomsPath("install","user")+'archives/','s');
mkdir(atomsPath("install","user")+'archives/');
rmdir(atomsPath("install","allusers")+'archives/','s');
mkdir(atomsPath("install","allusers")+'archives/');

// no module should be installed
assert_checktrue(isempty(atomsGetInstalled()));


// Restore original values
// =============================================================================
atomsSetConfig("autoloadAddAfterInstall",config_autoload);
atomsSetConfig("Verbose" ,config_Verbose);
atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));
