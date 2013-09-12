
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- JVM_NOT_MANDATORY -->

x_current = 1;

x_neigh = neigh_func_vfsa(x_current,10,[]);

if (x_neigh==%nan) | (abs(x_neigh)==%inf) then pause,end

