// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->




//
//  Reference:
//
//    An extension of the simplex method to constrained
//    nonlinear optimization
//    M.B. Subrahmanyam
//    Journal of optimization theory and applications
//    Vol. 62, August 1989
//
//    Gould F.J.
//    Nonlinear Tolerance Programming
//    Numerical methods for Nonlinear optimization
//    Edited by F.A. Lootsma, pp 349-366, 1972

//
// optimtestcase --
//   Non linear inequality constraints are positive.
//    
// Arguments
//   x: the point where to compute the function
//   index : the stuff to compute
//
function [ f , c , index ] = optimtestcase ( x , index )
  f = []
  c = []
  if ( ( index == 2 ) | ( index == 6 ) ) then
    f = x(1)^2 + x(2)^2 + 2.0 * x(3)^2 + x(4)^2 ...
      - 5.0 * x(1) - 5.0 * x(2) - 21.0 * x(3) + 7.0 * x(4)
  end
  if ( ( index == 5 ) | ( index == 6 ) ) then
    c1 = - x(1)^2 - x(2)^2 - x(3)^2 - x(4)^2 ...
              - x(1) + x(2) - x(3) + x(4) + 8
    c2 = - x(1)^2 - 2.0 * x(2)^2 - x(3)^2 - 2.0 * x(4)^2 ...
              + x(1) + x(4) + 10.0
    c3 = - 2.0 * x(1)^2 - x(2)^2 - x(3)^2 - 2.0 * x(1) ...
              + x(2) + x(4) + 5.0
    c = [c1 c2 c3]
  end
endfunction
//
// neldermead_constraints --
//   The Nelder-Mead algorithm, with variable-size simplex
//   and modifications for bounds and
//   inequality constraints.
//
function this = neldermead_constraints ( this )
  // Check settings correspond to algo
  [ this.optbase , hascons ] = optimbase_hasconstraints ( this.optbase );
  if ( ~hascons ) then
      errmsg = msprintf(gettext("%s: Problem has no constraints, but variable algorithm is designed for them."), "neldermead_constraints")
      error(errmsg)
  end
  verbose = optimbase_cget ( this.optbase , "-verbose" )
  //
  // Order the vertices for the first time
  //
  simplex = this.simplex0;
  n = optimbase_cget ( this.optbase , "-numberofvariables" );
  fvinitial = optimbase_get ( this.optbase , "-fx0" );
  // Sort function values and x points by increasing function value order
  this = neldermead_log (this,"Step #1 : order");
  simplex = optimsimplex_sort ( simplex );
  currentcenter = optimsimplex_center ( simplex );
  currentxopt = optimbase_cget ( this.optbase , "-x0" );
  newfvmean = optimsimplex_fvmean ( simplex );
  nbve = optimsimplex_getnbve ( simplex );
  ihigh = nbve;
  inext = ihigh - 1
  ilow = 1
  [ this.optbase , hasbounds ] = optimbase_hasbounds ( this.optbase );
  nbnlc = optimbase_cget ( this.optbase , "-nbineqconst" )
  //
  // Initialize
  //
  terminate = %f;
  iter = 0;
  step = "init";
  //
  // Nelder-Mead Loop
  //
  while ( ~terminate )
    this.optbase = optimbase_incriter ( this.optbase );
    iter = iter + 1;
    xlow = optimsimplex_getx ( simplex , ilow )
    flow = optimsimplex_getfv ( simplex , ilow )
    xhigh = optimsimplex_getx ( simplex , ihigh )
    fhigh = optimsimplex_getfv ( simplex , ihigh )
    xn = optimsimplex_getx ( simplex , inext )
    fn = optimsimplex_getfv ( simplex , inext )
    //
    // Store history
    //
    xcoords = optimsimplex_getallx ( simplex )
    this = neldermead_storehistory ( this , n , flow , xlow , xcoords );
    currentfopt = flow;
    previousxopt = currentxopt;
    currentxopt = xlow;
    previouscenter = currentcenter;
    currentcenter = optimsimplex_center ( simplex );
    oldfvmean = newfvmean;
    newfvmean = optimsimplex_fvmean ( simplex );
    if ( verbose == 1 ) then
      deltafv = abs(optimsimplex_deltafvmax ( simplex ));
      totaliter = optimbase_get ( this.optbase , "-iterations" );
      funevals = optimbase_get ( this.optbase , "-funevals" );
      ssize = optimsimplex_size ( simplex )
      this = neldermead_log (this,sprintf("================================================================="));
      this = neldermead_log (this,sprintf("Iteration #%d (total = %d)",iter,totaliter));
      this = neldermead_log (this,sprintf("Function Eval #%d",funevals));
      this = neldermead_log (this,sprintf("Xopt : [%s]",_strvec(xlow)));
      this = neldermead_log (this,sprintf("Fopt : %e",flow));
      this = neldermead_log (this,sprintf("DeltaFv : %e",deltafv));
      this = neldermead_log (this,sprintf("Center : [%s]",_strvec(currentcenter)));
      this = neldermead_log (this,sprintf("Size : %e",ssize));
      str = optimsimplex_tostring ( simplex )
      for i = 1:nbve
        this = neldermead_log (this,str(i));
      end
    end
    neldermead_outputcmd ( this, "iter" , simplex , step )

    //
    // Update termination flag
    //
    if ( iter > 1 ) then
      [ this , terminate , status] = neldermead_termination (this , ...
        fvinitial , oldfvmean , newfvmean , previouscenter , currentcenter , simplex );
      if ( terminate ) then
        if ( verbose == 1 ) then
          this = neldermead_log (this,sprintf("Terminate with status : %s",status));
        end
        break
      end
    end
    //
    // Compute xbar, center of better vertices
    //
    if ( verbose == 1 ) then
      this = neldermead_log (this,sprintf("Reflect"));
    end
    xbar = optimsimplex_xbar ( simplex );
    if ( verbose == 1 ) then
      this = neldermead_log (this,sprintf("xbar=[%s]" , _strvec(xbar)));
    end
    //
    // Reflect the worst point with respect to center
    //
    xr = neldermead_interpolate ( xbar , xhigh , this.rho );
    if ( verbose == 1 ) then
      this = neldermead_log (this,sprintf("xr=[%s]" , _strvec(xr)));
    end
    // Adjust point to satisfy bounds and nonlinear inequality constraints
    if ( hasbounds | nbnlc > 0 ) then
      [ this , status , xr ] = _scaleinboundsandcons ( this , xr , xbar );
      if ( ~status ) then
        status = "impossibleconstr"
        break
      end
    end
    [ this.optbase , fr , cr , index ] = optimbase_function ( this.optbase , xr , 2 );
    if ( verbose == 1 ) then
      this = neldermead_log (this,sprintf("xr=[%s], f(xr)=%f", _strvec(xr) , fr));
    end
    if ( fr >= flow & fr < fn ) then
      if ( verbose == 1 ) then
        this = neldermead_log (this,sprintf("  > Perform reflection"));
      end
      simplex = optimsimplex_setve ( simplex , ihigh , fr , xr )
      step = "reflection";
    elseif ( fr < flow ) then
      // Expand
      if ( verbose == 1 ) then
        this = neldermead_log (this,sprintf("Expand"));
      end
      xe = neldermead_interpolate ( xbar , xhigh , this.rho*this.chi );
      // Adjust point to satisfy bounds and nonlinear inequality constraints
      if ( hasbounds | nbnlc > 0 ) then
        [ this , status , xe ] = _scaleinboundsandcons ( this , xe , xbar );
        if ( ~status ) then
          status = "impossibleconstr"
          break
        end
      end
      [ this.optbase , fe , ce , index ] = optimbase_function ( this.optbase , xe , 2 );
      if ( verbose == 1 ) then
        this = neldermead_log (this,sprintf("xe=[%s], f(xe)=%f", strcat(string(xe)," ") , fe ));
      end
      if (fe < fr) then
        if ( verbose == 1 ) then
          this = neldermead_log (this,sprintf("  > Perform Expansion"));
        end
        simplex = optimsimplex_setve ( simplex , ihigh , fe , xe )
        step = "expansion";
      else
        if ( verbose == 1 ) then
          this = neldermead_log (this,sprintf("  > Perform reflection"));
        end
        simplex = optimsimplex_setve ( simplex , ihigh , fr , xr )
        step = "reflection";
      end
    elseif ( fr >= fn & fr < fhigh ) then
      // Outside contraction
      if ( verbose == 1 ) then
        this = neldermead_log (this,sprintf("Contract - outside"));
      end
      xc = neldermead_interpolate ( xbar , xhigh , this.rho*this.gamma );
      // Adjust point to satisfy bounds and nonlinear inequality constraints
      if ( hasbounds | nbnlc > 0 ) then
        [ this , status , xc ] = _scaleinboundsandcons ( this , xc , xbar );
        if ( ~status ) then
          status = "impossibleconstr"
          break
        end
      end
      [ this.optbase , fc , cc , index ] = optimbase_function ( this.optbase , xc , 2 );
      if ( verbose == 1 ) then
        this = neldermead_log (this,sprintf("xc=[%s], f(xc)=%f", strcat(string(xc)," ") , fc));
      end
      if ( fc <= fr ) then
        if ( verbose == 1 ) then
          this = neldermead_log (this,sprintf("  > Perform Outside Contraction"));
        end
        simplex = optimsimplex_setve ( simplex , ihigh , fc , xc )
        step = "outsidecontraction";
      else
        //  Shrink
        if ( verbose == 1 ) then
          this = neldermead_log (this,sprintf("  > Perform Shrink"));
        end
        [ simplex , this ] = optimsimplex_shrink ( simplex , neldermead_costf , this.sigma , this );
        step = "shrink";
      end
    else
      // ( fr >= fn & fr >= fhigh )  
      // Inside contraction
      if ( verbose == 1 ) then
        this = neldermead_log (this,sprintf("Contract - inside"));
      end
      xc = neldermead_interpolate ( xbar , xhigh , -this.gamma );
      // Adjust point to satisfy bounds and nonlinear inequality constraints
      if ( hasbounds | nbnlc > 0 ) then
        [ this , status , xc ] = _scaleinboundsandcons ( this , xc , xbar );
        if ( ~status ) then
          status = "impossibleconstr"
          break
        end
      end
      [ this.optbase , fc , cc , index ] = optimbase_function ( this.optbase , xc , 2 );
      if ( verbose == 1 ) then
        this = neldermead_log (this,sprintf("xc=[%s], f(xc)=%f", strcat(string(xc)," ") , fc));
      end
      if ( fc < fhigh ) then
        if ( verbose == 1 ) then
          this = neldermead_log (this,sprintf("  > Perform Inside Contraction"));
        end
        simplex = optimsimplex_setve ( simplex , ihigh , fc , xc )
        step = "insidecontraction";
      else
        //  Shrink
        if ( verbose == 1 ) then
          this = neldermead_log (this,sprintf("  > Perform Shrink"));
        end
        [ simplex , this ] = optimsimplex_shrink ( simplex , neldermead_costf , this.sigma , this )
        step = "shrink";
      end
    end
    //
    // Sort simplex
    //
    if ( verbose == 1 ) then
      this = neldermead_log (this,sprintf("Sort"));
    end
    simplex  = optimsimplex_sort ( simplex );
  end
  this.optbase = optimbase_set ( this.optbase , "-xopt" , xlow.' );
  this.optbase = optimbase_set ( this.optbase , "-fopt" , flow );
  this.optbase = optimbase_set ( this.optbase , "-status" , status );
  this.simplexopt = simplex;
endfunction
  //
  // _scaleinboundsandcons --
  //   Given a point to scale and a reference point which satisfies the constraints, 
  //   scale the point towards the reference point until it satisfies all the constraints,
  //   including boun constraints.
  //   Returns isscaled = %T if the procedure has succeded before -boxnbnlloops
  //   Returns isscaled = %F if the procedure has failed after -boxnbnlloops
  //   iterations.
  // Arguments
  //   x : the point to scale
  //   xref : the reference point
  //   isscaled : %T or %F
  //   p : scaled point
  //
function [ this , isscaled , p ] = _scaleinboundsandcons ( this , x , xref )
  p = x
  [ this.optbase , hasbounds ] = optimbase_hasbounds ( this.optbase );
  nbnlc = optimbase_cget ( this.optbase , "-nbineqconst" )
  //
  // 1. No bounds, no nonlinear inequality constraints
  // => no problem
  //
  if ( ( hasbounds == %f ) & ( nbnlc == 0 ) ) then
    isscaled = %T
    return;
  end
  isscaled = %F
  //
  // 2. Scale into bounds
  //
  if ( hasbounds ) then
    [ this.optbase , p ] = optimbase_proj2bnds ( this.optbase ,  p );
    this = neldermead_log (this,sprintf(" > After projection into bounds p = [%s]" , ...
      _strvec(p)));
  end
  //
  // 2. Scale into nonlinear constraints
  // Try the current point and see if the constraints are satisfied.
  // If not, move the point "halfway" to the centroid,
  // which should satisfy the constraints, if
  // the constraints are convex.
  // Perform this loop until the constraints are satisfied.
  // If all loops have been performed without success, the scaling
  // has failed.
  //
  alpha = 1.0
  p0 = p
  while ( alpha > this.guinalphamin )
      [ this.optbase , feasible ] = optimbase_isinnonlincons ( this.optbase , p );
      if ( feasible ) then
        isscaled = %T;
        break;
      end
      alpha = alpha / 2.0
      this = neldermead_log (this,sprintf("Scaling inequality constraint with alpha = %e", ...
        alpha ));
      p = ( 1.0 - alpha ) * xref + alpha * p0;
  end
  this = neldermead_log (this,sprintf(" > After scaling into inequality constraints p = [%s]" , ...
    _strvec(p) ) );
  if ( ~isscaled ) then
    this = neldermead_log (this,sprintf(" > Impossible to scale into constraints." ));
  end
endfunction

//
// Test with my own algorithm,
// the "Mega Super Ultra Modified Simplex Method" !!!
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",200);
nm = neldermead_configure(nm,"-maxfunevals",400);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-3);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-nbineqconst",3);
nm = neldermead_configure(nm,"-method","mine");
nm = neldermead_configure(nm,"-mymethod",neldermead_constraints);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [0.0 1.0 2.0 -1.0]', 1e-3, 1e-3 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , -44.0 , 1e-5 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "tolsize" );
nm = neldermead_destroy(nm);

