// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ? - 2008 - Rainer von Seggern
// Copyright (C) ? - 2008 - Bruno Pincon
// Copyright (C) 2009 - INRIA - Michael Baudin
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [J,H] = numderivative(varargin)
    //
    // Check input arguments
    [lhs,rhs]=argn();
    if rhs<2 | rhs>6 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"numderivative",2,6));
    end
    if lhs<1 | lhs>2 then
        error(msprintf(gettext("%s: Wrong number of output arguments: %d to %d expected.\n"),"numderivative",1,2));
    end
    //
    // Get input arguments
    __numderivative_f__ = varargin(1)
    x = varargin(2)
    [n,p] = size(x)
    h = apifun_argindefault ( varargin , 3 , [] )
    order = apifun_argindefault ( varargin , 4 , 2 )
    H_form = apifun_argindefault ( varargin , 5 , "default" )
    Q = apifun_argindefault ( varargin , 6 , eye(n,n) )
    //
    // Check types
    if and(type(__numderivative_f__) <> [11 13 15] ) then
        // Must be a function (uncompiled or compiled) or a list
        error(msprintf(gettext("%s: Wrong type for argument %d: Function or list expected.\n"),"numderivative",1));
    end
    if type(__numderivative_f__)==15 then
        // List case
        // Check that the first element in the list is a function
        if and(type(__numderivative_f__(1)) <> [11 13] ) then
            error(msprintf(gettext("%s: Wrong type for argument %d: Function expected in first element of list.\n"),"numderivative",1));
        end
    end
    if type(x) ~= 1 then
        error(msprintf(gettext("%s: Wrong type for argument %d: Matrix expected.\n"),"numderivative",2));
    end
    if type(h) ~= 1 then
        error(msprintf(gettext("%s: Wrong type for argument %d: Matrix expected.\n"),"numderivative",3));
    end
    if type(order) ~= 1 then
        error(msprintf(gettext("%s: Wrong type for argument %d: Matrix expected.\n"),"numderivative",4));
    end
    if type(H_form) ~= 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"numderivative",5));
    end
    if type(Q) ~= 1 then
        error(msprintf(gettext("%s: Wrong type for argument %d: Matrix expected.\n"),"numderivative",6));
    end
    //
    // Check sizes
    if type(__numderivative_f__)==15 then
        // List case
        if ( length(__numderivative_f__) < 2 ) then
            error(msprintf(gettext("%s: Wrong number of elements in input argument #%d: At least %d elements expected, but current number is %d.\n"),"numderivative",1,2,length(__numderivative_f__)));
        end
    end
    if p ~= 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A column vector expected.\n"),"numderivative",2));
    end
    if ( h<> [] ) then
        if (size(h) <> [1 1]) & (size(h) <> [n 1]) then
            error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"numderivative",3,1));
        end
    end
    if size(order) <> [1 1] then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"numderivative",4,1,1));
    end
    if size(H_form) <> [1 1] then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"numderivative",5,1,1));
    end
    if size(Q) <> [n n] then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"numderivative",6,n,n));
    end
    //
    // Check value
    if ( h<> [] ) then
        if or(h<0) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be > %d.\n"),"numderivative",3,0));
        end
    end
    if and(order <> [1 2 4] ) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set  {%s}.\n"),"numderivative",4,"1, 2, 4"));
    end
    if and(H_form <> ["default" "blockmat" "hypermat"] ) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set  {%s}.\n"),"numderivative",5,"default, blockmat, hypermat"));
    end
    if norm(clean(Q*Q'-eye(n,n)))>0 then
        error(msprintf(gettext("%s: Q must be orthogonal.\n"),"numderivative"));
    end
    //
    // Proceed...
    if h == [] then
        h_not_given = %t
    else
        h_not_given = %f
    end
    //
    // Compute Jacobian
    if ( h_not_given ) then
        h = numderivative_step(x,order,1)
    end
    J = numderivative_deriv1(__numderivative_f__, x, h, order, Q)
    m = size(J,1);
    //
    // Quick return if possible
    if lhs == 1 then
        return
    end
    //
    // Compute Hessian matrix
    if ( h_not_given ) then
        h = numderivative_step(x,order,2)
    end
    funForHList = list(numderivative_funForH, __numderivative_f__, h, order, Q)
    H = numderivative_deriv1(funForHList, x, h, order, Q)
    //
    // At this point, H is a m-by-(n^2) block matrix.
    // Update the format of the Hessian
    if H_form == "default" then
        // H has the old scilab form
        H = matrix(H',n*n,m)'
    end
    if H_form == "hypermat" then
        if m>1 then
            // H is a hypermatrix if m>1
            H=H'; 
            H=hypermat([n n m],H(:)); 
        end 
    end
endfunction

//
// numderivative_step --
//   Returns the step for given x, given order and given derivative:
//   d = 1 is for Jacobian
//   d = 2 is for Hessian
function h = numderivative_step(x,order,d)
    n = size(x,"*")
    select d  
    case 1
        select order  
        case 1
            hdefault = sqrt(%eps)
        case 2
            hdefault = %eps^(1/3)
        case 4
            hdefault = %eps^(1/5)
        else
            lclmsg = gettext("%s: Unknown value %s for option %s.\n")
            error(msprintf(lclmsg,"numderivative_step",string(d),"d"));
        end
    case 2
        select order  
        case 1
            hdefault = %eps^(1/3)
        case 2
            hdefault = %eps^(1/4)
        case 4
            hdefault = %eps^(1/6)
        else
            lclmsg = gettext("%s: Unknown value %s for option %s.\n")
            error(msprintf(lclmsg,"numderivative_step",string(d),"d"));
        end
    else
        lclmsg = gettext("%s: Unknown value %s for option %s.\n")
        error(msprintf(lclmsg,"numderivative_step",string(order),"order"));
    end
    // Convert this scalar into a vector, with same size as x
    // For zero entries in x, use the default. 
    // For nonzero entries in x, scales by abs(x).
    h = hdefault * abs(x)
    h(x==0) = hdefault
endfunction


//
// numderivative_funForH --
//   Returns the numerical derivative of __numderivative_f__.
//   This function is called to compute the numerical Hessian.
function J = numderivative_funForH(x, __numderivative_f__, h, order, Q)
    // Transpose !
    J = numderivative_deriv1(__numderivative_f__, x, h, order, Q)';      
    J = J(:);
endfunction 

// numderivative_deriv1 --
//   Computes the numerical gradient of __numderivative_f__, using the given step h.
function g=numderivative_deriv1(__numderivative_f__, x, h, order, Q)
    n=size(x,'*') 
    Dy=[];
    select order
    case 1
        D = Q * diag(h);
        y = numderivative_evalf(__numderivative_f__,x);
        for i=1:n
            d = D(:,i)
            yplus = numderivative_evalf(__numderivative_f__,x+d)
            Dyi = (yplus-y)/h(i)
            Dy=[Dy Dyi]
        end
        g=Dy*Q'
    case 2
        D = Q * diag(h);
        for i=1:n
            d = D(:,i)
            yplus = numderivative_evalf(__numderivative_f__,x+d)
            yminus = numderivative_evalf(__numderivative_f__,x-d)
            Dyi = (yplus-yminus)/(2*h(i))
            Dy=[Dy Dyi]
        end
        g=Dy*Q'
    case 4
        D = Q * diag(h);
        for i=1:n
            d = D(:,i)
            yplus = numderivative_evalf(__numderivative_f__,x+d)
            yminus = numderivative_evalf(__numderivative_f__,x-d)
            yplus2 = numderivative_evalf(__numderivative_f__,x+2*d)
            yminus2 = numderivative_evalf(__numderivative_f__,x-2*d)
            dFh =  (yplus-yminus)/(2*h(i))
            dF2h = (yplus2-yminus2)/(4*h(i))
            Dyi = (4*dFh - dF2h)/3
            Dy=[Dy Dyi]
        end
        g = Dy*Q'
    end
endfunction

// numderivative_evalf --
// Computes the value of __numderivative_f__ at the point x.
// The argument __numderivative_f__ can be a function (macro or linked code) or a list.
function y=numderivative_evalf(__numderivative_f__,x) 
    if type(__numderivative_f__)==15 then  
        // List case
        __numderivative_fun__=__numderivative_f__(1); 
        y=__numderivative_fun__(x,__numderivative_f__(2:$)); 
    elseif or(type(__numderivative_f__)==[11 13]) then  
        // Function case
        y=__numderivative_f__(x);
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A function expected.\n"),"numderivative",1));
    end
endfunction

function argin = apifun_argindefault ( vararglist , ivar , default )
    // Returns the value of an input argument.


    [lhsnb,rhsnb]=argn()
    if ( rhsnb <> 3 ) then
        msgfmt = "%s: Wrong number of input argument: %d expected.\n"
        errmsg = msprintf(gettext(msgfmt), "apifun_argindefault", 3)
        error(errmsg)
    end
    if ( lhsnb <> 1 ) then
        msgfmt = "%s: Wrong number of output  argument: %d expected.\n"
        errmsg = msprintf(gettext(msgfmt), "apifun_argindefault", 1)
        error(errmsg)
    end
    //
    // Checking type of input arguments
    if ( typeof(vararglist) <> "list" ) then
        errmsg = msprintf(gettext("%s: Wrong type for argument %d: List expected.\n"), "apifun_argindefault", 2 );
        error(errmsg)
    end
    if ( typeof(ivar) <> "constant" ) then
        errmsg = msprintf(gettext("%s: Wrong type for argument %d: Real matrix expected.\n"), "apifun_argindefault", "ivar" , 3 );
        error(errmsg)
    end
    //
    // Checking size of input arguments
    if ( or(size(ivar) <> [1 1]) ) then
        strcomp = strcat(string(size(ivar))," ")
        errmsg = msprintf(gettext("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), "apifun_argindefault", 3, 1, 1 );
        error(errmsg)
    end
    //
    // Returns the value of the input argument #ivar.
    // If this argument was not provided, or was equal to the 
    // empty matrix, returns the default value.
    rhs = length(vararglist)
    if ( rhs < ivar ) then
        argin = default
    else
        if ( typeof(vararglist(ivar))== "constant" ) then
            if ( vararglist(ivar) <> [] ) then
                argin = vararglist(ivar)
            else
                argin = default
            end
        else
            argin = vararglist(ivar)
        end

    end
endfunction



