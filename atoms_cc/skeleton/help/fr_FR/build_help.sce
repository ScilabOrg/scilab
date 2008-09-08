// ====================================================================
// Copyright INRIA 2008
// Allan CORNET
// ====================================================================

help_lang_dir = get_absolute_file_path('build_help.sce');

tbx_build_help(TOOLBOX_TITLE, help_lang_dir);
tbx_build_help_loader(TOOLBOX_TITLE, help_lang_dir);

clear tbx_build_help tbx_build_loader;
clear help_lang_dir;
