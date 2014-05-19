// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 6286 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6286
//
// <-- Short Description -->
// 'lines' has an option  to cancel the display of blanks and empty lines
//

// lines test requires interaction here

s=['hello' ; 'world']
lines('n')
s=['hello' ; 'world']
lines('r')
s=['hello' ; 'world']
