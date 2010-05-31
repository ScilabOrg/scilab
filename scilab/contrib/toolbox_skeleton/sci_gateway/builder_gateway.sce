// ====================================================================
// Allan CORNET
// Simon LIPP
// INRIA 2008
// This file is released into the public domain
// ====================================================================
sci_gateway_dir = get_absolute_file_path('builder_gateway.sce');

tbx_builder_gateway_lang('fortran', sci_gateway_dir);
tbx_builder_gateway_lang('c', sci_gateway_dir);

// LCC does not manage C++
if isdef('with_lcc') & with_lcc() then
  languages = ['c', 'fortran'];
else
  tbx_builder_gateway_lang('cpp', sci_gateway_dir);
  languages = ['c', 'fortran','cpp'];
end

tbx_build_gateway_loader(languages, sci_gateway_dir);
tbx_build_gateway_clean(languages, sci_gateway_dir);

clear tbx_builder_gateway_lang tbx_build_gateway_loader;
clear sci_gateway_dir;
