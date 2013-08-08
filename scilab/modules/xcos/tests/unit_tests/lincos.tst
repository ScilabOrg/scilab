// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Serge Steer - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->


//Very simple system
//==================
importXcosDiagram("SCI/modules/xcos/tests/unit_tests/test_lincos.zcos")
execstr(scs_m.props.context)
//direct call of lincos
sys=lincos(scs_m);
assert_checkalmostequal (sys.A ,A,sqrt(%eps),1e-8);
assert_checkalmostequal (sys.B ,B,sqrt(%eps),1e-8);
assert_checkalmostequal (sys.C ,C,sqrt(%eps),1e-8);
assert_checkalmostequal (sys.D ,D,sqrt(%eps),1e-8);


//steadycos +lincos linearization around u=1
X0=[0;0];U0=1; Y0=[0;0];
IndX=1:$;     //All the states
IndU=[];       //All the inputs
IndY=1:$;       //Only the second output (theta), the position x is imposed
IndXd=[];     //All state derivatives must be zero
[X,U,Y,Xd,cpr]=steadycos(scs_m,X0,U0,Y0,IndX,IndU,IndY,IndXd,list(1e-10,0));
sys=lincos(cpr,X,U,list(1e-5,0));
execstr(scs_m.props.context)
assert_checkalmostequal (sys.A ,A,sqrt(%eps),1e-8);
assert_checkalmostequal (sys.B ,B,sqrt(%eps),1e-8);
assert_checkalmostequal (sys.C ,C,sqrt(%eps),1e-8);
assert_checkalmostequal (sys.D ,D,sqrt(%eps),1e-8);


//Explicit system
//==================
importXcosDiagram("SCI/modules/xcos/tests/unit_tests/IVPD.zcos")
//Search a steady state with pendulum pointing up
X0=[0;0;0;0]; //Initial state value [x',x,theta', theta] (the state order is determined by the compiler)
U0=0;             //Initial input value force applied to the cart
Y0=[1;0];         //Initial value of the output [x; theta]

//Set variables to be computed
IndX=1:4;     //All the states
IndU=1;       //All the inputs
IndY=2;       //Only the second output (theta), the position x is imposed
IndXd=[];     //All state derivatives must be zero
[X,U,Y,Xd,cpr]=steadycos(scs_m,X0,U0,Y0,IndX,IndU,IndY,IndXd,list(1e-10,0));
sys=lincos(cpr,X,U);
sys=contrss(sys);

A_ref=[0.0000000855049,13.333872344818,-0.0000000694592,10.831655054733;
1,0,0,0;
0,-6.001172174465,0,-4.874999942768;
0,0,-1,0];

B_ref=[-1.5859964876424;0;0;0];
C_ref=[0,-0.6305184201126,0,0.7761742856799;
0,0.7761742857352,0,0.6305184201576];
D_ref=[0;0];
assert_checkalmostequal (sys.A ,A_ref,sqrt(%eps),1e-8);
assert_checkalmostequal (sys.B ,B_ref,sqrt(%eps),1e-8);
assert_checkalmostequal (sys.C ,C_ref,sqrt(%eps),1e-8);
assert_checkalmostequal (sys.D ,D_ref,sqrt(%eps),1e-8);


//Implicit system (requires a compiler)
//==================================================
importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/steadycos_lincos_modelica.zcos");

// look for the specific sub-diagram to work on
for i=1:length(scs_m.objs)
    if typeof(scs_m.objs(i))=="Block" & scs_m.objs(i).gui=="SUPER_f" then
        scs_m = scs_m.objs(i).model.rpar;
        break;
    end
end

[X,U,Y,XP] = steadycos(scs_m,[],[],[],[],1,1:$);
X = [0; 0; 0; 0;];
U = [0];
sys = lincos(scs_m, X, U)
A_ref = 3.3333333;
B_ref = -0.5773503;
C_ref = 5.7735027;
D_ref = 0;
assert_checkalmostequal (sys.A ,A_ref,sqrt(%eps),1e-3);
assert_checkalmostequal (sys.B ,B_ref,sqrt(%eps),1e-3);
assert_checkalmostequal (sys.C ,C_ref,sqrt(%eps),1e-3);
assert_checkalmostequal (sys.D ,D_ref,sqrt(%eps),1e-3);

