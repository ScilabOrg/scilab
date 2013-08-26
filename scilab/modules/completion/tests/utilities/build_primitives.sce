// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2009-2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

previous_dir = pwd();
cd(get_absolute_file_path("build_primitives.sce"));
file_path = pwd();
mkdir(TMPDIR + "/completion");
cd(TMPDIR + "/completion");

ilib_name   = "completion_c";
files       = [ "sci_completeline.c"    , ..
"sci_getpartlevel.c"    , ..
"sci_getfilepartlevel.c", ..
"sci_getcommonpart.c"   , ..
"sci_getfields.c" ];

for i = 1 : size(files, "*")
    copyfile(file_path + "/" + files(i), TMPDIR + "/completion");
end

if getos() == "Windows" then
    libs   = SCI   + "/bin/scicompletion";
    cflags = "-I " + SCI +"/modules/completion/includes";

elseif isdir(SCI+"/modules/completion/includes")
    // Unix source version
    libs   = SCI + "/modules/completion/.libs/libscicompletion";
    cflags = "-I " + SCI +"/modules/completion/includes -I " + SCI + "/modules/localization/includes";

else
    // Unix binary version
    SCI_LIB     = strsubst(SCI,"/share\/scilab$/","lib/scilab"     ,"r");
    SCI_INCLUDE = strsubst(SCI,"/share\/scilab$/","include/scilab" ,"r");
    libs        = SCI_LIB + "/libscicompletion";
    cflags      = "-I " + SCI_INCLUDE
end

// name known in scilab , C function called
table = ["completeline"    ,"sci_completeline"    ;
"getpartlevel"    ,"sci_getpartlevel"    ;
"getfilepartlevel","sci_getfilepartlevel";
"getcommonpart"   ,"sci_getcommonpart"   ;
"getfields"       ,"sci_getfields" ];


libname = ilib_build(ilib_name,table,files,libs,[],"",cflags);

cd(previous_dir);

clear ilib_build;
clear libname;
clear table;
clear libs;
clear files;
clear ilib_name;
clear previous_dir;
clear gateway_c_dir;
clear SCI_LIB;
clear SCI_INCLUDE;
