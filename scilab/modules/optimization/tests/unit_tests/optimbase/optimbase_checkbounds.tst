// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

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

