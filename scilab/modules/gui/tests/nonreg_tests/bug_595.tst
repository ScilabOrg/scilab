// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 595 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=595


function m=saisieParBandes(lig,col,titre)
    nl=max(size(lig));
    nc=max(size(col));
    mat=zeros(nl,nc);
    m=x_mdialog(titre,string(lig),string(col),string(mat));
    m=evstr(m);
endfunction

r = saisieParBandes(["100Hz";"125Hz";"160Hz"],["Lp(dB)";"T(s)"],"essai");

if and(r<>0) then pause,end

