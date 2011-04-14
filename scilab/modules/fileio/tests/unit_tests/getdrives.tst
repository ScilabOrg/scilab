// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->

// ============================================================================
// Unitary tests for getdrives function
// ============================================================================

ierr = execstr("getdrives(0)", "errcatch");
if ierr <> 77 then pause,end

if getos() == "Windows" then
    //
    // Windows part
    //
    drives = getdrives();
    // At least there should be A:\
    if ~or(drives == "A:\") then pause,end
else
    //
    // Linux and Mac
    //
    if getdrives() <> "/" then pause,end
end