// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - 2011 - INRIA - Allan CORNET
// =============================================================================
function builder_gateway_cpp()

  CFLAGS = '';
  gw_cpp_path = get_absolute_file_path("builder_gateway_cpp.sce");
  src_cpp_path = fullpath(gw_cpp_path + '../../src/cpp');

  // PutLhsVar managed by user
  // if you do not this variable, PutLhsVar is added
  // in gateway generated (default mode in scilab 4.x and 5.x)
  WITHOUT_AUTO_PUTLHSVAR = %t;

  files_gateways = findfiles(gw_cpp_path, '*.cpp');

  scifunctions_name = files_gateways(grep(files_gateways,'sci_'));          
  scifunctions_name = strsubst(scifunctions_name, 'sci_', '');
  scifunctions_name = strsubst(scifunctions_name, 'percent', '%');
  scifunctions_name = strsubst(scifunctions_name, '.cpp', '');
  cppfunctions_name = files_gateways(grep(files_gateways,'sci_'));
  cppfunctions_name = strsubst(cppfunctions_name, '.cpp', '');
  
  gw_tables = [scifunctions_name, cppfunctions_name]; 

  libs_cpp = ['../../src/cpp/libcsv_tools_cpp'];

  if getos() <> 'Windows' then
    files_h = findfiles(gw_cpp_path, '*.h');
    files_gateways = [files_gateways; files_h];
  end

  CFLAGS = CFLAGS + " " + ilib_include_flag([gw_cpp_path, src_cpp_path]);

  tbx_build_gateway("gw_csv_tools", gw_tables, files_gateways, ..
                    get_absolute_file_path('builder_gateway_cpp.sce'), ..
                    libs_cpp, '', CFLAGS);

endfunction
// =============================================================================
builder_gateway_cpp();
clear builder_gateway_cpp;
// =============================================================================

