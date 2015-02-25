// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Pass tlist as rhs / lhs, let scilab reuse the allocated space
//==============================================================================

// <-- BENCH NB RUN : 10 -->

stacksize("max");

data = tlist(["mydata" "foo" "bar" "zero"], [], [], []);

data.foo = zeros(4096,4096);
data.bar = zeros(4096,4096);
data.zero = zeros(4096,4096);

function data=computation(data)
    data.zero = data.foo + data.bar
endfunction

// <-- BENCH START -->
data = computation(data);
// <-- BENCH END -->

