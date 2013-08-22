// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2010 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for standard deviation
// =============================================================================

if stdev(0) <> 0 then pause,end
if stdev(zeros(3,3)) <> 0 then pause,end
if stdev(zeros(3,3,3)) <> 0 then pause,end
