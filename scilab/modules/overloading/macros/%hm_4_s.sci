// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
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
function M=%hm_4_s(M,s)

    //M>=s
    if size(s,"*")<> 1 then
        error(msprintf(_("%s: Wrong type for input argument #%d: A scalar expected.\n"),"%hm_4_s",2));
    end
    M("entries")=M("entries")>=s
endfunction
