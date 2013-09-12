// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- CLI SHELL MODE -->




//
// Test optimbase_checkbounds method
//
//
opt = optimbase_new ();
opt = optimbase_configure ( opt , "-numberofvariables",2);
opt = optimbase_configure ( opt , "-verbose",1);
// The bounds are consistent
opt = optimbase_configure ( opt , "-boundsmin" , [-5.0 -5.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [5.0 5.0] );
opt = optimbase_checkbounds ( opt );
// The min bound does not have a consistent size
opt = optimbase_configure ( opt , "-boundsmin" , [-5.0 -5.0 10.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [5.0 5.0] );
instr = "opt = optimbase_checkbounds ( opt );";
lclmsg = gettext("%s: The number of variables %d does not match the number of min bounds: %d.\n");
assert_checkerror(instr,lclmsg,[],"optimbase_checkbounds",2,3);
// The max bound does not have a consistent size
opt = optimbase_configure ( opt , "-boundsmin" , [-5.0 -5.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [5.0 5.0 10.0] );
instr = "opt = optimbase_checkbounds ( opt );";
lclmsg = gettext("%s: The number of variables %d does not match the number of max bounds: %d.\n");
assert_checkerror(instr,lclmsg,[],"optimbase_checkbounds",2,3);
// The bounds are not consistent
opt = optimbase_configure ( opt , "-boundsmin" , [5.0 5.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [-5.0 -5.0] );
instr = "opt = optimbase_checkbounds ( opt );";
lclmsg = gettext("%s: The max bound %s for variable #%d is lower than the min bound %s.\n");
assert_checkerror(instr,lclmsg,[],"optimbase_checkbounds","-5",1,"5");
opt = optimbase_destroy(opt);

