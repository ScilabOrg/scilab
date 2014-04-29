// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre Lando
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 6958 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6958
//
// <-- Short Description -->
// The "ticks_labels" property from "axes" rejected column vector of labels.

v0=string(0:10);
drawaxis(x=0:10,y=0.5,dir="u",ticks="v",val=v0);
e=gce();
if execstr("e.ticks_labels=v0"  ,"errcatch") <> 0 then pause, end  // Should work now.
if execstr("e.ticks_labels=v0''","errcatch") <> 0 then pause, end

