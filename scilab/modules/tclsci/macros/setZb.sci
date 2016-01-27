// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===


//Fec
function setZb(min, max)
    global ged_handle; h=ged_handle
    tmp=h.z_bounds;
    tmp(1,1)=min;
    tmp(1,2)=max;
    tst=execstr("h.z_bounds=tmp","errcatch","n");
    if tst<>0 then
        disp "Warning: X data_bounds must contain double"
    end
endfunction
