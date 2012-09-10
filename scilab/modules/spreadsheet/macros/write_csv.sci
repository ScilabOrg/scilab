// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009-2012 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function write_csv(var, fname, sep, dec)
         if ~isdef("sep")
            sep=","
         end
         if ~isdef("dec")
            dec="."
         end
         csv_write(var, fname, sep, dec)
endfunction
