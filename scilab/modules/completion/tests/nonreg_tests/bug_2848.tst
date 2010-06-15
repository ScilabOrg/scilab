// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2848 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2848
//
// <-- Short Description -->
//    When using the TAB key for completion in the console, after using once a Scilab
//    macro, its name appears twice in the completion list: as a macro and as a variable.


with_atlas();
[a,b,c,d]=completion('with');
if c <> [] then pause,end

with_tk();
[a,b,c,d]=completion('with');
if c <> [] then pause,end

clear with_atlas
clear with_tk
[a,b,c,d]=completion('with');
if c <> [] then pause,end

with_atlas();
with_tk();
deff('[x] = with_foo(y,z)','x=y+z');
[a,b,c,d] = completion('with');
ref_a = ['with_embedded_jre'; 'with_module'];
ref_b = [];
ref_c = [];
ref_d = ['with_atlas'; ..
'with_foo'; ..
'with_javasci'; ..
'with_lcc'; ..
'with_macros_source'; ..
'with_modelica_compiler'; ..
'with_pvm'; ..
'with_texmacs'; ..
'with_tk'];

if or(a <> ref_a) then pause, end
if or(b <> ref_b) then pause, end
if or(c <> ref_c) then pause, end
if or(d <> ref_d) then pause, end

clear with_atlas
clear with_tk
clear with_foo

[a,b,c,d]=completion('with');
if c <> [] then pause,end
