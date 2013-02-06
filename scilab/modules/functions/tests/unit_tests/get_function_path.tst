// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for get_function_path() function
// =============================================================================

assert_checkequal(get_function_path('eigs'),SCI+'/modules/arnoldi/macros/eigs.sci');
assert_checkequal(get_function_path('atomsAutoload'),SCI+'/modules/atoms/macros/atomsAutoload.sci');
assert_checkequal(get_function_path('abcd'),SCI+'/modules/cacsd/macros/abcd.sci');
assert_checkequal(get_function_path('%b_a_b'),SCI+'/modules/compatibility_functions/macros/%b_a_b.sci');
assert_checkequal(get_function_path('check_modules_xml'),SCI+'/modules/core/macros/check_modules_xml.sci');
assert_checkequal(get_function_path('cell'),SCI+'/modules/data_structures/macros/cell.sci');
assert_checkequal(get_function_path('delete_frame'),SCI+'/modules/demo_tools/macros/delete_frame.sci');
assert_checkequal(get_function_path('bench_run'),SCI+'/modules/development_tools/macros/bench_run.sci');
assert_checkequal(get_function_path('dae'),SCI+'/modules/differential_equations/macros/dae.sci');
assert_checkequal(get_function_path('configure_msifort'),SCI+'/modules/dynamic_link/macros/configure_msifort.sci');
assert_checkequal(get_function_path('acosd'),SCI+'/modules/elementary_functions/macros/acosd.sci');
assert_checkequal(get_function_path('!_deff_wrapper'),SCI+'/modules/external_objects/macros/!_deff_wrapper.sci');
assert_checkequal(get_function_path('dir'),SCI+'/modules/fileio/macros/dir.sci');
assert_checkequal(get_function_path('get_function_path'),SCI+'/modules/functions/macros/get_function_path.sci');
assert_checkequal(get_function_path('coding_ga_binary'),SCI+'/modules/genetic_algorithms/macros/coding_ga_binary.sci');
assert_checkequal(get_function_path('add_help_chapter'),SCI+'/modules/helptools/macros/add_help_chapter.sci');
assert_checkequal(get_function_path('%i_1_s'),SCI+'/modules/integer/macros/%i_1_s.sci');
assert_checkequal(get_function_path('interp1'),SCI+'/modules/interpolation/macros/interp1.sci');
assert_checkequal(get_function_path('getscilabkeywords'),SCI+'/modules/io/macros/getscilabkeywords.sci');
assert_checkequal(get_function_path('classmarkov'),SCI+'/modules/linear_algebra/macros/classmarkov.sci');
assert_checkequal(get_function_path('cb_m2sci_gui'),SCI+'/modules/m2sci/macros/cb_m2sci_gui.sci');
assert_checkequal(get_function_path('loadmatfile'),SCI+'/modules/matio/macros/loadmatfile.sci');
assert_checkequal(get_function_path('tbx_build_blocks'),SCI+'/modules/modules_manager/macros/tbx_build_blocks.sci');
assert_checkequal(get_function_path('aplat'),SCI+'/modules/optimization/macros/aplat.sci');
assert_checkequal(get_function_path('prettyprint'),SCI+'/modules/output_stream/macros/prettyprint.sci');
assert_checkequal(get_function_path('%0_i_st'),SCI+'/modules/overloading/macros/%0_i_st.sci');
assert_checkequal(get_function_path('get_param'),SCI+'/modules/parameters/macros/get_param.sci');
assert_checkequal(get_function_path('cmndred'),SCI+'/modules/polynomials/macros/cmndred.sci');
assert_checkequal(get_function_path('edit_error'),SCI+'/modules/scinotes/macros/edit_error.sci');
assert_checkequal(get_function_path('analpf'),SCI+'/modules/signal_processing/macros/analpf.sci');
assert_checkequal(get_function_path('accept_func_default'),SCI+'/modules/simulated_annealing/macros/accept_func_default.sci');
assert_checkequal(get_function_path('analyze'),SCI+'/modules/sound/macros/analyze.sci');
assert_checkequal(get_function_path('adj2sp'),SCI+'/modules/sparse/macros/adj2sp.sci');
assert_checkequal(get_function_path('erfinv'),SCI+'/modules/special_functions/macros/erfinv.sci');
assert_checkequal(get_function_path('readxls'),SCI+'/modules/spreadsheet/macros/readxls.sci');
assert_checkequal(get_function_path('median'),SCI+'/modules/statistics/macros/median.sci');
assert_checkequal(get_function_path('arobasestring2strings'),SCI+'/modules/string/macros/arobasestring2strings.sci');
assert_checkequal(get_function_path('calendar'),SCI+'/modules/time/macros/calendar.sci');
assert_checkequal(get_function_path('ana_style'),SCI+'/modules/umfpack/macros/ana_style.sci');
assert_checkequal(get_function_path('%diagram_xcos'),SCI+'/modules/xcos/macros/%diagram_xcos.sci');

assert_checkequal(get_function_path('eigs.sci'),[]);
assert_checkfalse(execstr("get_function_path()","errcatch")==0);
assert_checkfalse(execstr("get_function_path(eigs)","errcatch")==0);
