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

//Implicit system (requires Codelica and a compiler)
if ~atomsIsInstalled("coselica") then atomsInstall("coselica");end
importXcosDiagram("SCI/modules/xcos/tests/unit_tests/IVPDM.zcos")
[cpr,ok]=xcos_compile(scs_m);
//Spécification du point initial
X0=[0;0;0;0;0]; //Valeur initiale des états [x;x';theta;theta';z]
U0=0;            //Valeur initiale des entrées
Y0=[1;0];       //Valeur initiale des sorties

//Spécification des variables à déterminer
IndX=1:5;     //Tous les états
IndU=1;        //Toutes les entrées
IndY=2;         //La deuxième sortie (Theta) est libre, la première (x) est imposée
IndXd=[];      //Toutes les dérivés  des états doivent être nulles

//Recherche d'un état stationnaire
[X,U,Y,Xd]=steadycos(cpr,X0,U0,Y0,IndX,IndU,IndY,IndXd,list(1e-10,0));
execstr(scs_m.props.context)
X_ref=[Y0(1);0;-alpha;0;0];
U_ref=(M+m)*g*sin(alpha);
Y_ref=X_ref([1 3]);
Xd_ref=zeros(X_ref);

assert_checkalmostequal (X ,X_ref);
assert_checkalmostequal (U ,U_ref);
assert_checkalmostequal (Y ,Y_ref);
assert_checkalmostequal (Xd ,Xd_ref,sqrt(%eps),1e-8);
