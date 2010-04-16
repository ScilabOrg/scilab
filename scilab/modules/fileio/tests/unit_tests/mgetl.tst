// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- JVM NOT MANDATORY -->
// =============================================================================
// Unitary tests for mgetl function
//==============================================================================
if mgetl('SCI/etc/scilab.start', 0) <> [] then pause,end
//==============================================================================
fd = mopen('SCI/etc/scilab.start', 'rt');
if mgetl(fd, 0) <> [] then pause,end
mclose(fd);
//==============================================================================
ref = ["1 Scilab";"2 Puffin";"3 ";"4 Puffin";"5 Scilab"];
mputl(ref, TMPDIR + "/text.txt");
//==============================================================================
txt = mgetl(TMPDIR + "/text.txt");
if or(ref <> txt) then pause,end
//==============================================================================
txt = mgetl(TMPDIR + "/text.txt", -10);
if or(ref <> txt) then pause,end
//==============================================================================
txt = mgetl(TMPDIR + "/text.txt", -1);
if or(ref <> txt) then pause,end
//==============================================================================
txt = mgetl(TMPDIR + "/text.txt", 1);
if txt <> ref(1) then pause,end
//==============================================================================
txt = mgetl(TMPDIR + "/text.txt", 2);
if or(txt <> ref(1:2)) then pause,end
//==============================================================================
txt = mgetl(TMPDIR + "/text.txt", 100);
if or(txt <> ref) then pause,end
//==============================================================================
fd = mopen(TMPDIR + "/text2.txt","wt");
r = mgetl(fd);
mclose(fd);
if r <> [] then pause,end
//==============================================================================
fd = mopen(TMPDIR + "/text.txt","rt");
r = mgetl(fd, 1);
if r <> ref(1) then pause,end
//==============================================================================
r = mgetl(fd, 2);
if r <> ref(2:3) then pause,end
//==============================================================================
r = mgetl(fd, 3);
if r <> ref(4:5) then pause,end
//==============================================================================
mclose(fd);
//==============================================================================
txt = mgetl("SCI/modules/fileio/tests/unit_tests/text.txt");
if and(size(txt) <> [6 1]) then pause,end
//==============================================================================
