//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

// <-- JVM NOT MANDATORY -->

// <-- ENGLISH IMPOSED -->

x = mlist(["ZDYF4WCD";"v";"dv"],0.5,0.5);
msg = "%s: Function not defined for given argument type(s), "+...
" Check arguments or define function %s for overloading."
lclmsg = gettext(msg);
assert_checkerror ( "overloading_error(""acosh"",x)", lclmsg, [], "overloading_error" , "%ZDYF4WCD_acosh");
