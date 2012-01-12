// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2009-2011 - DIGITEO - Michael Baudin
// Copyright (C) 2011-2010 - DIGITEO - Antoine ELIAS
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// test_run  --
//   Launch unit tests.

function test_run(varargin)
    function status = test_module(_params)
        name = splitModule(_params.moduleName);
        if with_module(name(1)) then
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
                    directory_path = directory_path + filesep() + name(j);
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
                bFind = %f;
                for j = 1:size(directories, "*")
                    currentDir = directories(j);
                    testFile = currentDir + filesep() + _params.tests_mat(i) + ".tst";
                    if isfile(testFile) then
                        tests($+1, [1,2]) = [currentDir, _params.tests_mat(i)];
                        bFind = %t;
                    end
                end

                if bFind == %f then
                    error(sprintf(gettext("The test ""%s"" is not available from the ""%s"" module"), _params.tests_mat(i), name(1)));
                end
            end
        end

        //initialize counter
        details_failed      = "";
        test_count          = size(tests, "r");
        test_passed_count   = 0;
        test_failed_count   = 0;
        test_skipped_count  = 0;

        moduleName          = _params.moduleName;
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
                if size(name, "*") > 1 then
                    displayModuleName = sprintf("%s", name(1));
                    for j=2:size(name, "*")
                        displayModuleName = displayModuleName + sprintf("|%s", name(j));
                    end
                else
                    displayModuleName = sprintf("%s", name(1));
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

            if size(name, "*") > 1 then
                displayModuleName = sprintf("[%s", name(1));
                for j=2:size(name, "*")
                    displayModuleName = displayModuleName + sprintf("|%s", name(j));
                end
                displayModuleName = displayModuleName + sprintf("] %s", tests(i,2));
            else
                displayModuleName = sprintf("[%s] %s", name(1), tests(i,2));
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

                if result.id < 10 then
                    //failed
                    test_failed_count = test_failed_count + 1;
                    details_failed = [ details_failed ; sprintf("   TEST : [%s] %s", _params.moduleName, tests(i,2))];
                    details_failed = [ details_failed ; sprintf("   %s", result.message) ];
                    details_failed = [ details_failed ; result.details ];
                    details_failed = [ details_failed ; "" ];
                elseif (result.id >= 10) & (result.id < 20) then
                    // skipped
                    test_skipped_count = test_skipped_count + 1;
                end
            end
        end

        status.totalTime = toc();
        clearglobal TICTOC;
        status.test_passed_count  = test_passed_count;
        status.test_failed_count  = test_failed_count;
        status.test_skipped_count = test_skipped_count;

        // Summary
        status.test_count     = test_count;
        status.details_failed   = details_failed;
    endfunction

    function status = test_single(_module, _testPath, _testName)
        //option flag

        skip          = %F;
        interactive   = %F;
        notyetfixed   = %F;
        longtime      = %F;
        reopened      = %F;
        jvm           = %T;
        graphic       = %F;
        execMode      = "";
        platform      = "all";
        language      = "any";
        try_catch     = %T;
        error_output  = "check";
        reference     = "check";
        xcosNeeded    = %F;

        //some paths
        tmp_tst     = pathconvert( TMPDIR + "/" + _testName + ".tst", %F);
        tmp_dia     = pathconvert( TMPDIR + "/" + _testName + ".dia.tmp", %F);
        tmp_res     = pathconvert( TMPDIR + "/" + _testName + ".res", %F);
        tmp_err     = pathconvert( TMPDIR + "/" + _testName + ".err", %F);
        path_dia    = pathconvert( TMPDIR + "/" + _testName + ".dia", %F);

        path_dia_ref  = _testPath + _testName + ".dia.ref";
        // Reference file management OS by OS
        if getos() == 'Windows' then
            altreffile  = [ _testPath + _testName + ".win.dia.ref" ];
        elseif getos() == 'Darwin' then
            altreffile  = [ _testPath + _testName + ".unix.dia.ref" ; _testPath + _testName + ".macosx.dia.ref" ];
        elseif getos() == 'Linux' then
            altreffile  = [ _testPath + _testName + ".unix.dia.ref" ; _testPath + _testName + ".linux.dia.ref" ];
        else
            altreffile  = [ _testPath + _testName + ".unix.dia.ref" ];
        end

        for i=1:size(altreffile,"*")
            if isfile(altreffile(i)) then
                path_dia_ref = altreffile(i);
            end
        end

        //output structure
        status.id = 0;
        status.message = "";
        status.details = "";

        //Reset standard globals
        rand("seed",0);
        rand("uniform");

        //load file
        testFile = _testPath + _testName + ".tst";
        sciFile = mgetl(testFile);

        //read options
        if ~isempty(grep(sciFile, "<-- NOT FIXED -->")) then
            status.id = 10;
            status.message = "skipped: not yet fixed";
            return;
        end

        if ~isempty(grep(sciFile, "<-- REOPENED -->")) then
            status.id = 10;
            status.message = "skipped: Bug reopened";
            return;
        end

        // platform
        if ~isempty(grep(sciFile, "<-- WINDOWS ONLY -->")) & getos() <> "Windows" then
            status.id = 10;
            status.message = "skipped: Windows only";
            return;
        end

        if ~isempty(grep(sciFile, "<-- LINUX ONLY -->")) & getos() <> "Linux" then
            status.id = 10;
            status.message = "skipped: Linux only";
            return;
        end

        if ~isempty(grep(sciFile, "<-- MACOSX ONLY -->")) & getos() <> "Darwin" then
            status.id = 10;
            status.message = "skipped: MacOSX only";
            return;
        end

        // Test execution
        if ~isempty(grep(sciFile, "<-- INTERACTIVE TEST -->")) then
            status.id = 10;
            status.message = "skipped: interactive test";
            return;
        end

        if ~isempty(grep(sciFile, "<-- LONG TIME EXECUTION -->")) & ~_module.longtime then
            status.id = 10;
            status.message = "skipped: Long time duration";
            return;
        end

        if ~isempty(grep(sciFile, "<-- TEST WITH GRAPHIC -->")) then
            if _module.wanted_mode == "NWNI" then
                status.id = 10;
                status.message = "skipped: Test with graphic";
                return;
            end

            graphic = %T;
            jvm = %T;
            execMode = "NW";
        end

        if ~isempty(grep(sciFile, "<-- JVM NOT MANDATORY -->")) then
            jvm = %F;
            execMode = "NWNI";
        end

        if ~isempty(grep(sciFile, "<-- XCOS TEST -->")) then
            xcosNeeded = %T;
            jvm = %T;
        end

        // Language
        if ~isempty(grep(sciFile, "<-- FRENCH IMPOSED -->")) then
            language = "fr_FR";
        end


        if ~isempty(grep(sciFile, "<-- ENGLISH IMPOSED -->")) then
            language = "en_US";
        end

        // Test building
        if ~isempty(grep(sciFile, "<-- NO TRY CATCH -->")) then
            try_catch = %F;
        end

        // Test result
        if ~isempty(grep(sciFile, "<-- NO CHECK ERROR OUTPUT -->")) then
            error_output = "skip";
        end

        if ~isempty(grep(sciFile, "<-- NO CHECK REF -->")) then
            reference = "skip";
        end

        //build real test file

        // Do some modification in tst file
        //replace "pause,end" by "bugmes();quit;end"
        sciFile = strsubst(sciFile, "pause,end", "bugmes();quit;end");
        sciFile = strsubst(sciFile, "pause, end", "bugmes();quit;end");
        sciFile = strsubst(sciFile, "pause;end", "bugmes();quit;end");
        sciFile = strsubst(sciFile, "pause; end", "bugmes();quit;end");

        //to avoid suppression of input --> with prompts
        sciFile = strsubst(sciFile, "-->", "@#>");
        //remove halt calls
        sciFile = strsubst(sciFile, "halt();", "");

        // Build test header
        head = [
            "// <-- HEADER START -->";
            "mode(3);" ;
            "lines(28,72);";
            "lines(0);" ;
            "function %onprompt" ;
            "   quit;" ;
            "endfunction" ;
            "deff(''[]=bugmes()'',''write(%io(2),''''error on test'''')'');";
            "predef(''all'');" ;
            "tmpdirToPrint = msprintf(''TMPDIR1=''''%s''''\n'',TMPDIR);"
        ]

        if xcosNeeded then
            head = [ head ; "loadXcosLibs();"];
        end

        if try_catch then
            head = [ head ; "try" ];
        end

        head = [
            head;
            "diary(''" + tmp_dia + "'');";
            "write(%io(2),tmpdirToPrint);";
            "// <-- HEADER END -->"
        ];

        // Build test footer
        tail = [ "// <-- FOOTER START -->" ];

        if try_catch then
            tail = [ 
                tail;
                "catch";
                "   errmsg = ""<--""+""Error on the test script file""+""-->"";";
                "   printf(""%s\n"",errmsg);";
                "   lasterror()";
                "end";
            ];
        end

        tail = [ tail; "diary(0);" ];

        if graphic then
            tail = [ tail; "xdel(winsid());sleep(1000);" ];
        end

        tail = [ tail; "exit(0);" ; "// <-- FOOTER END -->" ];

        //Build final test
        sciFile = [head ; sciFile ; tail];


        //Build command to execute

        //scilab path
        if (getos() <> 'Windows') & ~isfile(SCI+"/bin/scilab") then
            SCI_BIN = strsubst(SCI,'share/scilab','');
        else
            SCI_BIN = SCI;
        end

        //mode
        if _module.wanted_mode == "NW" then
            mode_arg = "-nw";
            elseif _module.wanted_mode == "NWNI" then
            mode_arg = "-nwni";
        else
            if execMode == "NWNI" then
                mode_arg = "-nwni";
                elseif execMode == "NW" then
                mode_arg = "-nw";
            else
                mode_arg = "-nw";
            end
        end

        //language
        if language == "any" then
            language_arg = "";
        elseif getos() == 'Windows' then
            language_arg = "-l "+ language;
        else
            language_arg = "LANG=" + language + " ";
        end
 
        loader_path = pathconvert(fullfile(_module.moduleName, 'loader.sce'), %f);

        // Build final command
        if getos() == 'Windows' then
            if (isdir(_module.moduleName) & isfile(loader_path)) // external module not in Scilab
                test_cmd = "( """ + SCI_BIN + "\bin\scilex.exe" + """" + " " + mode_arg + " " + language_arg + " -nb -e ""exec(""""" + loader_path + """"");exec(""""" + tmp_tst + """"");"" > """ + tmp_res + """ ) 2> """ + tmp_err + """";
            else // standard module
                test_cmd = "( """ + SCI_BIN + "\bin\scilex.exe" + """" + " " + mode_arg + " " + language_arg + " -nb -f """ + tmp_tst + """ > """ + tmp_res + """ ) 2> """ + tmp_err + """";
            end
        else
            if (isdir(_module.moduleName) & isfile(loader_path))
                test_cmd = "( " + language_arg + " " + SCI_BIN + "/bin/scilab " + mode_arg + " -nb -e ""exec(''" + loader_path + "'');exec(''" + tmp_tst +"'');""" + " > " + tmp_res + " ) 2> " + tmp_err;
            else
                test_cmd = "( " + language_arg + " " + SCI_BIN + "/bin/scilab " + mode_arg + " -nb -f " + tmp_tst + " > " + tmp_res + " ) 2> " + tmp_err;
            end
        end

        //clean previous tmp files
        if isfile(tmp_tst) then
            deletefile(tmp_tst);
        end

        if isfile(tmp_dia) then
            deletefile(tmp_dia);
        end

        if isfile(tmp_res) then
            deletefile(tmp_res);
        end

        if isfile(tmp_err) then
            deletefile(tmp_err);
        end

        //create tmp test file
        mputl(sciFile, tmp_tst);

        //execute test
        host(test_cmd);

        //Check errors
        if (error_output == "check") & (_module.error_output == "check") then
            if getos() == "Darwin" then
                tmp_errfile_info = fileinfo(tmp_err);
                msg = "JavaVM: requested Java version (1.5) not available. Using Java at ""/System/Library/Java/JavaVirtualMachines/1.6.0.jdk/Contents/Home"" instead."

                if ~isempty(tmp_errfile_info) then
                    txt = mgetl(tmp_err);
                    txt(txt==msg) = [];
                    if isempty(txt) then
                        deletefile(tmp_err);
                    end
                end
            end

            tmp_errfile_info = fileinfo(tmp_err);

            if isfile(tmp_err) & tmp_errfile_info(1) <> 0 then
                status.id = 5;
                status.message = "failed: error_output not empty\n   Use ''no_check_error_output'' option to disable this check.";
                status.details = checkthefile(tmp_err);
                return;
            end
        end

        //Process output files

        //Get the dia file
        if isfile(tmp_dia) then
            dia = mgetl(tmp_dia);
        else
            status.id = 6;
            status.message = "failed: the dia file is not correct";
            status.details = checkthefile(tmp_dia);
            return;
        end

        // To get TMPDIR value
        tmpdir1_line = grep(dia, "TMPDIR1");
        execstr(dia(tmpdir1_line));

        //Check for execution errors
        if try_catch & grep(dia,"<--Error on the test script file-->") <> [] then
            details = [ checkthefile(tmp_dia); ..
            launchthecommand(testFile)];
            status.id = 3;
            status.message = "failed: premature end of the test script";
            status.details = details;
            return;
        end

        // Remove Header and Footer
        dia = remove_headers(dia);

        //Check for execution errors
        dia_tmp = dia;

        // remove commented lines
        dia_tmp(grep(dia_tmp, "//")) = [];

        if try_catch & grep(dia_tmp, "!--error") <> [] then
            details = [ checkthefile(tmp_dia); ..
            launchthecommand(testFile) ];
            status.id = 1;
            status.message = "failed: the string (!--error) has been detected";
            status.details = details;
            return;
        end


        if grep(dia_tmp,"error on test")<>[] then
            details = [ checkthefile(tmp_dia); ..
            launchthecommand(testFile) ];
            status.id = 2;
            status.message = "failed: one or several tests failed";
            status.details = details;
            return;
        end


        if tmpdir1_line == [] then
            status.id = 6;
            status.message = "failed: the dia file is not correct";
            status.details = checkthefile(tmp_dia);
            return;
        end


        // Check the reference file only if check_ref (i.e. for the whole
        // test sequence) is true and this_check_ref (i.e. for the specific current .tst)
        // is true.

        if (reference=="check") & (_module.reference=="check")  then
            if isfile(path_dia_ref) == %f then
                status.id = 5;
                status.message = "failed: the ref file doesn''t exist\n   Use ''no_check_ref'' option to disable this check.";
                status.details = createthefile(path_dia_ref);
                return;
            end
        end

        // Comparaison ref <--> dia

        if ( (reference=="check") & (_module.reference=="check") ) | (_module.reference=="create") then
            //  Do some modification in  dia file

            dia(grep(dia, "write(%io(2), tmpdirToPrint")) = [];
            dia(grep(dia, "TMPDIR1")) = [];
            dia(grep(dia, "diary(0)")) = [];

            dia = strsubst(dia,TMPDIR ,"TMPDIR");
            dia = strsubst(dia,TMPDIR1, "TMPDIR");

            if getos() == 'Windows' then
                dia = strsubst(dia, strsubst(TMPDIR, "\","/"), "TMPDIR");
                dia = strsubst(dia, strsubst(TMPDIR1, "\","/"), "TMPDIR");
                dia = strsubst(dia, strsubst(TMPDIR, "/","\"), "TMPDIR");
                dia = strsubst(dia, strsubst(TMPDIR1, "/","\"), "TMPDIR");
                dia = strsubst(dia, strsubst(getshortpathname(TMPDIR), "\","/"), "TMPDIR");
                dia = strsubst(dia, strsubst(getshortpathname(TMPDIR1), "\","/"), "TMPDIR");
                dia = strsubst(dia, getshortpathname(TMPDIR), "TMPDIR");
                dia = strsubst(dia, getshortpathname(TMPDIR1), "TMPDIR");
            end

            dia = strsubst(dia, SCI, "SCI");

            if getos() == 'Windows' then
                dia = strsubst(dia, strsubst(SCI, "\","/"), "SCI");
                dia = strsubst(dia, strsubst(SCI, "/","\"), "SCI");
                dia = strsubst(dia, strsubst(getshortpathname(SCI), "\","/"), "SCI");
                dia = strsubst(dia, getshortpathname(SCI), "SCI");
            end

            //suppress the prompts
            dia = strsubst(dia, "-->", "");
            dia = strsubst(dia, "@#>", "-->");
            dia = strsubst(dia, "-1->", "");

            //standardise  number display

            // strsubst(dia, " .", "0.");
            // strsubst(dia, "-.", "-0.")
            // strsubst(dia, "E+", "D+");
            // strsubst(dia, "E-", "D-");

            //not to change the ref files
            dia = strsubst(dia ,"bugmes();return", "bugmes();quit");

            if _module.reference=="create" then
                // Delete previous .dia.ref file
                if isfile(path_dia_ref) then
                    deletefile(path_dia_ref)
                end

                mputl(dia, path_dia_ref);
                status.id = 20;
                status.message = "passed: ref created";
                return;
            else
            // write down the resulting dia file
                mputl(dia, path_dia);

                //Check for diff with the .ref file
                [u,ierr] = mopen(path_dia_ref, "r");
                if ierr== 0 then //ref file exists
                    ref=mgetl(u);
                    mclose(u)

                    // suppress blank (diff -nw)
                    dia = strsubst(dia, ' ', '')
                    ref = strsubst(ref, ' ', '')

                    dia(find(dia == '')) = [];
                    ref(find(ref == '')) = [];

                    dia(find(dia == '')) = [];
                    ref(find(ref == '')) = [];

                    dia(find(part(dia, (1:2)) == "//")) = [];
                    ref(find(part(ref, (1:2)) == "//")) = [];

                    if or(ref <> dia) then
                        status.id = 4;
                        status.message = "failed: dia and ref are not equal";
                        status.details = comparethefiles(path_dia, path_dia_ref);
                        return;
                    end

                else
                    error(sprintf(gettext("The ref file (%s) doesn''t exist"), path_dia_ref));
                end
            end
        end
    endfunction

    // checkthefile
    function msg = checkthefile( filename )
        // Returns a 2-by-1 matrix of strings, containing a message such as:
        //   Check the following file :
        //   - C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.tst
        // Workaround for bug #4827
        msg(1) = "   Check the following file :"
        msg(2) = "   - "+filename
    endfunction

    // launchthecommand
    function msg = launchthecommand( filename )
        // Returns a 2-by-1 matrix of strings, containing a message such as:
        //   Or launch the following command :
        //   - exec("C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.tst")
        // Workaround for bug #4827
        msg(1) = "   Or launch the following command :"
        msg(2) = "   - exec(""" + filename + """);"
    endfunction

    // => remove header from the diary txt
    function dia_out = remove_headers(dia_in)
        dia_out = dia_in;
        body_start = grep(dia_out,"// <-- HEADER END -->");
        if body_start <> [] then
            dia_out(1:body_start(1)) = [];
        end

        body_end   = grep(dia_out,"// <-- FOOTER START -->");
        if body_end <> [] then
            [dia_nl,dia_nc] = size(dia);
            dia_out(body_end(1):dia_nl) = [];
        end
    endfunction

    //createthefile
    function msg = createthefile ( filename )
        // Returns a 2-by-1 matrix of strings, containing a message such as:
        //   Add or create the following file :
        //   - C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.dia.ref
        // Workaround for bug #4827
        msg(1) = "   Add or create the following file : "
        msg(2) = "   - "+filename
    endfunction

    // comparethefiles
    function msg = comparethefiles ( filename1 , filename2 )
        // Returns a 3-by-1 matrix of strings, containing a message such as:
        //   Compare the following files :
        //   - C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.dia
        //   - C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.dia.ref
        // Workaround for bug #4827
        msg(1) = "   Compare the following files :"
        msg(2) = "   - "+filename1
        msg(3) = "   - "+filename2
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

    function name = splitModule(name)
    if ~isempty( regexp(stripblanks(name),"/\|/") ) then
        name = stripblanks( strsubst( strsplit(name,regexp(stripblanks(name),"/\|/")) , "/\|$/","","r" ) );
    end
    endfunction

    function example = test_examples()
        example = [ sprintf("Examples :\n\n") ];
        example = [ example ; sprintf("// Launch all tests\n") ];
        example = [ example ; sprintf("test_run();\n") ];
        example = [ example ; sprintf("test_run([]);\n") ];
        example = [ example ; sprintf("test_run([],[]);\n") ];
        example = [ example ; "" ];
        example = [ example ; sprintf("// Test one or several module\n") ];
        example = [ example ; sprintf("test_run(''core'');\n") ];
        example = [ example ; sprintf("test_run(''core'',[]);\n") ];
        example = [ example ; sprintf("test_run([''core'',''string'']);\n") ];
        example = [ example ; "" ];
        example = [ example ; sprintf("// Launch one or several test in a specified module\n") ];
        example = [ example ; sprintf("test_run(''core'',[''trycatch'',''opcode'']);\n") ];
        example = [ example ; "" ];
        example = [ example ; sprintf("// With options\n") ];
        example = [ example ; sprintf("test_run([],[],''no_check_ref'');\n") ];
        example = [ example ; sprintf("test_run([],[],''no_check_error_output'');\n") ];
        example = [ example ; sprintf("test_run([],[],''create_ref'');\n") ];
        example = [ example ; sprintf("test_run([],[],''list'');\n") ];
        example = [ example ; sprintf("test_run([],[],''help'');\n") ];
        example = [ example ; sprintf("test_run([],[],[''no_check_ref'',''mode_nw'']);\n") ];
        example = [ example ; "" ];
    endfunction

    function newOption = clean_option(var, option)
        newOption = var;
        newOption(newOption == option) = [];
    endfunction

    function result = check_option(var, option)
        result = or(var == option);
    endfunction

    function value = assign_option(var, option, truevalue, falsevalue)
        if check_option(var, option) then
            value = truevalue;
        else
            value = falsevalue;
        end
    endfunction

    lhs     = argn(1);
    rhs     = argn(2);

    // =========================================================================
    // Print test_run help
    // =========================================================================
    if (rhs >= 3) & (~ isempty(grep(varargin(3),"help"))) then
        example = test_examples();
        printf("%s\n",example);
        return;
    end

    status.details_failed         = "";
    status.test_count             = 0;
    status.test_passed_count      = 0;
    status.test_failed_count      = 0;
    status.test_skipped_count     = 0;
    status.totalTime              = 0;
    status.list                   = [];

    params.longtime               = %f;
    params.wanted_mode            = "";
    params.error_output           = "check";
    params.reference              = "check";
    params.testTypes              = "all_tests";
    params.skip_mat               = [];
    params.tests_mat              = [];
    params.test_passed_percent    = 0;
    params.test_failed_percent    = 0;
    params.test_skipped_percent   = 0;

    // =======================================================
    // Gestion des types de tests à lancer et des options
    // =======================================================
    if rhs >= 3 then

        option_mat = varargin(3);
        if (check_option(option_mat, "unit_tests") & check_option(option_mat, "nonreg_tests")) | check_option(option_mat, "all_tests") then
            params.testTypes = "all_tests";
        elseif check_option(option_mat, "unit_tests") then
            params.testTypes = "unit_tests";
        elseif check_option(option_mat, "nonreg_tests") then
            params.testTypes = "nonreg_tests";
        end

        clean_option(option_mat, "unit_tests");
        clean_option(option_mat, "nonreg_tests");
        clean_option(option_mat, "all_tests");

        // Skip tests
        params.skip_mat     = assign_option(option_mat, "skip_tests", varargin(2), params.skip_mat);
        option_mat          = clean_option(option_mat, "skip_tests");

        // Mode
        params.wanted_mode  = assign_option(option_mat, "mode_nw", "NW", params.wanted_mode);
        option_mat          = clean_option(option_mat, "mode_nw");

        params.wanted_mode  = assign_option(option_mat, "mode_nwni", "NWNI", params.wanted_mode);
        option_mat          = clean_option(option_mat, "mode_nwni");

        // Reference
        params.reference    = assign_option(option_mat, "no_check_ref", "skip", params.reference);
        option_mat          = clean_option(option_mat, "no_check_ref");

        params.reference    = assign_option(option_mat, "create_ref", "create", params.reference);
        option_mat          = clean_option(option_mat, "create_ref");

        params.reference    = assign_option(option_mat, "list", "list", params.reference);
        option_mat          = clean_option(option_mat, "list");

        // Error Output
        params.error_output = assign_option(option_mat, "no_check_error_output", "skip", params.error_output);
        option_mat          = clean_option(option_mat, "no_check_error_output");

        // Enable long-time tests
        params.longtime     = assign_option(option_mat, "disable_lt", %f, params.longtime);
        option_mat          = clean_option(option_mat, "disable_lt");

        params.longtime     = assign_option(option_mat, "enable_lt", %t, params.longtime);
        option_mat          = clean_option(option_mat, "enable_lt");

        if option_mat <> [] then
            printf("\nUnrecognized option(s): \n\n");
            for i=1:size(option_mat, "*")
                printf("  - ""%s""\n", option_mat(i));
            end
            return;
        end
    end


    if params.reference <> "list"
        printf("   TMPDIR = %s\n",TMPDIR);
        printf("\n");
    end
    // =======================================================
    // Gestion des tests à lancer
    // =======================================================
    if (rhs == 0) ..
    | ((rhs == 1) & (varargin(1)==[])) ..
    | (((rhs == 2)|(rhs == 3)) & (varargin(1)==[]) & (varargin(2)==[])) then

        // No input argument
        // test_run()
        // test_run([])
        // => Launch each test of each module

        module_list = getmodules();
        module_list = gsort(module_list,"lr","i");

        test_count = size(module_list,"*");
        for i=1:test_count
            if params.reference <> "list" then
                printf("   %03d/%03d - [%s] : \n\n", i, test_count, module_list(i));
            end

            params.tests_mat    = [];
            params.moduleName   =  module_list(i);

            result              = test_module(params);

            if params.reference <> "list" then
                status.details_failed       = [status.details_failed; result.details_failed];
                status.test_count           = status.test_count + result.test_count;
                status.test_passed_count    = status.test_passed_count + result.test_passed_count;
                status.test_failed_count    = status.test_failed_count + result.test_failed_count;
                status.test_skipped_count   = status.test_skipped_count + result.test_skipped_count;
                status.totalTime            = status.totalTime + result.totalTime;
                printf("\n");
            else
                status.test_count           = status.test_count + result.test_count;
                status.list                 = [status.list; result.list];
            end
        end

    elseif (rhs == 1) ..
    | ((rhs == 2) & (varargin(2)==[])) ..
    | ((rhs == 3) & (varargin(2)==[])) ..
    | ( ~ isempty(params.skip_mat)) then

        // One input argument
        // test_run(<module_name>)
        // test_run([<module_name_1>,<module_name_2>])
        // varargin(1) = [<module_name_1>,<module_name_2>]

        module_mat = varargin(1);

        test_count = size(module_mat,"*");
        for i = 1:test_count
            if params.reference <> "list" then
                printf("   %03d/%03d - [%s] : \n\n", i, test_count, module_mat(i));
            end

            params.tests_mat    = [];
            params.moduleName   =  module_mat(i);

            result              = test_module(params);

            status.test_count   = status.test_count + result.test_count;
            if params.reference <> "list" then
                status.details_failed       = [status.details_failed; result.details_failed];
                status.test_passed_count    = status.test_passed_count + result.test_passed_count;
                status.test_failed_count    = status.test_failed_count + result.test_failed_count;
                status.test_skipped_count   = status.test_skipped_count + result.test_skipped_count;
                status.totalTime            = status.totalTime + result.totalTime;
                printf("\n");
            else
                status.test_count           = status.test_count + result.test_count;
                status.list                 = [status.list; result.list];
            end
        end
    elseif or(rhs==[2 3]) then
        // Two input arguments
        // test_run(<module_name>,<test_name>)
        // test_run(<module_name>,[<test_name_1>,<test_name_2>] )

        // varargin(1) = <module_name> ==> string 1x1
        // varargin(2) = <test_name_1> ==> mat nl x nc

        params.tests_mat    = varargin(2);
        params.moduleName   = varargin(1);

        if ((or(size(params.moduleName) <> [1,1])) & (params.tests_mat <> [])) then
            example = test_examples();
            err   = ["" ; msprintf(gettext("%s: Wrong size for input argument."),"test_run") ; "" ; example ];
            printf("%s\n",err);
            return;
        end

        result = test_module(params);

        if params.reference <> "list" then
            status.totalTime            = result.totalTime;
            status.details_failed       = [status.details_failed; result.details_failed];
            status.test_count           = status.test_count + result.test_count;
            status.test_passed_count    = status.test_passed_count + result.test_passed_count;
            status.test_failed_count    = status.test_failed_count + result.test_failed_count;
            status.test_skipped_count   = status.test_skipped_count + result.test_skipped_count;
        else
            status.test_count           = status.test_count + result.test_count;
            status.list                 = [status.list; result.list];
        end
    else
        error(msprintf(gettext('%s: Wrong number of input arguments.'),"test_run"));
    end

    if params.reference == "list" then
        test_count = size(status.list, "r");
        for i=1:test_count
            printf("   %4.d - [%s] %s\n", i, status.list(i, 1), status.list(i,2));
        end
        return;
    end

    //pourcent computation
    if status.test_count <> 0 then
        test_passed_percent  = status.test_passed_count  / status.test_count * 100;
        test_skipped_percent = status.test_skipped_count / status.test_count * 100;
        test_failed_percent  = status.test_failed_count  / status.test_count * 100;
    else
        test_passed_percent  = 0;
        test_skipped_percent = 0;
        test_failed_percent  = 0;
    end

    printf("\n");
    printf("   --------------------------------------------------------------------------\n");
    printf("   Summary\n\n");
    printf("   tests           %4d - 100 %% \n", status.test_count);
    printf("   passed          %4d - %3d %% \n", status.test_passed_count , test_passed_percent);
    printf("   failed          %4d - %3d %% \n", status.test_failed_count , test_failed_percent);
    printf("   skipped           %4d - %3d %% \n", status.test_skipped_count, test_skipped_percent);
    printf("   length              %4.2f sec \n", status.totalTime);
    printf("   --------------------------------------------------------------------------\n");

    if status.test_failed_count > 0 then
        printf("   Details\n\n");
        printf("%s\n",status.details_failed);
        printf("\n");
        printf("   --------------------------------------------------------------------------\n");
    end
endfunction

