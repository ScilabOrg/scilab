// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) ENPC
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function libn = ilib_compile(lib_name,makename,files, ..
				                         ldflags, ..
				                         cflags, ..
				                         fflags, ..
				                         cc)


  libn=""; //** init variable 

  if ~haveacompiler() then
  	error(_("A Fortran or C compiler is required."))
  	return;
  end

  [lhs,rhs]=argn(0);
  
  if rhs < 3 then files = []; end 
  
  if typeof(lib_name)<>'string' then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"ilib_compile",1));
    return ;
  end
  
  oldpath = getcwd();
  files = files(:)';
  files1 = strsubst(strsubst(files,'.obj','') ,'.o','');
  [make_command,lib_name_make,lib_name,path,makename,files]= ...
      ilib_compile_get_names(lib_name,makename,files);

  if path<> '';  chdir(path);  end 
  
  // first try to build each file step by step 
  if MSDOS then
    //** ----------- Windows section  -----------------
    nf = size(files,'*');
    
    for i=1:nf 
      write(%io(2),_("   Compilation of ")+files1(i));
      unix_s(make_command+makename + ' '+ files(i)); 
    end
    
    // then the shared library 
    write(%io(2),_("   Building shared library (be patient)"));
    unix_s(make_command + makename + ' '+ lib_name); 
   
  else
    //** ---------- Linux section ---------------------  
    // Source tree version
	if isdir(SCI+"/modules/core/includes/") then
	  cflags="-I"+SCI+"/modules/core/includes/ -I"+SCI+"/modules/mexlib/includes/ "
	  cflags="-I"+SCI+"/modules/output_stream/includes/ -I"+SCI+"/libs/MALLOC/includes "+cflags
	  fflags="-I"+SCI+"/modules/core/includes/"+fflags
	end

	// Binary version
	if isdir(SCI+"/../../include/scilab/core/") then
	  cflags="-I"+SCI+"/../../include/scilab/core/ -I"+SCI+"/../../include/scilab/mexlib/ "
	  cflags="-I"+SCI+"/../../include/output_stream/includes/ -I"+SCI+"/../../include/malloc/includes/ "+cflags
	  fflags="-I"+SCI+"/../../include/core/includes/"+fflags
	end

	// System version (ie: /usr/include/scilab/)	
	if isdir("/usr/include/scilab/") then
	  cflags="-I/usr/include/scilab/core/ -I/usr/include/scilab/mexlib/ "
	  cflags="-I/usr/include/scilab/output_stream/ -I/usr/include/scilab/malloc/ "+cflags
	  fflags="-I/usr/include/scilab/core/"+fflags
	end
	
      oldPath = pwd();
	  // Switch back to the TMPDIR where the mandatory files are
	  chdir(TMPDIR);
	  cmd = "make "
	  
	  cmd = cmd + gencompilationflags_unix(ldflags, cflags, fflags, cc)

    //** BEWARE : this function can cause errors if used with "old style" Makefile inside a Scilab 5
    //**          environment where the Makefile are created from a "./configure"  
	  [msg, ierr, stderr] = unix_g(cmd) ; 
	  if ierr <> 0 then
		mprintf(gettext("%s: An error occured during the compilation:\n"),"ilib_compile");
		lines(0)
	    disp(stderr);
		mprintf("\n");
		mprintf(gettext("%s: The command was:\n"),"ilib_compile");
		mprintf(cmd);
		mprintf("\n");
		chdir(oldPath); // Go back to the working dir
	    return ;
	  end
	  if stderr <> "" then
		msprintf(gettext("%s: Warning: No error code returned by the compilation but the error output is not empty:\n"),"ilib_compile");
		mprintf(stderr);
		return ;
	  end

	  // Copy the produce lib to the working path
	  copyfile(".libs/" + lib_name, oldPath);
   
  end
	
  libn = path + lib_name_make ; 
  chdir(oldpath);
  
endfunction
//==========================================


//==========================================
// function only defined in ilib_compile
//==========================================
function [make_command,lib_name_make,lib_name,path,makename,files] = ilib_compile_get_names(lib_name,makename,files) 

  files = strsubst(strsubst(files,'.obj','') ,'.o',''); //compatibility
    
  k = strindex(makename,['/','\']);
  
  if k~=[] then
    path = part(makename,1:k($));
    makename = part(makename,k($)+1:length(makename));
  else
     path='';
  end
  
  lib_name = lib_name+getdynlibext();
  lib_name_make = lib_name;
  
  if MSDOS then // WINDOWS
    FILES = [];
    for x = files(:)' 
      [ptmp,ftmp,fext] = fileparts(x);
      FILES = [FILES,ptmp+ftmp];
    end
    
    // Visual Studio C++ 
    if ( findmsvccompiler() <> 'unknown' ) then 
      makename = makename + '.mak' ; 
      make_command = 'nmake /Y /nologo /f '
    else
      // LCC-WIN32
      if findlcccompiler() then
        makename = makename + '.lcc' ; 
        make_command = 'make -f ';
      else
      // TO DO : Add another compiler here
      end
      
    end
    if FILES<>[] then 
      files = FILES + '.obj' ;
    end
  else // LINUX
  
    make_command = 'make '; 
    
    if files <> [] then 
      files = files + '.o';
    end
    
  end
  
endfunction 

//==========================================
