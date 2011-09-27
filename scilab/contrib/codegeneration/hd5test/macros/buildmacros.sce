
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET <allan.cornet@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//Delete old binaries
mdelete("*.bin");

genlib("hd5testlib",SCI+"\contrib\codegeneration\hd5test",%f,%t);

//copy the new files to the scilab directory
//copyfile(thisDir, SCI+"\contrib\20-sim4C\macros");