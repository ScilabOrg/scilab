// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function rtol = assert_cond2reltol ( varargin )
    // Suggests a relative error, computed from the condition number.
    //
    // Calling Sequence
    //   rtol = assert_cond2reltol ( condition )
    //   rtol = assert_cond2reltol ( condition , offset )
    //
    // Parameters
    //   condition : a matrix of doubles, the condition number. The condition number must be strictly positive.
    //   offset : a matrix of doubles, a shift for the number of required decimal digits (default offset=0). For example, offset=1 increases the accuracy requirement (decreases the relative tolerance by a factor 10^-1), offset=-1 decreases the accuracy requirement (increases the relative tolerance by a factor 10^1).
    //   rtol : a matrix of doubles, the relative tolerance. The relative tolerance is strictly positive, lower than 1.
    //
    // Description
    //   Depending on the condition number, returns the corresponding relative tolerance.
    //
    //   Any optional parameter equal to the empty matrix [] is set to its default value.
    //
    //   We emphasize that this relative tolerance is only a suggestion.
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
    // We compute the number of required digits d, then the relative tolerance is 10^-d.
    //   
    // Examples
    //   assert_cond2reltol ( 0 ) // 1.110D-16
    //   assert_cond2reltol ( 1 ) // 1.110D-16
    //   assert_cond2reltol ( 1.e1 ) // 1.110D-15
    //   assert_cond2reltol ( 1.e2 ) // 1.110D-14
    //   assert_cond2reltol ( 1.e3 ) // 1.110D-13
    //   assert_cond2reltol ( 1.e16 ) // 1
    //   assert_cond2reltol ( 1.e17 ) // 1
    //   assert_cond2reltol ( 1.e18 ) // 1
    //
    //   // Matrix input.
    //   condition = [0,1,10,100,1000,1.D13,1.D14,1.D15,1.D16,1.D17,1.D18];
    //   expected = [1.110D-16    1.110D-16    1.110D-15    1.110D-14    1.110D-13    0.0011102    0.0111022    0.1110223    1.    1.    1.];
    //   assert_cond2reltol ( condition )
    //
    //   // Using offset
    //   // Negative offset : require less accuracy
    //   assert_cond2reltol ( 1.e2 , [0 -1] ) // [1.1D-14 1.1D-13]
    //   // Positive offset : requires more accuracy
    //   // See that the impact of offset is constrained.
    //   assert_cond2reltol ( 1.e2 , [0 1 2 3] ) // [1.1D-14 1.1D-15 1.1D-16 1.1D-16]
    //   // Negative offset
    //   // See that the impact of offset is constrained.
    //   assert_cond2reltol ( 1.e14 , [0 -1 -2 -3] ) // [1.1D-02    1.1D-01    1 1]
    //
    //   // Plot the relative tolerance depending on the condition
    //   condition = logspace(0,18,1000);
    //   r = assert_cond2reltol ( condition );
    //   plot(condition,r)
    //   h=gcf();
    //   h.children.log_flags="lln";
    //   h.children.children.children.thickness=4;
    //   xt = h.children.x_ticks;
    //   xt.locations = 10^(0:2:18)';
    //   xt.labels = ["10^0";"10^2";"10^4";"10^6";"10^8";"10^10";"10^12";"10^14";"10^16";"10^18"];
    //   h.children.x_ticks=xt;
    //   yt = h.children.y_ticks;
    //   yt.locations = 10^-(0:2:18)';
    //   yt.labels = ["10^0";"10^-2";"10^-4";"10^-6";"10^-8";"10^-10";"10^-12";"10^-14";"10^-16";"10^-18"];
    //   h.children.y_ticks=yt;
    //   xtitle("Relative tolerance","Condition","Relative tolerance");
    //   r = assert_cond2reltol ( condition , +3 );
    //   plot(condition,r,"r")
    //   r = assert_cond2reltol ( condition , -3 );
    //   plot(condition,r,"g")
    //   legend(["Offset=0","Offset=+3","Offset=-3"]);
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
    if ( and ( rhs <> [1 2] ) ) then
        errmsg = sprintf ( gettext ( "%s: Unexpected number of input arguments : %d provided while %d to %d are expected.") , "assert_cond2reltol" , rhs , 1 , 2 )
        error(errmsg)
    end
    //
    // Get arguments
    condition = varargin ( 1 )
    offset = argindefault ( rhs , varargin , 2 , 0 )
    //
    // Check types of variables
    if ( typeof(condition) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.") , "assert_cond2reltol" , 1 , "condition" , typeof(condition) , "constant" )
        error(errmsg)
    end
    if ( typeof(offset) <> "constant" ) then
        errmsg = sprintf ( gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.") , "assert_cond2reltol" , 2 , "offset" , typeof(offset) , "constant" )
        error(errmsg)
    end
    //
    // Check size of variables
    // Let the user and assert_expandvar manage this.
    //
    // Check content of variables
    if ( or ( condition < 0 ) ) then
        errmsg = sprintf ( gettext ( "%s: Unexpected content of input argument #%d : variable %s has negative entries.") , "assert_cond2reltol" , 1 , "condition" )
        error(errmsg)
    end
    //
    [condition,offset] = assert_expandvar (condition,offset)
    //
    d = assert_cond2reqdigits ( condition , offset , 10 )
    rtol = 10^-d
endfunction

