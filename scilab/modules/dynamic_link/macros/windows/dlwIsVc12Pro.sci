// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2014 - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
function bOK = dlwIsVc12Pro()
    bOK = %f;
    try
        if winqueryreg('HKLM', 'Software\Microsoft\DevDiv\vs\Servicing\12.0\devenv', "install") == 1  & ...
                dlwIsVc12Express() == %f then
            bOK = %t;
        end
    catch
    end
endfunction
//=============================================================================
