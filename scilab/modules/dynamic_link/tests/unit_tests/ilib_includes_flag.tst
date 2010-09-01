// =============================================================================                                                                                                                                  
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab                                                                                                                                                                                                                                                                                                                              
// Copyright (C) 2010 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Check failure case
if ( execstr('ilib_includes_flag(3)','errcatch') == 0 ) then pause, end

// not a dir, should fail

if ( execstr('ilib_includes_flag(/usr/include/stdlib.h)','errcatch')  == 0 ) then pause, end

 // should not exist, should fail
if ( execstr('ilib_includes_flag(/usr/my_exotic_dir/);','errcatch')  == 0 ) then pause, end


ref_flag=" -I"+SCI+"/bin "+"-I"+SCI+"/modules -I/usr/include";
includes_path = [ "bin" , "modules" , "/usr/include" ];

cd SCI;
output_flag = ilib_includes_flag(includes_path);

if (ref_flag ~= output_flag) then pause, end