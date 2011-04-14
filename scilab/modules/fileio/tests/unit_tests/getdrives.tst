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

if getos() == "Linux"
  if getdrives() <> "/" then pause,end
end
