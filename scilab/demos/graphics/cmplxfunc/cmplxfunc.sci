
function []=func1()
expr=['R = 4;  e = 0.001; theta = 30; alpha = 60;'
		'PlotCmplxFunc(R,e,""Disk"",""Ox"",[40 20],""log"",theta,alpha,[e,R])']
		x_message(expr);
		execstr(expr);
endfunction
		
function []=func2()
expr=['R = 2; theta = -130; alpha = 73;'
		'PlotCmplxFunc(R,0,""Disk"",""Ox"",[40 20],""exp"",theta,alpha,[-R,R])']
		x_message(expr);
		execstr(expr);
endfunction

function []=func3()
expr=['R = %pi/2-0.15; theta = -130; alpha = 73;'
		'PlotCmplxFunc(R,0,""Square"",""Ox"",41,""tan"",theta,alpha,[-R,R])']
		x_message(expr);
		execstr(expr);
endfunction

function []=func4()
expr=['R = 2 ; theta = -110; alpha = 75;'
		'PlotCmplxFunc(R,0.001,""Square"",""Oy"",41,""atan"",theta,alpha,[-R,R])']
		x_message(expr);
		execstr(expr);
endfunction

function []=func5()
expr=['R = %pi; theta = -130; alpha = 73; e = 0.001;'
		'PlotCmplxFunc(R,e,""Disk"",""Ox"",[40 20],""sin"",theta,alpha,[-R,R])']
		x_message(expr);
		execstr(expr);
endfunction

function []=func6()
expr=['theta = -110; alpha = 75;'
		'PlotCmplxFunc(2,%eps,""Square"",""Ox"",41,""asin"",theta,alpha,[-1,1])']
		x_message(expr);
		execstr(expr);
endfunction



function []=func7()
expr=['R = %pi; theta = 18; alpha = 43;'
		'PlotCmplxFunc(R,0,""Disk"",""Ox"",[40 20],""cos"",theta,alpha,[-R,R])']
		x_message(expr);
		execstr(expr);
endfunction

function []=func8()
expr=['theta = -110; alpha = 75;'
		'PlotCmplxFunc(2,%eps,""Square"",""Ox"",41,""acos"",theta,alpha,[-1,1])']
		x_message(expr);
		execstr(expr);
endfunction

function []=func9()
expr=['R = %pi; e = 0; theta = -148; alpha = 60;'
		'PlotCmplxFunc(R,0,""Disk"",""Ox"",[40 20],""sinh"",theta,alpha,[-R,R])']
		x_message(expr);
		execstr(expr);
endfunction

function []=func10()
expr=['R = 2; theta = -110; alpha = 75;'
		'PlotCmplxFunc(2,%eps,""Square"",""Oy"",41,""asinh"",theta,alpha,[-R,R])']
		x_message(expr);
		execstr(expr);
endfunction

function []=func11()
expr=['R = %pi; e = 0; theta = -130; alpha = 56;'
		'PlotCmplxFunc(R,0,""Disk"",""Ox"",[40 20],""cosh"",theta,alpha,[-R,R])']
		x_message(expr);
		execstr(expr);
endfunction

function []=func12()
expr=['R = 2; theta = -110; alpha = 75;'
		'PlotCmplxFunc(R,%eps,""Square"",""Ox"",41,""acosh"",theta,alpha,[1,R])']
		x_message(expr);
		execstr(expr);
endfunction

function []=func13()
expr=['R = %pi/2-0.2; theta = -130; alpha = 73;'
		'PlotCmplxFunc(R,0,""Square"",""Ox"",41,""tanh"",theta,alpha,[-R,R])']
		x_message(expr);
		execstr(expr);
endfunction

function []=func14()
expr=['R = 2 ; theta = -110; alpha = 75;'
		'PlotCmplxFunc(R,0.001,""Square"",""Ox"",41,""atanh"",theta,alpha,[-0.99,0.99])']
		x_message(expr);
		execstr(expr);
endfunction

