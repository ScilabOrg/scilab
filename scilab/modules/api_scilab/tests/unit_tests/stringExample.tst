// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/stringExample"));
cd(pathconvert(TMPDIR+"/stringExample"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/stringExample.c",pathconvert(TMPDIR+"/stringExample/stringExample.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("stringExample",["stringExample","stringExample"],"stringExample.c",[],"","",cflags);
exec("loader.sce");

             
a = "Scilab";
b = ["New","Scilab";"API","functions"];
if stringExample(a) <> a then pause, end
if stringExample(b) <> b then pause, end
 
       
