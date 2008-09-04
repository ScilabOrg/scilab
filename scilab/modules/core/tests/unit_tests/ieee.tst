// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//ieee

warning("off");

ieee(2);
if execstr("1/0","errcatch") <> 0 then pause,end
if 1/0<>%inf then pause,end

ieee(1);
if execstr("1/0","errcatch") <> 0 then pause,end
if 1/0<>%inf then pause,end

ieee(0);
if execstr("1/0","errcatch") == 0 then pause,end
