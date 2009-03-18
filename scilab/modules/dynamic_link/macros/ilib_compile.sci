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
  
  managed_ext = ['.obj','.o'];
  for i=1:size(files,'*') // compatibility scilab 4.x
    [path_f, file_f, ext_f] = fileparts(files(i));
    if or(managed_ext == ext_f) then
      files1(i) = path_f + file_f;
    else
      files1(i) = path_f + file_f + ext_f;
    end
  end
  
  [make_command,lib_name_make,lib_name,path,makename,files]= ...
      ilib_compile_get_names(lib_name,makename,files);

  if path<> '';  chdir(path);  end 
  
  // first try to build each file step by step 
  if MSDOS then
    //** ----------- Windows section  -----------------
    nf = size(files,'*');
    
    for i=1:nf
      if ( ilib_verbose() <> 0 ) then
        mprintf(_("   Compilation of ") + string(files1(i)) +'\n');
      end
      unix_s(make_command+makename + ' '+ files(i)); 
    end
    
    // then the shared library 
    if ( ilib_verbose() <> 0 ) then
      mprintf(_("   Building shared library (be patient)\n"));
    end
    unix_s(make_command + makename + ' '+ lib_name); 
   
  else
    //** ---------- Linux section ---------------------  
	defaultModulesCHeader=[ "core", "mexlib", "output_stream" ];
	defaultModulesFHeader=[ "core" ];
	
    // Source tree version
	if isdir(SCI+"/modules/core/includes/") then
	  for x = defaultModulesCHeader(:)' 
	      cflags=" -I"+SCI+"/modules/"+x+"/includes/ "+cflags;
	  end
	  cflags=" -I"+SCI+"/libs/MALLOC/includes/ " + cflags;

 	  for x = defaultModulesFHeader(:)' 
	  	  fflags=" -I"+SCI+"/modules/"+x+"/includes/ " + fflags;
          end
	end

	// Add MALLOC since that is this two cases, it is at the same place
	defaultModulesCHeader=[ defaultModulesCHeader, "malloc" ]; 

	// Binary version
	if isdir(SCI+"/../../include/scilab/core/") then
	  for x = defaultModulesCHeader(:)' 
	  	  cflags="-I"+SCI+"/../../include/scilab/"+ x + "/ " + cflags
	  end
 	  for x = defaultModulesFHeader(:)' 
	  	  fflags="-I"+SCI+"/../../include/scilab/"+x+"/ "+fflags
	  end
	end

	// System version (ie: /usr/include/scilab/)	
	if isdir("/usr/include/scilab/") then
	  for x = defaultModulesCHeader(:)' 
	  	  cflags="-I/usr/include/scilab/"+x+"/ "+cflags
	  end
 	  for x = defaultModulesFHeader(:)' 
	  	  fflags="-I/usr/include/scilab/"+x+"/ "+fflags
	  end
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
	    if ( ilib_verbose() <> 0 ) then
	      mprintf(gettext("%s: An error occured during the compilation:\n"),"ilib_compile");
	      lines(0);
	      disp(stderr);
		    mprintf("\n");
		    mprintf(gettext("%s: The command was:\n"),"ilib_compile");
		    mprintf(cmd);
		    mprintf("\n");
		  end
		  chdir(oldPath); // Go back to the working dir
	    return ;
	  end
	  if stderr <> "" then
	    if ( ilib_verbose() <> 0 ) then
	      mprintf(gettext("%s: Warning: No error code returned by the compilation but the error output is not empty:\n"),"ilib_compile");
	      mprintf(stderr);
	    end
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
  
  managed_ext = ['.obj','.o'];
  for i=1:size(files,'*') // compatibility scilab 4.x
    [path_f, file_f, ext_f] = fileparts(files(i));
    if or(managed_ext == ext_f) then
      files(i) = path_f + file_f;
    else
      files(i) = path_f + file_f + ext_f;
    end
  end
    
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
