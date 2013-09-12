// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2008 - INRIA - Sylvestre LEDRU (add cc options)
// Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//=============================================================================
function ilib_mex_build(ilib_name, table, files, libs, makename, ldflags, cflags, fflags, cc)

    if ~haveacompiler() then
        error(msprintf(gettext("%s: A Fortran or C compiler is required.\n"), "ilib_mex_build"));
        return;
    end

    [lhs,rhs] = argn(0);
    if rhs < 4 then
        error(msprintf(gettext("%s: Wrong number of input argument(s).\n"), "ilib_mex_build"));
        return
    end

    if rhs > 4 & makename <> [] & makename <> "" then
        warning(msprintf(_("%s: Wrong value for input argument #%d: """" or ""[]"" expected.\n"),"ilib_mex_build", 5));
    end

    if ~isempty(files) & (or(fileext(files)==".o") | or(fileext(files)==".obj")) then
        error(999, msprintf(_("%s: A managed file extension for input argument #%d expected."), "ilib_mex_build", 3));
    end

    if ~isempty(files) & ~and(isfile(files)) then
        error(999, msprintf(_("%s: Wrong value for input argument #%d: existing file(s) expected.\n"), "ilib_mex_build", 3));
    end

    if rhs <= 5 then ldflags = ""; end
    if rhs <= 6 then cflags  = ""; end
    if rhs <= 7 then fflags  = ""; end
    if rhs <= 8 then cc  = ""; end

    if ~isdef("makename") then
        makename = "";
    end


    if getos() <> "Windows" then
        // These flags are important... The rename of the fly the name of the
        // function in the C / C++ / Fortran code
        cflags=" -DmexFunction=mex_\$* " + cflags
        fflags=" -Dmexfunction=mex\$* " + fflags
    end

    ilib_build(ilib_name, table, files, libs, makename, ldflags, cflags, fflags, %t, cc);

endfunction
//=============================================================================
