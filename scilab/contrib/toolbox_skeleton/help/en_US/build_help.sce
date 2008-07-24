// ====================================================================
// Copyright INRIA 2008
// Allan CORNET
// ====================================================================

help_lang_dir = get_absolute_file_path('build_help.sce');

build_help(TOOLBOX_TITLE, help_lang_dir);
build_help_loader(TOOLBOX_TITLE, help_lang_dir);

clear help_lang_dir;
