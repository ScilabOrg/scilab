// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael BAUDIN
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function isok = atomsCheckModule(modulenames, verbose)
    // Check the given list of ATOMS modules.
    //
    // Calling Sequence
    //   isok = atomsCheckModule(modulenames, verbose)
    //
    // Parameters
    //   names : a matrix of strings, the names of the modules to check
    //   verbose : a 1-by-1 matrix of booleans, set to true to display messages
    //
    // Description
    //   Installs, load, launch the tests and remove all the modules
    //   in modulenames.
    //
    // Examples
    //   atomsCheckModule("scibench", %t)
    //   atomsCheckModule(["apifun" "scibench"],%t)
    //

    tabsp = "  * ";
    if ~isdef("verbose") then
        verbose = %t;
    end
    //============================================================================
    function isok = atomsCheckModuleInstall(modulename, verbose)
        isok = %f;
        if verbose then
            mprintf(gettext("\n%s Installing ...\n"), tabsp);
        end

        if execstr("r = [];r = atomsInstall(modulename, ''user'');", "errcatch") == 0 then
            isok = (r <> []);
            if isok & verbose then
                disp(r);
            end
        end
    endfunction
    //============================================================================
    function isok = atomsCheckModuleLoad(modulename, verbose)
        isok = %f;
        if verbose then
            mprintf(gettext("\n%s Loading ...\n"), tabsp);
        end

        if execstr("r = [];r = atomsLoad(modulename);", "errcatch") <> 0 then
            isok = %f;
        else
            isok = (r <> []);
            if isok & verbose then
                disp(r);
            end
        end
    endfunction
    //============================================================================
    function isok = atomsCheckModuleTest(modulename, verbose)
        isok = %f;
        if verbose then
            mprintf(gettext("\n%s Testing ...\n"), tabsp);
        end

        isok = atomsTest(modulename);
    endfunction
    //============================================================================
    function isok = atomsCheckModuleRemove(modulename, verbose)
        isok = %f;
        if verbose then
            mprintf(gettext("\n%s Removing ...\n"), tabsp);
        end
        if execstr("r = atomsRemove(modulename, ''user'', %t);", "errcatch") <> 0 then
            isok = %f;
        else
            isok = (r == []);
        end
    endfunction
    //============================================================================
    isok = %f;

    if type(modulenames) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: a matrix of string expected.\n"), "atomsCheckModule", 1));
    end

    if type(verbose) <> 4 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: a boolean expected.\n"), "atomsCheckModule", 2));
    end

    if size(verbose, "*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: a boolean expected.\n"), "atomsCheckModule", 2));
    end

    nm = size(modulenames, "*");

    for k = 1:nm
        modulename = modulenames(k);
        im = atomsIsInstalled(modulename);
        if im then
            error(msprintf(gettext("%s: Module %s is already installed. Please remove it before checking it.\n"), "atomsCheckModule", modulename));
        end
    end

    for k = 1:nm
        modulename = modulenames(k);
        r = atomsSearch(modulename);
        if grep(r(:, 1), modulename)  == [] then
            error(msprintf(gettext("%s: Module %s does not exist in current repositories.\n"), "atomsCheckModule", modulename));
        end
    end

    if nm > 1 then
        u = unique(modulenames);
        if size(u, "*") <> nm then
            error(msprintf(gettext("%s: Wrong value(s) for input argument #%d: some unique module names expected.\n"), "atomsCheckModule", 1));
        end
    end

    for k = 1:nm
        im = atomsIsInstalled(modulename);
        if im then
            error(msprintf(gettext("%s: Module %s is already installed. Please remove it before checking it.\n"), "atomsCheckModule", modulename));
        end
    end

    if verbose then
        mprintf(gettext("\nNumber of modules to check: %d\n"), nm);
    end

    for k = 1:nm
        modulename = modulenames(k);

        isok(k) = atomsCheckModuleInstall(modulename, verbose);
        isok(k) = isok(k) & atomsCheckModuleLoad(modulename, verbose);
        isok(k) = isok(k) & atomsCheckModuleTest(modulename, verbose);
        isok(k) = isok(k) & atomsCheckModuleRemove(modulename, verbose);
    end

    isok = matrix(isok, size(modulenames, "r"), size(modulenames, "c"));

endfunction


