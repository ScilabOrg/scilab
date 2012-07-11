// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

gstacksize('min');
mingstksize = gstacksize();
if mingstksize(1) <> 11000 then pause,end

gstacksize('max');
maxgstksize = gstacksize();

gstacksize(ceil((maxgstksize(1) + mingstksize(1))/2));
curgstksize = gstacksize();
if curgstksize(1) <> ceil((maxgstksize(1) + mingstksize(1))/2) then pause,end

gstacksize(mingstksize(1));
newmingstksize = gstacksize();
if newmingstksize(1) <> mingstksize(1) then pause,end
