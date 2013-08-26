// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Run the modules+"/macros/buildmacros.sce" script

function tbx_builder_macros(module)


    // Number of input arguments

    if argn(2)<>1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"tbx_builder_macros",1));
    end

    // Input argument N°1
    // → module

    if type(module) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"tbx_builder_macros",1));
    end

    if size(module,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"),"tbx_builder_macros",1));
    end

    if ~isdir(module) then
        error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_builder_macros",module));
    end

    mprintf(gettext("Building macros...\n"));
    tbx_builder(pathconvert(module+"/macros/buildmacros.sce",%F));

endfunction
