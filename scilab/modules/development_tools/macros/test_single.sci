// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//module options structure
//longtime : boolean
//wanted_mode : string ( NWNI, NW, ...)
//error_output : string (skip, create, ...)
//reference : string (check, ...)

function status = test_single(_module, _testPath, _testName)
    //option flag
    skip            = %F;
    interactive     = %F;
    notyetfixed     = %F;
    longtime        = %F;
    reopened        = %F;
    jvm             = %T;
    graphic         = %F;
    execMode        = "";
    platform        = "all";
    language        = "any";
    try_catch       = %T;
    error_output    = "check";
    reference       = "check";
    xcosNeeded      = %F;

    //some paths    
    tmp_tst         = pathconvert( TMPDIR + "/" + _testName + ".tst", %F);    
    tmp_dia         = pathconvert( TMPDIR + "/" + _testName + ".dia.tmp", %F);    
    tmp_res         = pathconvert( TMPDIR + "/" + _testName + ".res", %F);    
    tmp_err         = pathconvert( TMPDIR + "/" + _testName + ".err", %F);    
    path_dia        = pathconvert( TMPDIR + "/" + _testName + ".dia", %F);    

    path_dia_ref    = _testPath + _testName + ".dia.ref";
    // Reference file management OS by OS
    if getos() == 'Windows' then
        altreffile  = [ _testPath + _testName + ".win.dia.ref" ];
    elseif MACOSX then
        altreffile  = [ _testPath + _testName + ".unix.dia.ref" ; _testPath + _testName + ".macosx.dia.ref" ];
    elseif LINUX then
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
        status.message = "skipped : not yet fixed";
        return;
    end

    if ~isempty(grep(sciFile, "<-- REOPENED -->")) then
        status.id = 10;
        status.message = "skipped : Bug reopened";
        return;
    end

    // platform
    // =========================================================================

    if ~isempty(grep(sciFile, "<-- WINDOWS ONLY -->")) & getos() <> "Windows" then
        status.id = 10;
        status.message = "skipped : Windows only";
        return;
    end

    if ~isempty(grep(sciFile, "<-- LINUX ONLY -->")) & getos() <> "Linux" then
        status.id = 10;
        status.message = "skipped : Linux only";
        return;
    end

    if ~isempty(grep(sciFile, "<-- MACOSX ONLY -->")) & getos() <> "Darwin" then
        status.id = 10;
        status.message = "skipped : MacOSX only";
        return;
    end

    // Test execution
    // =========================================================================

    if ~isempty(grep(sciFile, "<-- INTERACTIVE TEST -->")) then
        status.id = 10;
        status.message = "skipped : interactive test";
        return;
    end

    if ~isempty(grep(sciFile, "<-- LONG TIME EXECUTION -->")) & _module.longtime then
        status.id = 10;
        status.message = "skipped : Long time duration";
        return;
    end

    if ~isempty(grep(sciFile, "<-- TEST WITH GRAPHIC -->")) then
        if _module.wanted_mode == "NWNI" then
            status.id = 10;
            status.message = "skipped : Long time duration";
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
    // =========================================================================

    if ~isempty(grep(sciFile, "<-- FRENCH IMPOSED -->")) then
        language = "fr_FR";
    end

    if ~isempty(grep(sciFile, "<-- ENGLISH IMPOSED -->")) then
        language = "en_US";
    end

    // Test building
    // =========================================================================

    if ~isempty(grep(sciFile, "<-- NO TRY CATCH -->")) then
        try_catch = %F;
    end

    // Test result
    // =========================================================================

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

    head = [                                                                    ...
        "// <-- HEADER START -->";                                              ...
        "mode(3);" ;                                                            ...
        "lines(28,72);";                                                        ...
        "lines(0);" ;                                                           ...
        "function %onprompt" ;                                                  ...
        "quit;" ;                                                               ...
        "endfunction" ;                                                         ...
        "deff(''[]=bugmes()'',''write(%io(2),''''error on test'''')'');" ;      ...
        "predef(''all'');" ;                                                    ...
        "tmpdirToPrint = msprintf(''TMPDIR1=''''%s''''\n'',TMPDIR);"            ...
    ]

    if xcosNeeded then
        head = [ head ; "loadXcosLibs();"];
    end

    if try_catch then
        head = [ head ; "try" ];
    end
    
    head = [                                                                    ...
        head ;                                                                  ...
        "diary(''" + tmp_dia + "'');";                                          ...
        "write(%io(2),tmpdirToPrint);";                                         ...
        "// <-- HEADER END -->"                                                 ...
    ];
    
    // Build test footer
    tail = [ "// <-- FOOTER START -->" ];

    if try_catch then
        tail = [ tail;                                                          ...
            "catch";                                                            ...
            "   errmsg = ""<--""+""Error on the test script file""+""-->"";";   ...
            "   printf(""%s\n"",errmsg);";                                      ...
            "   lasterror()";                                                   ...
            "end";                                                              ...
            ];
    end

    tail = [ tail; "diary(0);" ];

    if graphic then
        tail = [ tail; "xdel(winsid());sleep(1000);" ];
    end

    tail = [ tail; "exit;" ; "// <-- FOOTER END -->" ];
 
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
        if mode == "NWNI" then
            mode_arg = "-nwni";
        elseif mode == "NW" then
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
        language_arg = "LANG=" + language + " ; ";
    end

    //Buld final command
    if getos() == 'Windows' then
        test_cmd = "( """ + SCI_BIN + "\bin\scilex.exe" + """" + " " + mode_arg + " " + language_arg + " -nb -f """ + tmp_tst + """ > """ + tmp_res + """ ) 2> """ + tmp_err + """";
    else
        test_cmd = "( " + language_arg + " " + SCI_BIN + "/bin/scilab " + mode_arg + " -nb -f " + tmp_tst + " > " + tmp_res + " ) 2> " + tmp_err;
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
            status.message = "failed  : error_output not empty\n     Use ''no_check_error_output'' option to disable this check.";
            status.details = checkthefile(tmp_err);
            return;
        end
    end

    //Process output files    

    //Get the dia file
    dia = mgetl(tmp_dia);

    // To get TMPDIR value
    tmpdir1_line = grep(dia, "TMPDIR1");
    execstr(dia(tmpdir1_line));

    //Check for execution errors
    if try_catch & grep(dia,"<--Error on the test script file-->") <> [] then
        details = [ checkthefile(tmp_dia); ..
                    launchthecommand(testFile)];
        status.id = 3;
        status.message = "failed  : premature end of the test script";
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
        status.message = "failed  : the string (!--error) has been detected";
        status.details = details;
        return;
    end


    if grep(dia_tmp,"error on test")<>[] then
        details = [ checkthefile(tmp_dia); ..
                    launchthecommand(testFile) ];
        status.id = 2;
        status.message = "failed  : one or several tests failed";
        status.details = details;
        return;
    end


    if tmpdir1_line == [] then
        status.id = 6;
        status.message = "failed  : the dia file is not correct";
        status.details = checkthefile(tmp_dia);
        return;
    end
    

    // Check the reference file only if check_ref (i.e. for the whole
    // test sequence) is true and this_check_ref (i.e. for the specific current .tst)
    // is true.

    if (reference=="check") & (_module.reference=="check")  then
        if isfile(path_dia_ref) == %f then
            status.id = 5;
            status.message = "failed  : the ref file doesn''t exist\n     Use ''no_check_ref'' option to disable this check.";
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
            status.message = "passed : ref created";
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
                    status.message = "failed  : dia and ref are not equal";
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
  //     Check the following file :
  //     - C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.tst
  // Workaround for bug #4827
  msg(1) = "     Check the following file :"
  msg(2) = "     - "+filename
endfunction

// launchthecommand
function msg = launchthecommand( filename )
  // Returns a 2-by-1 matrix of strings, containing a message such as:
  //     Or launch the following command :
  //     - exec("C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.tst")
  // Workaround for bug #4827
  msg(1) = "     Or launch the following command :"
  msg(2) = "     - exec(""" + filename + """);"
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
  //     Add or create the following file :
  //     - C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.dia.ref
  // Workaround for bug #4827
  msg(1) = "     Add or create the following file : "
  msg(2) = "     - "+filename
endfunction

// comparethefiles
function msg = comparethefiles ( filename1 , filename2 )
  // Returns a 3-by-1 matrix of strings, containing a message such as:
  //     Compare the following files :
  //     - C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.dia
  //     - C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.dia.ref
  // Workaround for bug #4827
  msg(1) = "     Compare the following files :"
  msg(2) = "     - "+filename1
  msg(3) = "     - "+filename2
endfunction
