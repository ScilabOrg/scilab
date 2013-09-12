// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [flist,compiled,profilable,called]=listfunctions(scope)
    if ~exists("scope","local") then scope="local"; end
    nam=who(scope)'
    called=uint32(zeros(nam)); afun=(called==1); pfun=afun; cfun=pfun;
    for i=1:size(nam,2)
        clear rvar lst;
        // rvar is cleared to avoid function redefinition warning
        // lst (topmost, variable size) is cleared to speed up garbage collection
        if scope=="global" then global(nam(i)); end
        execstr("rvar="+nam(i));
        if type(rvar)==11 then afun(i)=%t; end
        if type(rvar)==13 then
            afun(i)=%t; cfun(i)=%t;
            lst=macr2lst(rvar)
            pfun(i)=and(lst(5)(1)=="25")
            if pfun(i) then execstr("called(i)="+lst(5)(2)); end
        end
    end
    flist=nam(afun)
    compiled=cfun(afun)
    profilable=pfun(afun)
    called=called(afun)
endfunction
