//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012-2012 - Scilab Enterprises - Bruno JOFRET
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
//
//
function [wrect, frect, logflag, arect]=xgetech()

    a = gca();
    wrect = a.axes_bounds;
    frect = matrix(a.data_bounds, 1, 4);
    logflag = part(a.log_flags, 1:2);
    arect = a.margins;

endfunction
