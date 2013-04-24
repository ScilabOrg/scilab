// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
//   <bruno.pincon@iecn.u-nancy.fr>
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// display the file bench.txt

txt = read(SCI+"/modules/umfpack/demos/bench.txt",-1,1,"(A)");
messagebox(_(txt),"modal","info");

