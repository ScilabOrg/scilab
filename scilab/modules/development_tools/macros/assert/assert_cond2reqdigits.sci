// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function d = assert_cond2reqdigits ( varargin )
    // Suggests the number of required digits, given the condition number.
    //
    // Calling Sequence
    //   d = assert_cond2reqdigits ( condition )
    //   d = assert_cond2reqdigits ( condition , offset )
    //   d = assert_cond2reqdigits ( condition , offset , b )
    //
    // Parameters
    //   condition : a matrix of doubles, the condition number. The condition number must be strictly positive.
    //   offset : a matrix of doubles, a shift for the number of required base-b digits (default offset=0). For example, offset=-1 produces a smaller number of required digits (reduces the required accuracy), offset=1 produces a larger number of required digits (increases the required accuracy).
    //   b : a matrix of floating point integers, the b (default b = 10).
    //   d : a matrix of doubles, the number of required digits. This is a positive real, between 0 and 15.95, if b=10 or between 0 and 53, if b=2.
    //
    // Description
    //   Depending on the condition number, returns the corresponding number of required decimal digits.
    //
    //   Any optional parameter equal to the empty matrix [] is set to its default value.
    //
    //   We emphasize that this number of required digits is only a suggestion.
    //   Indeed, there may be correct reasons of using a lower or a higher relative tolerance.
    //   
    // <itemizedlist>
    //   <listitem>
    //     Consider the case where an excellent algorithm is able to make accurate computations, 
    //     even for an ill-conditionned problem.
    //     In this case, we may require more accuracy (positive offset).
    //   </listitem>
    //   <listitem>
    //     Consider the case where there is a trade-off between performance and accuracy, where performance wins.
    //     In this case, we may require less accuracy (negative offset).
    //   </listitem>
    // </itemizedlist>
    //
    //   Any scalar input argument is expanded to a matrix of doubles of the same size as the other input arguments.
    //
    // The algorithm is the following. 
    // We compute the base-10 logarithm of condition, then subtract the offset.
    // This number represents the expected number of lost digits.
    // We project it into the interval [0,dmax], where dmax -log10(2^(-53)) is the maximum 
    // achievable number of accurate digits for doubles.
    // We compute the number of required digits d, by difference between dmax and the number 
    // of lost digits.
    // Then the relative tolerance is 10^-d.
    //   
    // Examples
    //    assert_cond2reqdigits ( 0 ) // 15.95459
    //    assert_cond2reqdigits ( 1 ) // 15.95459
    //    assert_cond2reqdigits ( 1.e1 ) // 14.95459
    //    assert_cond2reqdigits ( 1.e2 ) // 13.95459
    //    assert_cond2reqdigits ( 1.e3 ) // 12.95459
    //    assert_cond2reqdigits ( 1.e16 ) // 0
    //    assert_cond2reqdigits ( 1.e17 ) // 0
    //    assert_cond2reqdigits ( 1.e18 ) // 0
    //
    //   // Matrix input.
    //   condition = [0,1,10,100,1000,1.D13,1.D14,1.D15,1.D16,1.D17,1.D18];
    //    assert_cond2reqdigits ( condition )
    //
    //   // Using offset
    //   // Netative offset : decrease number of required digits (requires less accuracy)
    //   assert_cond2reqdigits ( 1.e2 , [0 -1] ) // [13.95459    12.95459]
    //   // Positive offset : increase number of required digits (requires more accuracy)
    //   // See that the impact of offset is constrained.
    //   assert_cond2reqdigits ( 1.e2 , [0 1 2 3] ) // [13.95459    14.95459    15.95459    15.95459]
    //   // Netative offset (requires less accuracy)
    //   // See that the impact of offset is constrained.
    //   assert_cond2reqdigits ( 1.e14 , [0 -1 -2 -3] ) // [1.9545898    0.9545898    0.    0.]
    //
    // // Using base-2
    //    assert_cond2reqdigits ( 0     , [] , 2 ) // 53
    //    assert_cond2reqdigits ( 1     , [] , 2 ) // 53
    //    assert_cond2reqdigits ( 1.e1  , [] , 2 ) // 49.678072
    //    assert_cond2reqdigits ( 1.e2  , [] , 2 ) // 46.356144
    //    assert_cond2reqdigits ( 1.e3  , [] , 2 ) // 43.034216
    //    assert_cond2reqdigits ( 1.e16 , [] , 2 ) // 0
    //    assert_cond2reqdigits ( 1.e17 , [] , 2 ) // 0
    //    assert_cond2reqdigits ( 1.e18 , [] , 2 ) // 0
    //
    //   // Plot the number of required decimal digits depending on the condition
    //   condition = logspace(0,18,1000);
    //   d = assert_cond2reqdigits ( condition );
    //   plot(condition,d)
    //   h=gcf();
    //   h.children.log_flags="lnn";
    //   h.children.children.children.thickness=4;
    //   xt = h.children.x_ticks;
    //   xt.locations = 10^(0:2:18)';
    //   xt.labels = ["10^0";"10^2";"10^4";"10^6";"10^8";"10^10";"10^12";"10^14";"10^16";"10^18"];
    //   h.children.x_ticks=xt;
    //   xtitle("Number of required digits","Condition","Required decimal digits");
    //
    //   // Plot the number of required binary digits depending on the condition
    //   condition = logspace(0,18,1000);
    //   d = assert_cond2reqdigits ( condition , [] , 2 );
    //   plot(condition,d)
    //   h=gcf();
    //   h.children.log_flags="lnn";
    //   h.children.children.children.thickness=4;
    //   xt = h.children.x_ticks;
    //   xt.locations = 10^(0:2:18)';
    //   xt.labels = ["10^0";"10^2";"10^4";"10^6";"10^8";"10^10";"10^12";"10^14";"10^16";"10^18"];
    //   h.children.x_ticks=xt;
    //   xtitle("Number of required digits","Condition","Required binary digits");
    //   d = assert_cond2reqdigits ( condition , +10 , 2 );
    //   plot(condition,d,"r")
    //   d = assert_cond2reqdigits ( condition , -10 , 2 );
    //   plot(condition,d,"g")
    //   legend(["Offset=0","Offset=+10","Offset=-10"]);
    //
    // Authors
    //   Michael Baudin, Digiteo, 2010
    //


    function varargout = assert_expandvar ( varargin )
        // Expand variables so that they all have the same shape.
        //
        // Calling Sequence
        //   ovar1 = assert_expandvar ( ivar1 )
        //   [ ovar1 , ovar2 ] = assert_expandvar ( ivar1 , ivar2 )
        //   [ ovar1 , ovar2 , ovar3 ] = assert_expandvar ( ivar1 , ivar2 , ivar3 )
        //   [ ovar1 , ovar2 , ovar3 , ovar4 ] = assert_expandvar ( ivar1 , ivar2 , ivar3 , ivar4 )
        //
        // Parameters
        //   ivar1 : input variable #1
        //   ovar1 : output variable #1
        //
        // Description
        //   This function allows to expand input arguments of 
        //   computationnal functions. 
        //   If scalars are input, expand it to the size of other other variables.
        //   
        //   If matrices are input, all must have the same shape: if not, an error is generated.
        //
        // Examples
        //   // Expand ovar1 to [1 1 1]
        //   [ ovar1 , ovar2 ] = assert_expandvar ( 1 , [2 3 4] )
        //
        //   // Expand ovar2 to [4 4 4]
        //   [ ovar1 , ovar2 ] = assert_expandvar ( [1 2 3] , 4 )
        //
        //   // Expand ovar2 to [4 4 4]'
        //   [ ovar1 , ovar2 ] = assert_expandvar ( [1 2 3]' , 4 )
        //
        // Authors
        //   Michael Baudin - 2009-2010 - DIGITEO

        [lhs,rhs]=argn()
        if ( rhs <> lhs ) then
            errmsg = sprintf ( gettext ( "%s: The number of output arguments %d do not match the number of input arguments %d.") , "assert_expandvar" , lhs , rhs )
            error(errmsg)
        end

        //
        // Check if there is one argument which is a matrix.
        // imat is the index of the input argument which is a matrix.
        istherematrix = %f
        imat = 0
        for ivar = 1 : rhs
            if ( size ( varargin(ivar) , "*" ) <> 1 ) then
                istherematrix = %t
                imat = ivar
                break
            end
        end
        // If there is no matrix, returns the output arguments as is.
        if ( ~istherematrix ) then
            for ovar = 1 : lhs
                varargout ( ovar ) = varargin ( ovar )
            end
            return
        end
        // If there is one matrix, get its size.
        nbrows = size ( varargin ( imat ) , "r" )
        nbcols = size ( varargin ( imat ) , "c" )
        // Check that all matrices have the same shape.
        for ivar = 1 : rhs
            nbi = size ( varargin ( ivar ) , "*" )
            if ( nbi <> 1 ) then
                nbrowsi = size ( varargin ( ivar ) , "r" )
                nbcolsi = size ( varargin ( ivar ) , "c" )
                if ( nbrowsi <> nbrows ) then
                    errmsg = msprintf(gettext("%s: Expected %d rows in input argument #%d, but found %d rows instead."), "assert_expandvar" , nbrows , ivar , nbrowsi );
                    error(errmsg)
                end
                if ( nbcolsi <> nbcols ) then
                    errmsg = msprintf(gettext("%s: Expected %d columns in input argument #%d, but found %d columns instead."), "assert_expandvar" , nbcols , ivar , nbcolsi );
                    error(errmsg)
                end
            end
        end
        // Expand all input arguments which are scalar up to this shape.
        for ivar = 1 : rhs
            if ( size ( varargin(ivar) , "*" ) == 1 ) then
                varargin ( ivar ) = varargin ( ivar ) * ones ( nbrows , nbcols )
            end
        end
        // Set all output variables
        for ovar = 1 : lhs
            varargout ( ovar ) = varargin ( ovar )
        end
    endfunction

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


    [lhs,rhs]=argn()
    if ( and ( rhs <> [1 2 3] ) ) then
        errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d to %d expected.") , "assert_cond2reqdigits" , 1 , 3 )
        error(errmsg)
    end
    //
    // Get arguments
    condition = varargin ( 1 )
    offset = argindefault ( rhs , varargin , 2 , 0 )
    b = argindefault ( rhs , varargin , 3 , 10 )
    //
    // Check types of variables
    if ( typeof(condition) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Real matrix expected.") , "assert_cond2reqdigits" , 1 )
        error(errmsg)
    end
    if ( typeof(offset) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Real matrix expected.") , "assert_cond2reqdigits" , 2 )
        error(errmsg)
    end
    if ( typeof(b) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Wrong type for input argument #%d: Real matrix expected.") , "assert_cond2reqdigits" , 3 )
        error(errmsg)
    end
    //
    // Check size of variables
    // Let the user and assert_expandvar manage this.
    //
    // Check content of variables
    if ( or ( condition < 0 ) ) then
        errmsg = sprintf ( gettext ( "%s: Unexpected content of input argument #%d : variable %s has negative entries.") , "assert_cond2reqdigits" , 1 , "condition" )
        error(errmsg)
    end
    if ( or ( b < 2 ) ) then
        errmsg = sprintf ( gettext ( "%s: Unexpected content of input argument #%d : variable %s has entries lower than %d.") , "assert_cond2reqdigits" , 3 , "b" , 2 )
        error(errmsg)
    end
    //
    [condition,offset] = assert_expandvar (condition,offset)
    //
    backIEEE=ieee()
    ieee(2)
    //
    dmax = -log(2^(-53))/log(b)
    lost = min(max(log(condition)/log(b)-offset,0),dmax)
    d = dmax - lost
    //
    ieee(backIEEE)
endfunction

