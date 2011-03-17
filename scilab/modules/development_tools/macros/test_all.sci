// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function test_all(varargin)
    lhs         = argn(1);
    rhs         = argn(2);

    // =========================================================================
    // Print test_run help
    // =========================================================================

    if (rhs >= 3) & (~ isempty(grep(varargin(3),"help"))) then
        example = test_examples();
        printf("%s\n",example);
        return;
    end

    status.details_failed       = "";
    status.test_count           = 0;
    status.test_passed_count    = 0;
    status.test_failed_count    = 0;
    status.test_skipped_count   = 0;
    status.totalTime            = 0;
    status.list                 = [];

    params.longtime             = %t;
    params.wanted_mode          = "NW";
    params.error_output         = "check";
    params.reference            = "check";
    params.testTypes            = "all_tests";
    params.skip_mat             = [];
    params.tests_mat            = [];
    params.test_passed_percent  = 0;
    params.test_failed_percent  = 0;
    params.test_skipped_percent = 0;

    // =======================================================
    // Gestion des types de tests à lancer et des options
    // =======================================================
    if rhs >= 3 then

        option_mat = varargin(3);
        if (or(option_mat == "unit_tests") & or(option_mat == "nonreg_tests")) | (or(option_mat == "all_tests") ) then
            params.testTypes = "all_tests";
        elseif or(option_mat == "unit_tests") then
            params.testTypes = "unit_tests";
        elseif or(option_mat == "nonreg_tests") then
            params.testTypes = "nonreg_tests";
        end

        if or(option_mat == "skip_tests") then
            params.skip_mat = varargin(2);
        end

        // Mode
        if grep(option_mat,"mode_nw") <> [] then
            params.wanted_mode = "NW";
        end

        if grep(option_mat,"mode_nwni") <> [] then
            params.wanted_mode = "NWNI";
        end

        // Reference
        if grep(option_mat,"no_check_ref") <> [] then
            params.reference = "skip";
        end

        if grep(option_mat,"create_ref") <> [] then
            params.reference = "create";
        end

        if grep(option_mat,"list") <> [] then
            params.reference = "list";
        end

        // Error Output
        if grep(option_mat,"no_check_error_output") <> [] then
            params.error_output = "skip";
        end

        // Enable long-time tests
        if grep(option_mat,"disable_lt") <> [] then
            params.longtime = %f;
        end

        if grep(option_mat,"enable_lt") <> [] then
            params.longtime = %t;
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

        //printf("--------------------------------------------------------\n");
        //printf("  for test only modules API_SCILAB and DOUBLE are used  \n");
        //printf("--------------------------------------------------------\n");
        //module_list = ["api_scilab", "double"];


        test_count = size(module_list,"*");
        for i=1:test_count
            if params.reference <> "list" then
                printf("   %03d/%03d - [%s] : \n\n", i, test_count, module_list(i));
            end

            params.tests_mat            = [];
            params.moduleName           =  module_list(i);
            
            result                      = test_module(params);

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

            params.tests_mat            = [];
            params.moduleName           =  module_mat(i);
            
            result                      = test_module(params);

            status.test_count           = status.test_count + result.test_count;
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

        params.tests_mat            = varargin(2);
        params.moduleName           = varargin(1);

        if ((or(size(params.moduleName) <> [1,1])) & (params.tests_mat <> [])) then
            example = test_examples();
            err     = ["" ; msprintf(gettext("%s: Wrong size for input argument."),"test_run") ; "" ; example ];
            printf("%s\n",err);
            return;
        end

        result                      = test_module(params);
    
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
    printf("   tests                     %4d - 100 %% \n", status.test_count);
    printf("   passed                    %4d - %3d %% \n", status.test_passed_count , test_passed_percent);
    printf("   failed                    %4d - %3d %% \n", status.test_failed_count , test_failed_percent);
    printf("   skipped                   %4d - %3d %% \n", status.test_skipped_count, test_skipped_percent);
    printf("   length                          %4.2f sec \n", status.totalTime);
    printf("   --------------------------------------------------------------------------\n");

    if status.test_failed_count > 0 then
        printf("   Details\n\n");
        printf("%s\n",status.details_failed);
        printf("\n");
        printf("   --------------------------------------------------------------------------\n");
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

