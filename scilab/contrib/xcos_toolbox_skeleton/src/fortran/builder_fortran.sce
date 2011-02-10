// This file is released under the 3-clause BSD license. See COPYING-BSD.

tbx_build_src(["sum_f_tbx"],   ..
              ["sum_f_tbx.f"], ..
              "f",        ..
              get_absolute_file_path("builder_fortran.sce"));

clear tbx_build_src;
