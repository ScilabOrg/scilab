// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Michael Baudin
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// [xopt,fopt,iter]=karmarkar(Aeq,beq,c,x0)
// [xopt,fopt,iter]=karmarkar(Aeq,beq,c,x0,rtol)
// [xopt,fopt,iter]=karmarkar(Aeq,beq,c,x0,rtol,sf)
// [xopt,fopt,iter]=karmarkar(Aeq,beq,c,x0,rtol,sf,maxiter)
// [xopt,fopt,iter]=karmarkar(Aeq,beq,c,x0,rtol,sf,maxiter,outfun)

function [xopt,fopt,iter]=karmarkar(varargin)

    function argin = argindefault ( rhs , vararglist , ivar , default )
        // Returns the value of the input argument #ivar.
        // If this argument was not provided, or was equal to the 
        // empty matrix, returns the default value.
        if ( rhs < ivar ) then
            argin = default
        else
            if ( vararglist(ivar) <> [] ) then
                argin = vararglist(ivar)
            else
                argin = default
            end
        end
    endfunction

    //
    [lhs,rhs]=argn(0)
    if ( rhs<4 | rhs>8 ) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"karmarkar",4,8));
    end
    //
    Aeq = varargin(1)
    beq = varargin(2)
    c = varargin(3)
    x0 = varargin(4)
    rtol = argindefault ( rhs , varargin , 5 , 1.d-5 )
    sf = argindefault ( rhs , varargin , 6 , 1/4 )
    maxiter = argindefault ( rhs , varargin , 7 , 200 )
    __karmarkar_outfun__ = argindefault ( rhs , varargin , 8 , [] )
    //
    // Check input arguments
    //
    //
    // Check type
    //
    if ( typeof(Aeq) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",1));
    end
    if ( typeof(beq) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",2));
    end
    if ( typeof(c) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",3));
    end
    if ( typeof(x0) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",4));
    end
    if ( typeof(rtol) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",5));
    end
    if ( typeof(sf) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",6));
    end
    if ( typeof(maxiter) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",7));
    end
    if ( __karmarkar_outfun__ <> [] ) then
        if ( and(typeof(__karmarkar_outfun__) <> ["function" "list" "fptr"] ) ) then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"),"karmarkar",8,"function or list"));
        end
        if ( typeof(__karmarkar_outfun__)=="list" ) then
            if ( and(typeof(__karmarkar_outfun__(1))<>["function" "fptr"] ) ) then
                error(msprintf(gettext("%s: Wrong type for input argument #%d: %s expected.\n"),"karmarkar",8,"function"));
            end
        end
    end
    //
    // Check size
    //
    [n,p]=size(Aeq)
    if ( or ( size(beq) <> [n 1] ) ) then 
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",2));
    end
    if ( or ( size(c) <> [p 1] ) ) then 
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",3));
    end
    if ( or ( size(x0) <> [p 1] ) ) then 
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",4));
    end
    if ( or ( size(rtol) <> [1 1] ) ) then 
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",5));
    end
    if ( or ( size(sf) <> [1 1] ) ) then 
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",6));
    end
    if ( or ( size(maxiter) <> [1 1] ) ) then 
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",7));
    end
    //
    // Check x0
    //
    if ( min(x0)<0 ) then 
        error(msprintf(gettext("%s: x0 is not positive."),"karmarkar"));
    end
    if ( norm(Aeq*x0-beq)>rtol ) then 
        error(msprintf(gettext("%s: x0 does not satisfy the equality constraints."),"karmarkar"));
    end
    //
    xopt=x0
    tc=c'
    fopt=tc*xopt
    norstep = %inf
    iter=0
    step = zeros(p,1)
    stop = %f
    if ( __karmarkar_outfun__ <> [] ) then
        cbktype = typeof(__karmarkar_outfun__)
        if ( cbktype == "list" ) then
            // List or tlist
            __karmarkar_outfun__f = __karmarkar_outfun__(1);
        end
    end

    while ( norstep>rtol*norm(xopt) & iter<maxiter )
        if ( __karmarkar_outfun__ <> [] ) then
            if ( cbktype == "list" ) then
                stop = __karmarkar_outfun__f ( iter , xopt , fopt , step , __karmarkar_outfun__(2:$));
            elseif ( or(cbktype == ["function" "fptr"] ) ) then
                stop = __karmarkar_outfun__ ( iter , xopt , fopt , step )
            end
        else
            stop = %f
        end
        if ( stop ) then
            break
        end
        iter=iter+1
        //    ax=Aeq*diag(xopt)
        ax=Aeq.*(ones(n,1)*xopt')
        xc=xopt.*c
        y=ax'\xc
        //    y=(ax*ax')\(ax*xc)
        d=-xc+ax'*y
        dk=xopt.*d
        if ( min(dk)>0 ) then 
            error(msprintf(gettext("%s: Unbounded problem!"),"karmarkar"))
        end
        if ( min(d)==0 ) then
          alpha = -1
        else
          alpha = -sf / min(d)
        end
        step = alpha*dk
        norstep = norm(step)
        xopt=xopt+step
        fopt=tc*xopt
    end
    if ( __karmarkar_outfun__ <> [] ) then
        if ( cbktype == "list" ) then
            stop = __karmarkar_outfun__f ( iter , xopt , fopt , step , __karmarkar_outfun__(2:$));
        elseif ( or(cbktype == ["function" "fptr"] ) ) then
            stop = __karmarkar_outfun__ ( iter , xopt , fopt , step )
        end
    end
endfunction

