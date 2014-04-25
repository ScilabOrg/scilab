// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13359 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13359
//
// <-- Short Description -->
// Nyquist datatip were not displaying negative frequencies properly
//
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

s=poly(0,'s');
h=syslin('c' , 1 / (s + 1));
nyquist(h);
title("Add datatip and check negative frequencies");
