// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
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

function [s]=%lss_m_p(s1,d2)
    //   [s]=%lss_m_p(s1,d2)
    //   s=s1*d2
    //
    //   s1 : state-space
    //   d2 : polynomial matrix
    //!
    s=s1*tlist(["lss","A","B","C","D","X0","dt"],[],[],[],d2,[],[])
endfunction
