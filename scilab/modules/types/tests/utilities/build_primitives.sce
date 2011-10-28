// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2009-2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

previous_dir = pwd();
cd(get_absolute_file_path("build_primitives.sce"));

ilib_name  = "testHandler_c";
files      = ["sci_testHandler.cpp"
	      "ScilabVariablesHandlerTest.cpp"
	      "GiwsException.cpp" ];

if getos() == "Windows" then
    libs   = SCI   + "/bin/scitypes";
    cflags = "-I " + SCI +"/modules/types/includes";

elseif isdir(SCI+"/modules/types/includes")
    // Unix source version
    libs   = SCI + "/modules/types/.libs/libscitypes";
    cflags = "-I " + SCI +"/modules/types/includes -I " + SCI + "/modules/localization/includes";
    cflags = cflags + " -I" + get_absolute_file_path("build_primitives.sce") + " -I" + SCI + "/modules/jvm/includes/"
    disp(cflags)
else
    // Unix binary version
    SCI_LIB     = strsubst(SCI,"/share\/scilab$/","lib/scilab"     ,"r");
    SCI_INCLUDE = strsubst(SCI,"/share\/scilab$/","include/scilab" ,"r");
    libs        = SCI_LIB + "/libscitypes";
    
end

// name known in scilab , C function called
table = ["testHandler"    ,"sci_testHandler"];


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
