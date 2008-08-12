// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function r=Err(x),r=norm(x,1),endfunction
rand('normal')

//==========================================================================
//==============================    rcond     ============================== 
//==========================================================================
//Empty matrix
A=[];
if rcond(A)<>[] then pause,end

//Rectangular matrix
if execstr('rcond(rand(2,3))','errcatch')==0 then pause,end
if execstr('rcond(rand(2,3)+%i*eye())','errcatch')==0 then pause,end
//Small dimension
//---------------
//Real Case
if Err(rcond(eye(5,5))-1)>10*%eps then pause,end
//Complex Case
if  Err(rcond(eye(5,5)*(1+%i))-1)>10*%eps then pause,end


//Large dimension
//---------------
//Real Case
if Err(rcond(eye(50,50))-1)>10*%eps then pause,end
//Complex Case
if  Err(rcond(eye(50,50)*(1+%i))-1)>10*%eps then pause,end
