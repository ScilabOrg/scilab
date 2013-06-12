// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/IVPD.zcos"));
execstr(scs_m.props.context);
//Set initial point for steady state search
X0=[0;0;0;0]; //Initial values for states
U0=0;            //Initial values for inputs
Y0=[0;0];       //Initial values for outputs

//Set free variables
IndX=1:4;      //All states are free
IndU=1;        //all inputs are free
IndY=2;        //second output (Theta) is free, the first one (x) is fixed
IndXd=[];      //all derivatives must be 0

//steady state search
[X,U,Y,Xd]=steadycos(scs_m,X0,U0,Y0,IndX,IndU,IndY,IndXd,list(1e-10,0));

//Check results
assert_checkalmostequal(-alpha,X(4));
assert_checkalmostequal(Y,[0;-alpha]);
assert_checkalmostequal(U,(M+m)*g*sin(alpha));

//linearization
sys=lincos(scs_m,X,U,list(1e-10,0));

//Check results
assert_checkalmostequal(sys.A,[0,0,0,-4.83048258484346427;
                               1,0,0,0;
                               0,0,0,18.2088722913447540;
                               0,0,1,0]);  
 
