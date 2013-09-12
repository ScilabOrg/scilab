// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Simple wrapper around xmltojar

function tbx_build_help(moduletitle, path)

    rhs = argn(2);

    // Number of input arguments

    if and(rhs<> [1 2]) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "tbx_build_help", 1, 2));
    end

    // Input argument N°1
    // → name

    if type(moduletitle) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "tbx_build_help", 1));
    end

    if size(moduletitle,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"), "tbx_build_help", 1));
    end

    // Input argument N°2
    // → path

    if rhs < 2 then
        path = pwd();
    else
        if type(path) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), "tbx_build_help", 2));
        end

        if size(path,"*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"), "tbx_build_help", 2));
        end

        if ~isdir(path) then
            error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"), "tbx_build_help", path));
        end
    end

    path = pathconvert(path, %f, %t);

    // Check scilab mode
    if and(getscilabmode() <> ["STD" "NW"]) then
        error(msprintf(gettext("%s: documentation cannot be built in this scilab mode: %s.\n"),"tbx_build_help",getscilabmode()));
    end

    directory_language = basename(path);
    default_language = getlanguage();

    xmltojar(path, moduletitle, directory_language, default_language);

endfunction
