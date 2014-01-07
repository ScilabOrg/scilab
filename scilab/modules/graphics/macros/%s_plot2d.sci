// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function []=%s_plot2d(xr,f,style,strf,leg,rect,nax,void)
    // 2D plot of function f: a Scilab function or the name (as a string)
    //  of a dynamically linked function.
    //!

    [lhs, rhs] = argn(0)

    if type(xr) == 10 then // logflag passed first
        logflag = xr;
        xr = f;
        f = style;
        clear style; // In the case %s_plot2d(logflag, xr, f), the variable style is defined
        if exists("strf","local")==1 then style = strf; end
        if exists("leg","local")==1 then strf = leg; end
        if exists("rect","local")==1 then leg = rect; end
        if exists("nax","local")==1 then rect = nax; end
        if exists("void","local")==1 then nax = void; end
    elseif rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "plot2d", 2));
    end
    // Handling optional arguments

    opts=[]

    if exists("style","local")==1 then opts=[opts,"style=style"],end
    if exists("strf","local")==1 then opts=[opts,"strf=strf"],end
    if exists("leg","local")==1 then opts=[opts,"leg=leg"],end
    if exists("rect","local")==1 then opts=[opts,"rect=rect"],end
    if exists("nax","local")==1 then opts=[opts,"nax=nax"],end
    if exists("logflag","local")==1 then opts=[opts,"logflag=logflag"],end
    if exists("frameflag","local")==1 then opts=[opts,"frameflag=frameflag"],end
    if exists("axesflag","local")==1 then opts=[opts,"axesflag=axesflag"],end
    if size(opts,2)<rhs-2 then
        error(msprintf(gettext("%s: Wrong value for input argument: ''%s'', ''%s'', ''%s'', ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"),"plot2d","style","strf","leg","rect","nax","logflag","frameflag","axesflag"));
    end
    execstr("plot2d(xr,feval(xr,f),"+strcat(opts,",")+")")

endfunction
