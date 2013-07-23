// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

msgerror = msprintf(gettext("Incorrect assignment.\n"));
assert_checkerror ( "error(1)" , msgerror );

msgerror = msprintf(gettext("Invalid factor.\n"));
assert_checkerror ( "error(2)" , msgerror );

msgerror = msprintf(gettext("Waiting for right parenthesis.\n"));
assert_checkerror ( "error(3)" , msgerror );

msgerror = msprintf(gettext("Undefined variable: %s\n"), "error");
assert_checkerror ( "error(4)" , msgerror );

msgerror = msprintf(gettext("Inconsistent column/row dimensions.\n"));
assert_checkerror ( "error(5)" , msgerror );

msgerror = msprintf(gettext("Inconsistent row/column dimensions.\n"));
assert_checkerror ( "error(6)" , msgerror );

msgerror = msprintf(gettext("Dot cannot be used as modifier for this operator.\n"));
assert_checkerror ( "error(7)" , msgerror );

msgerror = msprintf(gettext("Inconsistent addition.\n"));
assert_checkerror ( "error(8)" , msgerror );

msgerror = msprintf(gettext("Inconsistent subtraction.\n"));
assert_checkerror ( "error(9)" , msgerror );

msgerror = msprintf(gettext("Inconsistent multiplication.\n"));
assert_checkerror ( "error(10)" , msgerror );

msgerror = msprintf(gettext("Inconsistent right division.\n"));
assert_checkerror ( "error(11)" , msgerror );

msgerror = msprintf(gettext("Inconsistent left division.\n"));
assert_checkerror ( "error(12)" , msgerror );

msgerror = msprintf(gettext("Redefining permanent variable.\n"));
assert_checkerror ( "error(13)" , msgerror );

msgerror = msprintf(gettext("Eye variable undefined in this context.\n"));
assert_checkerror ( "error(14)" , msgerror );

msgerror = msprintf(gettext("Submatrix incorrectly defined.\n"));
assert_checkerror ( "error(15)" , msgerror );

msgerror = msprintf(gettext("Incorrect command!\n"));
assert_checkerror ( "error(16)" , msgerror );

msgerror = msprintf(gettext("Too many variables!\n"));
assert_checkerror ( "error(18)" , msgerror );

msgerror = msprintf(gettext("Problem is singular.\n"));
assert_checkerror ( "error(19)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: Square matrix expected.\n"), 3);
assert_checkerror ( "error(20,3)" , msgerror );

msgerror = msprintf(gettext("Invalid index.\n"));
assert_checkerror ( "error(21)" , msgerror );

msgerror = msprintf(gettext("Recursion problems. Sorry...\n"));
assert_checkerror ( "error(22)" , msgerror );

msgerror = msprintf(gettext("Matrix norms available are 1, 2, inf, and fro.\n"));
assert_checkerror ( "error(23)" , msgerror );

msgerror = msprintf(gettext("Convergence problem...\n"));
assert_checkerror ( "error(24)" , msgerror );

msgerror = msprintf(gettext("Bad call to primitive: %s\n"), "error");
assert_checkerror ( "error(25)" , msgerror );

msgerror = msprintf(gettext("Too complex recursion! (recursion tables are full)\n"));
assert_checkerror ( "error(26)" , msgerror );

msgerror = msprintf(gettext("Division by zero...\n"));
assert_checkerror ( "error(27)" , msgerror );

msgerror = msprintf(gettext("Empty function...\n"));
assert_checkerror ( "error(28)" , msgerror );

msgerror = msprintf(gettext("Matrix is not positive definite.\n"));
assert_checkerror ( "error(29)" , msgerror );

msgerror = msprintf(gettext("Invalid exponent.\n"));
assert_checkerror ( "error(30)" , msgerror );

msgerror = msprintf(gettext("Incorrect string.\n"));
assert_checkerror ( "error(31)" , msgerror );

msgerror = msprintf(gettext("Singularity of log or tan function.\n"));
assert_checkerror ( "error(32)" , msgerror );

msgerror = msprintf(gettext("Too many '':''\n"));
assert_checkerror ( "error(33)" , msgerror );

msgerror = msprintf(gettext("Incorrect control instruction syntax.\n"));
assert_checkerror ( "error(34)" , msgerror );

msgerror = msprintf(gettext("Incorrect control instruction syntax.\n"));
assert_checkerror ( "error(34)" , msgerror );

msgerror = msprintf(gettext("Syntax error in a ''%s'' instruction.\n") , "select/case");
assert_checkerror ( "error(35)" , msgerror );

msgerror = msprintf(gettext("Wrong input argument #%d.\n") , 18);
assert_checkerror ( "error(36,18)" , msgerror );

msgerror = msprintf(gettext("Incorrect function at line %d.\n") , 18);
assert_checkerror ( "error(37,18)" , msgerror );

msgerror = msprintf(gettext("Incorrect function.\n"));
assert_checkerror ( "error(37)" , msgerror );

msgerror = msprintf(gettext("Wrong file name.\n"));
assert_checkerror ( "error(38)" , msgerror );

msgerror = msprintf(gettext("Incorrect number of input arguments.\n"));
assert_checkerror ( "error(39)" , msgerror );

msgerror = msprintf(gettext("Waiting for end of command.\n"));
assert_checkerror ( "error(40)" , msgerror );

msgerror = msprintf(gettext("Incompatible output argument.\n"));
assert_checkerror ( "error(41)" , msgerror );

msgerror = msprintf(gettext("Incompatible input argument.\n"));
assert_checkerror ( "error(42)" , msgerror );

msgerror = msprintf(gettext("Not implemented in scilab...\n"));
assert_checkerror ( "error(43)" , msgerror );

msgerror = msprintf(gettext("Wrong argument #%d.\n"), 8);
assert_checkerror ( "error(44,8)" , msgerror );

msgerror = msprintf(gettext("null matrix (argument # %d).\n"), 81);
assert_checkerror ( "error(45,81)" , msgerror );

msgerror = msprintf(gettext("Incorrect syntax.\n"));
assert_checkerror ( "error(46)" , msgerror );

msgerror = msprintf(gettext(" end or else is missing...\n"));
assert_checkerror ( "error(47)" , msgerror );

msgerror = msprintf(gettext(" input line longer than buffer size: %d\n"), 4096);
assert_checkerror ( "error(48)" , msgerror );

msgerror = msprintf(gettext("Incorrect file or format.\n"));
assert_checkerror ( "error(49)" , msgerror );

msgerror = msprintf(gettext("%s: subroutine not found.\n"), "");
assert_checkerror ( "error(50)" , msgerror );

msgerror = "";
assert_checkerror ( "error(51)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: Real matrix expected.\n"), 69);
assert_checkerror ( "error(52, 69)" , msgerror );

msgerror = msprintf(gettext("Wrong type for input argument #%d: Real or complex matrix expected.\n"), 69);
assert_checkerror ( "error(53, 69)" , msgerror );

msgerror = msprintf(gettext("Wrong type for input argument #%d: Polynomial expected.\n"), 69);
assert_checkerror ( "error(54, 69)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: String expected.\n"), 69);
assert_checkerror ( "error(55, 69)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: List expected.\n"), 69);
assert_checkerror ( "error(56, 69)" , msgerror );

msgerror = msprintf(gettext("Problem with comparison symbol...\n"));
assert_checkerror ( "error(57)" , msgerror );

msgerror = msprintf(gettext("Wrong number of input arguments."));
assert_checkerror ( "error(58)" , msgerror );

msgerror = msprintf(gettext("Wrong number of output arguments.\n"));
assert_checkerror ( "error(59)" , msgerror );

msgerror = msprintf(gettext("Wrong size for argument: Incompatible dimensions.\n"));
assert_checkerror ( "error(60)" , msgerror );

msgerror = msprintf(gettext("Direct access : give format.\n"));
assert_checkerror ( "error(61)" , msgerror );

msgerror = msprintf(gettext("End of file at line %d.\n"), 45);
assert_checkerror ( "error(62,45)" , msgerror );

msgerror = msprintf(gettext("%d graphic terminal?\n"), 45);
assert_checkerror ( "error(63,45)" , msgerror );

msgerror = msprintf(gettext("Integration fails.\n"));
assert_checkerror ( "error(64)" , msgerror );

msgerror = msprintf(gettext("%d: logical unit already used.\n"), 45);
assert_checkerror ( "error(65,45)" , msgerror );

msgerror = msprintf(gettext("Too many files opened!\n"));
assert_checkerror ( "error(66)" , msgerror );

msgerror = msprintf(gettext("Unknown file format.\n"));
assert_checkerror ( "error(67)" , msgerror );

msgerror = msprintf(gettext("Floating point exception.\n"));
assert_checkerror ( "error(69)" , msgerror );

msgerror = msprintf(gettext("Too many arguments in fort (max 30).\n"));
assert_checkerror ( "error(70)" , msgerror );

msgerror = msprintf(gettext("This variable is not valid in fort.\n"));
assert_checkerror ( "error(71)" , msgerror );

msgerror = msprintf(gettext("%s is not valid in this context.\n"), "error");
assert_checkerror ( "error(72)" , msgerror );

msgerror = msprintf(gettext("Error while linking.\n"));
assert_checkerror ( "error(73)" , msgerror );

msgerror = msprintf(gettext("Leading coefficient is zero.\n"));
assert_checkerror ( "error(74)" , msgerror );

msgerror = msprintf(gettext("Too high degree (max 100).\n"));
assert_checkerror ( "error(75)" , msgerror );

msgerror = msprintf(gettext("for x=val with type(val)=%d  is not implemented in Scilab.\n"),18);
assert_checkerror ( "error(76, 18)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong number of input arguments.\n"), "error");
assert_checkerror ( "error(77)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong number of output arguments.\n"), "error");
assert_checkerror ( "error(78)" , msgerror );

msgerror = msprintf(gettext("Indexing not allowed for output arguments of resume.\n"));
assert_checkerror ( "error(79)" , msgerror );

msgerror = msprintf(gettext("Incorrect function (argument n: %d).\n"), 99);
assert_checkerror ( "error(80, 99)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Real or complex matrix expected.\n"), "error", 99);
assert_checkerror ( "error(81, 99)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"), "error", 99);
assert_checkerror ( "error(82, 99)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Real vector expected.\n"), "error", 99);
assert_checkerror ( "error(83, 99)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Scalar expected.\n"), "error", 99);
assert_checkerror ( "error(84, 99)" , msgerror );

msgerror = msprintf(gettext("Host does not answer...\n"));
assert_checkerror ( "error(85)" , msgerror );

msgerror = msprintf(gettext("Uncontrollable system.\n"));
assert_checkerror ( "error(86)" , msgerror );

msgerror = msprintf(gettext("Unobservable system.\n"));
assert_checkerror ( "error(87)" , msgerror );

msgerror = msprintf(gettext("%s: singular or asymmetric problem.\n"), "sfact");
assert_checkerror ( "error(88)" , msgerror );

msgerror = msprintf(gettext("Wrong size for argument #%d.\n"), 38);
assert_checkerror ( "error(89, 38)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: Transfer matrix expected.\n"), 38);
assert_checkerror ( "error(90, 38)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: In state space form expected.\n"), 38);
assert_checkerror ( "error(91, 38)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: Rational matrix expected.\n"), 38);
assert_checkerror ( "error(92, 38)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: In continuous time expected.\n"), 38);
assert_checkerror ( "error(93, 38)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: In discrete time expected.\n"), 38);
assert_checkerror ( "error(94, 38)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: SISO expected.\n"), 38);
assert_checkerror ( "error(95, 38)" , msgerror );

msgerror = msprintf(gettext("time domain of argument #%d is not defined.\n"), 38);
assert_checkerror ( "error(96, 38)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: A system in state space or transfer matrix form expected.\n"), 38);
assert_checkerror ( "error(97, 38)" , msgerror );

msgerror = msprintf(gettext("Variable returned by scilab argument function is incorrect.\n"));
assert_checkerror ( "error(98)" , msgerror );

msgerror = msprintf(gettext("Elements of %dth argument must be in increasing order.\n"), 16);
assert_checkerror ( "error(99, 16)" , msgerror );

msgerror = msprintf(gettext("Elements of %dth argument are not in (strictly) decreasing order.\n"), 16);
assert_checkerror ( "error(100, 16)" , msgerror );

msgerror = msprintf(gettext("Last element of %dth argument <> first.\n"), 16);
assert_checkerror ( "error(101, 16)" , msgerror );

msgerror = msprintf(gettext("Variable or function %s are not in file.\n"), "error");
assert_checkerror ( "error(102)" , msgerror );

msgerror = msprintf(gettext("Variable %s is not a valid rational function.\n"), "error");
assert_checkerror ( "error(103)" , msgerror );

msgerror = msprintf(gettext("Variable %s is not a valid state space representation.\n"), "error");
assert_checkerror ( "error(104)" , msgerror );

msgerror = msprintf(gettext("Undefined function.\n"));
assert_checkerror ( "error(105)" , msgerror );

msgerror = msprintf(gettext("Function name already used.\n"));
assert_checkerror ( "error(106)" , msgerror );

msgerror = msprintf(gettext("Too many functions are defined (maximum #:%d).\n"), 18);
assert_checkerror ( "error(107, 18)" , msgerror );

msgerror = msprintf(gettext("Too complex for scilab, may be a too long control instruction.\n"));
assert_checkerror ( "error(108)" , msgerror );

msgerror = msprintf(gettext("Too large, can''t be displayed.\n"));
assert_checkerror ( "error(109)" , msgerror );

msgerror = msprintf(gettext("%s was a function when compiled but is now a primitive!\n"), "error");
assert_checkerror ( "error(110)" , msgerror );

msgerror = msprintf(gettext("Trying to re-define function %s.\n"), "error");
assert_checkerror ( "error(111)" , msgerror );

msgerror = msprintf(gettext("No more memory.\n"));
assert_checkerror ( "error(112)" , msgerror );

msgerror = msprintf(gettext("Too large string.\n"));
assert_checkerror ( "error(113)" , msgerror );

msgerror = msprintf(gettext("Too many linked routines.\n"));
assert_checkerror ( "error(114)" , msgerror );

msgerror = msprintf(gettext("Wrong value for argument #%d.\n"), 18);
assert_checkerror ( "error(116, 18)" , msgerror );

msgerror = msprintf(gettext("List element number %d is Undefined.\n"), 18);
assert_checkerror ( "error(117, 18)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: Named variable not an expression expected.\n"), 18);
assert_checkerror ( "error(118, 18)" , msgerror );

msgerror = "";
assert_checkerror ( "error(119)" , msgerror );

msgerror = msprintf(gettext("Indices for non-zero elements must be given by a 2 column matrix.\n"));
assert_checkerror ( "error(120)" , msgerror );

msgerror = msprintf(gettext("Incompatible indices for non-zero elements.\n"));
assert_checkerror ( "error(121)" , msgerror );

msgerror = msprintf(gettext("Logical unit number should be larger than %d.\n"), 12);
assert_checkerror ( "error(122, 12)" , msgerror );

msgerror = msprintf(gettext("Function not bounded from below.\n"));
assert_checkerror ( "error(123)" , msgerror );

msgerror = "";
assert_checkerror ( "error(124)" , msgerror );

msgerror = msprintf(gettext("Problem may be unbounded: too high distance between two consecutive iterations.\n"));
assert_checkerror ( "error(125)" , msgerror );

msgerror = msprintf(gettext("Inconsistent constraints.\n"));
assert_checkerror ( "error(126)" , msgerror );

msgerror = msprintf(gettext("No feasible solution.\n"));
assert_checkerror ( "error(127)" , msgerror );

msgerror = msprintf(gettext("Degenerate starting point.\n"));
assert_checkerror ( "error(128)" , msgerror );

msgerror = msprintf(gettext("No feasible point has been found.\n"));
assert_checkerror ( "error(129)" , msgerror );

msgerror = msprintf(gettext("Optimization fails: back to initial point.\n"));
assert_checkerror ( "error(130)" , msgerror );

msgerror = msprintf(gettext("%s: Stop requested by simulator (ind=0)\n"), "optim");
assert_checkerror ( "error(131)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong input parameters.\n"), "optim");
assert_checkerror ( "error(132)" , msgerror );

msgerror = msprintf(gettext("Too small memory.\n"));
assert_checkerror ( "error(133)" , msgerror );

msgerror = msprintf(gettext("%s: Problem with initial constants in simul.\n"), "optim");
assert_checkerror ( "error(134)" , msgerror );

msgerror = msprintf(gettext("%s: Bounds and initial guess are incompatible.\n"), "optim");
assert_checkerror ( "error(135)" , msgerror );

msgerror = msprintf(gettext("%s: This method is NOT implemented.\n"), "optim");
assert_checkerror ( "error(136)" , msgerror );

msgerror = msprintf(gettext("NO hot restart available in this method.\n"));
assert_checkerror ( "error(137)" , msgerror );

msgerror = msprintf(gettext("%s: Incorrect stopping parameters.\n"), "optim");
assert_checkerror ( "error(138)" , msgerror );

msgerror = msprintf(gettext("%s: Incorrect bounds.\n"), "optim");
assert_checkerror ( "error(139)" , msgerror );

msgerror = msprintf(gettext("Variable : %s must be a list\n"), "error");
assert_checkerror ( "error(140)" , msgerror );

msgerror = msprintf(gettext("Incorrect function (argument n: %d).\n"), 22);
assert_checkerror ( "error(141, 22)" , msgerror );

msgerror = msprintf(gettext("Hot restart: dimension of working table (argument n:%d).\n"), 22);
assert_checkerror ( "error(142, 22)" , msgerror );

msgerror = msprintf(gettext("%s: df0 must be positive !\n"), "optim");
assert_checkerror ( "error(143)" , msgerror );

msgerror =  msprintf(gettext("%s: Wrong type for argument #%d: Matrix of handle expected.\n"), "error", 0);
assert_checkerror ( "error(200)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Real or Complex matrix expected.\n"), "error", 22);
assert_checkerror ( "error(201, 22)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Real matrix expected.\n"), "error", 22);
assert_checkerror ( "error(202, 22)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Real vector expected.\n"), "error", 22);
assert_checkerror ( "error(203, 22)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Scalar expected.\n"), "error", 22);
assert_checkerror ( "error(204, 22)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong size for argument #%d: (%d,%d) expected.\n"), "error", 0, 1 , 301);
assert_checkerror ( "error(205)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong size for argument #%d: %d expected.\n"), "error", 0, 1);
assert_checkerror ( "error(206)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Matrix of strings expected.\n"), "error", 0);
assert_checkerror ( "error(207)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Boolean matrix expected.\n"), "error", 20);
assert_checkerror ( "error(208, 20)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Matrix expected.\n"), "error", 20);
assert_checkerror ( "error(209, 20)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: List expected.\n"), "error", 20);
assert_checkerror ( "error(210, 20)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Function or string (external function) expected.\n"), "error", 20);
assert_checkerror ( "error(211, 20)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Polynomial matrix expected.\n"), "error", 20);
assert_checkerror ( "error(212, 20)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Working int matrix expected.\n"), "error", 20);
assert_checkerror ( "error(213, 20)" , msgerror );

msgerror = msprintf(gettext("%s: Wrong type for argument #%d: Vector expected.\n"), "error", 20);
assert_checkerror ( "error(214, 20)" , msgerror );

msgerror = msprintf(gettext("%dth argument type must be boolean.\n"), 20);
assert_checkerror ( "error(215, 20)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: Boolean or scalar expected.\n"), 20);
assert_checkerror ( "error(216, 20)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: Sparse matrix of scalars expected.\n"), 20);
assert_checkerror ( "error(217, 20)" , msgerror );

msgerror = msprintf(gettext("Wrong type for argument #%d: Handle to sparse lu factors expected.\n"), 20);
assert_checkerror ( "error(218, 20)" , msgerror );

msgerror = msprintf(gettext("Wrong type argument #%d: Sparse or full scalar matrix expected.\n"), 20);
assert_checkerror ( "error(219, 20)" , msgerror );

msgerror = msprintf(gettext("Null variable cannot be used here.\n"));
assert_checkerror ( "error(220)" , msgerror );

msgerror = msprintf(gettext("A sparse matrix entry is defined with two different values.\n"));
assert_checkerror ( "error(221)" , msgerror );

msgerror = msprintf(gettext("%s not yet implemented for full input parameter.\n"), "lusolve");
assert_checkerror ( "error(222)" , msgerror );

msgerror = msprintf(gettext("It is not possible to redefine the %s primitive this way (see clearfun).\n"), "error");
assert_checkerror ( "error(223)" , msgerror );

msgerror = msprintf(gettext("Type data base is full.\n"));
assert_checkerror ( "error(224)" , msgerror );

msgerror = msprintf(gettext("This data type is already defined.\n"));
assert_checkerror ( "error(225)" , msgerror );

msgerror = msprintf(gettext("Inequality comparison with empty matrix.\n"));
assert_checkerror ( "error(226)" , msgerror );

msgerror = msprintf(gettext("Missing index.\n"));
assert_checkerror ( "error(227)" , msgerror );

msgerror = msprintf(gettext("reference to the cleared global variable %s.\n"), "error");
assert_checkerror ( "error(228)" , msgerror );

msgerror = msprintf(gettext("Operands of / and \\ operations must not contain NaN of Inf.\n"));
assert_checkerror ( "error(229)" , msgerror );

msgerror = msprintf(gettext("semi def fails.\n"));
assert_checkerror ( "error(230)" , msgerror );

msgerror = msprintf(gettext("Wrong type for first input argument: Single string expected.\n"));
assert_checkerror ( "error(231)" , msgerror );

msgerror = msprintf(gettext("Entry name not found.\n"));
assert_checkerror ( "error(232)" , msgerror );

msgerror = msprintf(gettext("Maximum number of dynamic interfaces reached.\n"));
assert_checkerror ( "error(233)" , msgerror );

msgerror = msprintf(gettext("%s: expecting more than one argument.\n"), "link");
assert_checkerror ( "error(234)" , msgerror );

msgerror = msprintf(gettext("%s: problem with one of the entry point.\n"), "link");
assert_checkerror ( "error(235)" , msgerror );

msgerror = msprintf(gettext("%s: the shared archive was not loaded.\n"), "link");
assert_checkerror ( "error(236)" , msgerror );

msgerror = msprintf(gettext("%s: Only one entry point allowed on this operating system.\n"), "link");
assert_checkerror ( "error(237)" , msgerror );

msgerror = msprintf(gettext("%s: First argument cannot be a number.\n"), "link");
assert_checkerror ( "error(238)" , msgerror );

msgerror = msprintf(gettext("You cannot link more functions, maxentry reached.\n"));
assert_checkerror ( "error(239)" , msgerror );

msgerror = msprintf(gettext("You cannot link more functions, maxentry reached.\n"));
assert_checkerror ( "error(239)" , msgerror );

msgerror = msprintf(gettext("File ""%s"" already exists or directory write access denied.\n"), "");
assert_checkerror ( "error(240)" , msgerror );

msgerror = msprintf(gettext("File ""%s"" does not exist or read access denied.\n"), "");
assert_checkerror ( "error(241)" , msgerror );

msgerror = msprintf(gettext("Binary direct access files must be opened by ''file''.\n"));
assert_checkerror ( "error(242)" , msgerror );

msgerror = msprintf(gettext("C file logical unit not allowed here.\n"));
assert_checkerror ( "error(243)" , msgerror );

msgerror = msprintf(gettext("Fortran file logical unit not allowed here.\n"));
assert_checkerror ( "error(244)" , msgerror );

msgerror = msprintf(gettext("No input file associated to logical unit %d.\n"), 23);
assert_checkerror ( "error(245, 23)" , msgerror );

msgerror = msprintf(gettext("Wrong value for argument #%d: the lu handle is no more valid.\n"), 45);
assert_checkerror ( "error(247, 45)" , msgerror );

msgerror = msprintf(gettext("Wrong value for argument #%d: Valid variable name expected.\n"), 45);
assert_checkerror ( "error(248, 45)" , msgerror );

msgerror = msprintf(gettext("Wrong value for argument #%d: Empty string expected.\n"), 45);
assert_checkerror ( "error(249, 45)" , msgerror );

msgerror = msprintf(gettext("Recursive extraction is not valid in this context.\n"));
assert_checkerror ( "error(250)" , msgerror );

msgerror = msprintf(gettext("%s: ipar dimensioned at least 11.\n"), "bvode");
assert_checkerror ( "error(251)" , msgerror );

msgerror = msprintf(gettext("%s: ltol must be of size ipar(4).\n"), "bvode");
assert_checkerror ( "error(252)" , msgerror );

msgerror = msprintf(gettext("%s: fixpnt must be of size ipar(11).\n"), "bvode");
assert_checkerror ( "error(253)" , msgerror );

msgerror = msprintf(gettext("%s: ncomp < 20 requested.\n"), "bvode");
assert_checkerror ( "error(254)" , msgerror );

msgerror = msprintf(gettext("%s: m must be of size ncomp.\n"), "bvode");
assert_checkerror ( "error(255)" , msgerror );

msgerror = msprintf(gettext("%s: sum(m) must be less than 40.\n"), "bvode");
assert_checkerror ( "error(256)" , msgerror );

msgerror = msprintf(gettext("%s: sum(m) must be less than 40.\n"), "bvode");
assert_checkerror ( "error(257)" , msgerror );

msgerror = msprintf(gettext("%s: input data error.\n"), "bvode");
assert_checkerror ( "error(258)" , msgerror );

msgerror = msprintf(gettext("%s: no. of subintervals exceeds storage.\n"), "bvode");
assert_checkerror ( "error(259)" , msgerror );

msgerror = msprintf(gettext("%s: The colocation matrix is singular.\n"), "bvode");
assert_checkerror ( "error(260)" , msgerror );

msgerror = msprintf(gettext("Interface property table is full.\n"));
assert_checkerror ( "error(261)" , msgerror );

msgerror = msprintf(gettext("Error while writing in file: disk full or deleted file.\n"));
assert_checkerror ( "error(263)" , msgerror );

msgerror = msprintf(gettext("Wrong value for argument #%d: Must not contain NaN or Inf.\n"), 2);
assert_checkerror ( "error(264, 2)" , msgerror );

msgerror = msprintf(gettext("%s and %s must have equal number of rows.\n"), "A", "B");
assert_checkerror ( "error(265)" , msgerror );

msgerror = msprintf(gettext("%s and %s must have equal number of columns.\n"), "A", "B");
assert_checkerror ( "error(266)" , msgerror );

msgerror = msprintf(gettext("%s and %s must have equal dimensions.\n"), "A", "B");
assert_checkerror ( "error(267)" , msgerror );

msgerror = msprintf(gettext("Invalid return value for function passed in arg %d.\n"), 8);
assert_checkerror ( "error(268, 8)" , msgerror );

msgerror = msprintf(gettext("Wrong value for argument #%d: eigenvalues must have negative real parts.\n"), 8);
assert_checkerror ( "error(269, 8)" , msgerror );

msgerror = msprintf(gettext("Wrong value for argument #%d: eigenvalues modulus must be less than one.\n"), 8);
assert_checkerror ( "error(270, 8)" , msgerror );

msgerror = msprintf(gettext("Size varying argument a*eye(), (arg %d) not allowed here.\n"), 8);
assert_checkerror ( "error(271, 8)" , msgerror );

msgerror = msprintf(gettext("endfunction is missing.\n"));
assert_checkerror ( "error(272)" , msgerror );

msgerror = msprintf(gettext("Instruction left hand side: waiting for a dot or a left parenthesis.\n"));
assert_checkerror ( "error(273)" , msgerror );

msgerror = msprintf(gettext("Instruction left hand side: waiting for a name.\n"));
assert_checkerror ( "error(274)" , msgerror );

msgerror = msprintf(gettext("varargout keyword cannot be used here.\n"));
assert_checkerror ( "error(275)" , msgerror );

msgerror = msprintf(gettext("Missing operator, comma, or semicolon.\n"));
assert_checkerror ( "error(276)" , msgerror );

msgerror = msprintf(gettext("Too many commands defined.\n"));
assert_checkerror ( "error(277)" , msgerror );

msgerror = msprintf(gettext("%s: Input arguments should have the same formal variable name.\n"), "error");
assert_checkerror ( "error(278)" , msgerror );

msgerror = "";
assert_checkerror ( "error(279)" , msgerror );

msgerror = "";
assert_checkerror ( "error(280)" , msgerror );

