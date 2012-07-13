// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// unit tests str2code
//===============================
if or(_str2code('abcdefghijklmnopqrstuvwxyz')<>(10:35)') then pause,end
if or(_str2code('ABCDEFGHIJKLMNOPQRSTUVWXYZ')<>-(10:35)') then pause,end
if or(_str2code('0123456789')<>(0:9)') then pause,end
if _str2code(emptystr())<>[] then pause,end
//===============================
if _str2code('Scilab') <> [-28;12;18;21;10;11] then pause,end
//===============================
