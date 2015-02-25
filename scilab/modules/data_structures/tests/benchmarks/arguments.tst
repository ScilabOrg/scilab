// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Pass raw values as rhs / lhs
//==============================================================================

// <-- BENCH NB RUN : 10 -->

stacksize("max");

foo = zeros(4096,4096);
bar = zeros(4096,4096);
zero = zeros(4096,4096);

function zero=computation(foo, bar)
    zero = foo + bar
endfunction

// <-- BENCH START -->
zero = computation(foo, bar);
// <-- BENCH END -->

