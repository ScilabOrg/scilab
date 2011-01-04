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
// [xopt,fopt,iter]=karmarkar(Aeq,beq,c,x0,rtolf)
// [xopt,fopt,iter]=karmarkar(Aeq,beq,c,x0,rtolf,sf)
// [xopt,fopt,iter]=karmarkar(Aeq,beq,c,x0,rtolf,sf,maxiter)
// [xopt,fopt,iter]=karmarkar(Aeq,beq,c,x0,rtolf,sf,maxiter,outfun)

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
    rtolf = argindefault ( rhs , varargin , 5 , 1.d-5 )
    gam = argindefault ( rhs , varargin , 6 , 1/4 )
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
    if ( typeof(rtolf) <> "constant" ) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"karmarkar",5));
    end
    if ( typeof(gam) <> "constant" ) then
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
    if ( or ( size(rtolf) <> [1 1] ) ) then 
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",5));
    end
    if ( or ( size(gam) <> [1 1] ) ) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",6));
    end
    if ( or ( size(maxiter) <> [1 1] ) ) then 
        error(msprintf(gettext("%s: Wrong size for input argument #%d."),"karmarkar",7));
    end
    //
    // Check content
    //

    if ( min(x0)<0 ) then 
        error(msprintf(gettext("%s: Wrong value for input argument #%d. %s"),"karmarkar",4,"x0 is not positive."));
    end
    if ( norm(Aeq*x0-beq)>rtolf ) then 
        error(msprintf(gettext("%s: Wrong value for input argument #%d. %s"),"karmarkar",4,"x0 does not satisfy the equality constraints."));
    end
    if ( rtolf < 0 | rtolf > 1 ) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d. %s"),"karmarkar",5,"rtolf must be in [0,1]."));
    end
    if ( gam < 0 | gam > 1 ) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d. %s"),"karmarkar",6,"gam must be in [0,1]."));
    end
    if ( maxiter < 1 ) then 
        error(msprintf(gettext("%s: Wrong value for input argument #%d. %s"),"karmarkar",7,"maxiter must be greater than 1."));
    end
    if ( floor(maxiter) <> maxiter ) then 
        error(msprintf(gettext("%s: Wrong value for input argument #%d. %s"),"karmarkar",7,"maxiter must be a floating point integer."));
    end
    //
    xopt=x0
    tc=c'
    fopt=tc*xopt
    fprev = fopt+1
    iter=0
    s = zeros(p,1)
    stop = %f
    if ( __karmarkar_outfun__ <> [] ) then
        cbktype = typeof(__karmarkar_outfun__)
        if ( cbktype == "list" ) then
            // List or tlist
            __karmarkar_outfun__f = __karmarkar_outfun__(1);
        end
    end

    while ( abs(fprev-fopt)>rtolf*abs(fprev) & iter<maxiter )
        if ( __karmarkar_outfun__ <> [] ) then
            if ( cbktype == "list" ) then
                stop = __karmarkar_outfun__f ( iter , xopt , fopt , s , __karmarkar_outfun__(2:$));
            elseif ( or(cbktype == ["function" "fptr"] ) ) then
                stop = __karmarkar_outfun__ ( iter , xopt , fopt , s )
            end
        else
            stop = %f
        end
        if ( stop ) then
            break
        end
        iter=iter+1
        // B=Aeq*diag(xopt)
        B=Aeq.*(ones(n,1)*xopt')
        v=xopt.*c
        // y=(B*B')\(B*v) which simplifies into:
        y=B'\v
        p=-v+B'*y
        if ( min(p)==0 ) then
          break
        end
        d=xopt.*p
        if ( min(d)>0 ) then 
            error(msprintf(gettext("%s: Unbounded problem!"),"karmarkar"))
        end
        alpha = -gam / min(p)
        s = alpha*d
        xopt=xopt+s
        fprev = fopt
        fopt=tc*xopt
    end
    if ( __karmarkar_outfun__ <> [] ) then
        if ( cbktype == "list" ) then
            stop = __karmarkar_outfun__f ( iter , xopt , fopt , s , __karmarkar_outfun__(2:$));
        elseif ( or(cbktype == ["function" "fptr"] ) ) then
            stop = __karmarkar_outfun__ ( iter , xopt , fopt , s )
        end
    end
endfunction

