// This file is released under the 3-clause BSD license. See COPYING-BSD.

// This macro compiles the files

src_c_path = get_absolute_file_path("builder_c.sce");

CFLAGS = "-I" + src_c_path;
LDFLAGS = "";
if (getos()<>"Windows") then
    if ~isdir(SCI+"/share") then
        // Source version
        CFLAGS = CFLAGS + " -I" + SCI + "/modules/scicos_blocks/includes" ;
        CFLAGS = CFLAGS + " -I" + SCI + "/modules/scicos/includes" ;
    else
        // Release version
        CFLAGS = CFLAGS + " -I" + SCI + "/../../include/scilab/scicos_blocks";
        CFLAGS = CFLAGS + " -I" + SCI + "/../../include/scilab/scicos";
    end
else
    CFLAGS = CFLAGS + " -I" + SCI + "/modules/scicos_blocks/includes";
    CFLAGS = CFLAGS + " -I" + SCI + "/modules/scicos/includes";

    // Getting symbols
    if findmsvccompiler() <> "unknown" & haveacompiler() then
        LDFLAGS = LDFLAGS + " """ + SCI + "/bin/scicos.lib""";
        LDFLAGS = LDFLAGS + " """ + SCI + "/bin/scicos_f.lib""";
    end
end

tbx_build_src(["sumblk4_tbx","sumblk_tbx"],       ..
              ["sumblk4_tbx.c","sumblk_tbx.c"],   ..
              "c",                              ..
              src_c_path,                       ..
              "",                               ..
              LDFLAGS,                          ..
              CFLAGS);

clear tbx_build_src;
clear src_c_path;
clear CFLAGS LDFLAGS;

