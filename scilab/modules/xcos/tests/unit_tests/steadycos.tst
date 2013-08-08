// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Serge Steer - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->


//Explicit system
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
[X,U,Y,Xd]=steadycos(scs_m,X0,U0,Y0,IndX,IndU,IndY,IndXd,list(1e-10,0));

execstr(scs_m.props.context)
X_ref=[0;Y0(1);0;-alpha];
U_ref=(M+m)*g*sin(alpha);
Y_ref=X_ref([2 4]);
Xd_ref=zeros(X_ref);

assert_checkalmostequal (X ,X_ref);
assert_checkalmostequal (U ,U_ref);
assert_checkalmostequal (Y ,Y_ref);
assert_checkalmostequal (Xd ,Xd_ref,sqrt(%eps),1e-8);

//same test with previously compiled diagram
[cpr,ok]=xcos_compile(scs_m);
[X,U,Y,Xd]=steadycos(cpr,X0,U0,Y0,IndX,IndU,IndY,IndXd,list(1e-10,0));
assert_checkalmostequal (X ,X_ref);
assert_checkalmostequal (U ,U_ref);
assert_checkalmostequal (Y ,Y_ref);
assert_checkalmostequal (Xd ,Xd_ref,sqrt(%eps),1e-8);


//Implicit system (requires a compiler)
importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/steadycos_lincos_modelica.zcos");

// look for the specific sub-diagram to work on
for i=1:length(scs_m.objs)
    if typeof(scs_m.objs(i))=="Block" & scs_m.objs(i).gui=="SUPER_f" then
        scs_m = scs_m.objs(i).model.rpar;
        break;
    end
end


[X,U,Y,XP] = steadycos(scs_m,[],[],[],[],1,1:$);
X_ref = [0; 0];
U_ref = [0];
Y_ref = [0];
XP_ref = [0; 0];
assert_checkalmostequal (X ,X_ref,sqrt(%eps),1e-3);
assert_checkalmostequal (U ,U_ref,sqrt(%eps),1e-3);
assert_checkalmostequal (Y ,Y_ref,sqrt(%eps),1e-3);
assert_checkalmostequal (XP ,XP_ref,sqrt(%eps),1e-3);
