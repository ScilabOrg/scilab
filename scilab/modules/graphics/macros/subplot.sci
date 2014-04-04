// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function subplot(m,n,p)
    [lhs,rhs]=argn(0)

    if rhs<>3 & rhs<>1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected."), "subplot", 1, 3));
        return
    end

    if rhs==1 then
        p=modulo(m,10)
        n=modulo((m-p)/10,10)
        m=(m-p-10*n)/100
    end

    if m*n*p==0 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: A %d digits integer with digits greater than %d expected.\n"),"subplot",1,3,0));
        return
    end

    j=int((p-1)/n)
    i=p-1-n*j
    axes_bounds=[i/n,j/m,1/n,1/m];

    a = gca();
    f = a.parent;
    na=size(f.children,"*")
    if na==1 then
        // an axes is automatically created when a figure is created
        // do not create a new axes if we have just this one
        a=f.children;da=gda()
        if a.children==[]& and(a.axes_bounds==da.axes_bounds) then
            //a single axes with no children, just resize it
            a.axes_bounds=axes_bounds;
            return
        end
    end
    // look for an axes with the same axes_bounds
    for k=1:na
        child = f.children(k);
        if child.type == "Axes" & and(child.axes_bounds == axes_bounds) then
            //make it current
            sca(child);
            return;
        end
    end
    //create a new axes
    a=newaxes(f);
    a.axes_bounds=axes_bounds;
    sca(a);
endfunction
