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

TOOLBOX_NAME = 'skeleton';
TOOLBOX_TITLE = 'Toolbox Skeleton';

toolbox_dir = get_absolute_file_path('builder.sce');

tbx_builder_macros(toolbox_dir);
tbx_builder_src(toolbox_dir);
tbx_builder_gateway(toolbox_dir);
tbx_builder_help(toolbox_dir);
tbx_build_loader(TOOLBOX_NAME, toolbox_dir);

clear tbx_builder_macros tbx_builder_src tbx_builder_gateway tbx_builder_help tbx_build_loader;
clear toolbox_dir;
clear TOOLBOX_NAME TOOLBOX_TITLE;
