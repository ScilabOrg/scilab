// This file is released into the public domain

sci_gateway_dir = get_absolute_file_path("builder_gateway.sce");

tbx_builder_gateway_lang(["fortran" "c"],sci_gateway_dir);
tbx_build_gateway_loader(["fortran" "c" "cpp"],sci_gateway_dir);
tbx_build_gateway_clean(["fortran" "c" "cpp"],sci_gateway_dir);

clear tbx_builder_gateway_lang tbx_build_gateway_loader;
clear sci_gateway_dir;
