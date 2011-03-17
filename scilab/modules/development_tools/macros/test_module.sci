// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function status = test_module(_params)

    name = splitModule(_params.moduleName);
    
    if with_module(_params.moduleName) then
        // It's a scilab internal module
        module.path = pathconvert(SCI + "/modules/" + name(1), %F);
    elseif or(librarieslist() == "atomslib") & atomsIsLoaded(name(1)) then
        // It's an ATOMS module
        module.path = pathconvert(atomsGetLoadedPath(name(1)) , %F, %T);
    elseif isdir(name(1)) then
        // It's an external module
        module.path = pathconvert(name(1), %F);
    else
        // It's an error
        error(sprintf(gettext("%s is not an installed module or toolbox"), name(1)));
    end
    
    //get tests from path
    my_types = ["unit_tests","nonreg_tests"];

    directories = [];
    for i=1:size(my_types,"*")
        if (_params.testTypes == "all_tests") | (_params.testTypes == my_types(i)) then
            directory_path = module.path + "/tests/" + my_types(i);

            for j=2:size(name,"*")
                directory_path = directory_path + filesep() + module_items(j);
            end

            if isdir(directory_path) then
                directories = [directories;getDirectories(directory_path + filesep())];
            end
        end
    end

    tests = [];
    if( _params.tests_mat == [])
        for i=1:size(directories, "*")
            currentDir = directories(i);
            tests_mat = gsort(basename(listfiles(currentDir + filesep() + "*.tst")),"lr","i");
            for j = 1:size(tests_mat, "*")
                if or(tests_mat(j) == _params.skip_mat) == %f then
                    tests($+1, [1,2]) = [currentDir, tests_mat(j)];
                end
            end
        end
    else
        //not empty tests_mat
        for i = 1:size(_params.tests_mat, "*")
            for j = 1:size(directories, "*")
                currentDir = directories(j);
                testFile = currentDir + filesep() + _params.tests_mat(i) + ".tst";
                if isfile(testFile) then
                    tests($+1, [1,2]) = [currentDir, _params.tests_mat(i)];
                end
            end
        end
    end

    //initialize counter
    details_failed     = "";
    test_count         = size(tests, "r");
    test_passed_count  = 0;
    test_failed_count  = 0;
    test_skipped_count = 0;

    moduleName = _params.moduleName;
    // Improve the display of the module
    if isdir(moduleName) then
        if part(moduleName,1:length(SCI)) == SCI then
            moduleName = "SCI" + part(moduleName,length(SCI)+1:length(moduleName));
        elseif part(moduleName,1:length(SCIHOME)) == SCIHOME then
            moduleName = "SCIHOME" + part(moduleName,length(SCIHOME)+1:length(moduleName));
        end
    end

    //don't test only return list of tests.
    if _params.reference == "list" then
        for i = 1:test_count
            path = getPath(tests(i,1), my_types);
            if ~isempty(path) then
                displayModuleName = sprintf("%s", moduleName);
                for j=1:size(path, "*")
                    displayModuleName = displayModuleName + sprintf("|%s", path(j));
                end
            else
                displayModuleName = sprintf("%s", moduleName);
            end
            tests(i,1) = displayModuleName;
        end
        status.list         = tests;
        status.test_count   = test_count;
        return;
    end

    tic();
    for i = 1:test_count
        printf("   %03d/%03d - ",i, test_count);
        path = getPath(tests(i,1), my_types);
        if ~isempty(path) then
            displayModuleName = sprintf("[%s", moduleName);
            for j=1:size(path, "*")
                displayModuleName = displayModuleName + sprintf("|%s", path(j));
            end
            displayModuleName = displayModuleName + sprintf("] %s", tests(i,2));
        else
            displayModuleName = sprintf("[%s] %s", moduleName, tests(i,2));
        end
        printf("%s", displayModuleName);
        for j = length(displayModuleName):50
            printf(".");
        end
        
        result = test_single(_params, tests(i,1), tests(i,2));

        if result.id == 0 then
            printf("passed\n");
            test_passed_count = test_passed_count + 1;
        else
            msg = sprintf(result.message);
            printf("%s \n", msg(1));
            for kline = 2:size(msg, "*")
                printf(part(" ", 1:62) + "%s \n", msg(2));
            end
            
            if result.id > 0 & result.id < 10 then
                //failed
                test_failed_count = test_failed_count + 1;
                details_failed = [ details_failed ; sprintf("   TEST : [%s] %s", _params.moduleName, tests(i,2))];
                details_failed = [ details_failed ; sprintf("     %s", result.message) ];
                details_failed = [ details_failed ; result.details ];
                details_failed = [ details_failed ; "" ];
            elseif (result.id >= 10) & (result.id < 20) then
                // skipped
                test_skipped_count = test_skipped_count + 1;
            end  

        end
    end
    elapsedTime = toc();
    status.totalTime = elapsedTime;
    status.test_passed_count  = test_passed_count;
    status.test_failed_count  = test_failed_count;
    status.test_skipped_count = test_skipped_count;

    // Summary
    status.test_count       = test_count;
    status.details_failed   = details_failed;
endfunction

function directories = getDirectories(directory)
    directories = directory;
    items = gsort(listfiles(directory),"lr","i");

    for i=1:size(items,"*")
        if isdir(directory + items(i)) then
            directories = [directories; getDirectories(directory + items(i) + filesep())];
        end
    end
endfunction

function path = getPath(directory, ref_dir)
    path = strsplit(directory, ["\","/"]);
    path(path == "") = [];
    for i = 1:size(ref_dir, "*")
        if find(path == ref_dir(i)) then
            path(1:find(path == ref_dir(i))) = [];
        end
    end
endfunction

function name = splitModule(name)
    if ~isempty( regexp(stripblanks(name),"/\|/") ) then
        name = stripblanks( strsubst( strsplit(name,regexp(stripblanks(name),"/\|/")) , "/\|$/","","r" ) );
    end
endfunction
