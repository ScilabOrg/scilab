//<-- CLI SHELL MODE -->
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

a=amell(10,0.1);
if a > 3.693 then pause,end
if a < 3.692 then pause,end
if or(size(amell([1:10], 0.1)) <> [1 10]) then pause,end
if or(amell([1:10], 1)<> 0) then pause,end
