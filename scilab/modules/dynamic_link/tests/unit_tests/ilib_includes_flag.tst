// =============================================================================                                                                                                                                  
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab                                                                                                                                                                                                                                                                                                                              
// Copyright (C) 2010 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Check failure case
try
  ilib_includes_flag(3);
  if %T then pause,end
catch

end

try
  ilib_includes_flag("/usr/include/stdlib.h"); // not a dir, should fail
  if %T then pause,end
catch

end

try
  ilib_includes_flag("/usr/my_exotic_dir/"); // should not exist, should fail
  if %T then pause,end
catch

end


ref_flag=" -I"+SCI+"/bin "+"-I"+SCI+"/modules -I/usr/include";
includes_path = [ "bin" , "modules" , "/usr/include" ];

cd SCI;
output_flag = ilib_includes_flag(includes_path);

if (ref_flag ~= output_flag) then pause, end