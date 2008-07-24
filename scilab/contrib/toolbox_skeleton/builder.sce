// ====================================================================
// Copyright INRIA 2008
// Allan CORNET
// ====================================================================
mode(-1);
lines(0);
try   
 getversion('scilab');
catch  
 error('Scilab 5.0 or more is required.');  
end;  
// ====================================================================

TOOLBOX_NAME = 'toolbox_skeleton';
TOOLBOX_TITLE = 'Toolbox Skeleton';

toolbox_dir = get_absolute_file_path('builder.sce');

builder_macros(toolbox_dir);
builder_src(toolbox_dir);
builder_gateway(toolbox_dir);
builder_help(toolbox_dir);
build_loader(TOOLBOX_NAME, toolbox_dir);

clear toolbox_dir;
clear TOOLBOX_NAME;
clear TOOLBOX_TITLE;
