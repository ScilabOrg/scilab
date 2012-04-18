// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function varargout = listvarinfile(filename)

    [lhs, rhs] = argn();

    //check file format
    if is_hdf5_file(filename) then
        if lhs > 1 then
            [a,b,c,d] = listvar_in_hdf5(filename);
        else
            a = listvar_in_hdf5(filename);
        end
    else
        if lhs > 1 then
            [a,b,c,d] = %_listvarinfile(filename);
        else
            a = %_listvarinfile(filename);
        end
    end

    if lhs == 1 then
        varargout = list([]);
    elseif lhs == 2 then
        varargout = list(a,b);
    elseif lhs == 3 then
        varargout = list(a,b,c);
    elseif lhs == 4 then
        varargout = list(a,b,c,d);
    end
endfunction
