//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8383 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8383
//
// <-- Short Description -->
// export and import "void" and "undefined" fields in {m,l}list
//


//undefined test


//list(1, undef, 3)

list_undefined = list(1);
list_undefined(3) = 3;
save(TMPDIR + "/list_undefined1.sod", "list_undefined");
clear list_undefined;
load(TMPDIR + "/list_undefined1.sod");
if or(definedfields(list_undefined) == 2) == %t then pause, end

//list(undef, 3)

list_undefined = list(1);
list_undefined(3) = 3;
list_undefined(1) = null();
save(TMPDIR + "/list_undefined2.sod", "list_undefined");
clear list_undefined;
load(TMPDIR + "/list_undefined2.sod");
if or(definedfields(list_undefined) == 1) == %t then pause, end


//list(1, undef)

list_undefined = list(1);
list_undefined(3) = 3;
list_undefined(3) = null();
save(TMPDIR + "/list_undefined3.sod", "list_undefined");
clear list_undefined;
load(TMPDIR + "/list_undefined3.sod");
if or(definedfields(list_undefined) == 2) == %t then pause, end

//void test


//list(1, void, 3)

list_void = list(1,,3);
save(TMPDIR + "/list_void1.sod", "list_void");
clear list_void;
load(TMPDIR + "/list_void1.sod");
if type(list_void(2)) <> 0 then pause, end


//list(void, 3)

list_void = list(,2);
save(TMPDIR + "/list_void2.sod", "list_void");
clear list_void;
load(TMPDIR + "/list_void2.sod");
if type(list_void(1)) <> 0 then pause, end

//list(1, void)

list_void = list(1,);
save(TMPDIR + "/list_void3.sod", "list_void");
clear list_void;
load(TMPDIR + "/list_void3.sod");
if type(list_void(2)) <> 0 then pause, end

