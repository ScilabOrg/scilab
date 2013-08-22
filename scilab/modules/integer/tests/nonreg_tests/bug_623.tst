// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2006-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 623 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=623
//
// <-- Short Description -->
//    Even after applying the patch corresponding to bug report
//    608, integer cast to boolean is still not perfect.

uno =  int8(1);
1   == [uno uno];

if or(1<>[uno uno])     then pause,end
if or([1 1]<>[uno uno]) then pause,end
if or([uno uno]<>1)     then pause,end
if or([uno uno]<>[1 1]) then pause,end

if execstr("uint8(1) & %t" ,"errcatch")  <> 144 then pause,end
if execstr("%t&uint8(1)"   ,"errcatch")  <> 144 then pause,end
if execstr("int32(1) & %t" ,"errcatch")  <> 144 then pause,end
if execstr("%t&int32(1)"   ,"errcatch")  <> 144 then pause,end
if execstr("uint32(1) & %t","errcatch")  <> 144 then pause,end
if execstr("%t&uint32(1)"  ,"errcatch")  <> 144 then pause,end
if execstr("int16(1) & %t" ,"errcatch")  <> 144 then pause,end
if execstr("%t&int16(1)"   ,"errcatch")  <> 144 then pause,end
if execstr("int8(1) & %f " ,"errcatch")  <> 144 then pause,end
if execstr("%t&int8(1)"    ,"errcatch")  <> 144 then pause,end
