// ====================================================================
// Allan CORNET
// INRIA 2008
// Template toolbox_skeleton
// ====================================================================

src_dir = get_absolute_file_path('builder_src.sce');

builder_src_lang('fortran', src_dir);
builder_src_lang('c', src_dir);

clear src_dir;
