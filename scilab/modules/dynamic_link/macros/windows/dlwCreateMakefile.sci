// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function Makename = dlwCreateMakefile(name, ..
                             files, ..
                             libs, ..
                             libname, ..
                             ldflags, ..
                             cflags, ..
                             fflags, ..
                             mexcflags, ..
                             mexfflags)

  Makename = dlwGetDefltMakefileName() + dlwGetMakefileExt();
  
  if isdef('mexcflags') then
    MEXCFLAGS = mexcflags;
  else
    MEXCFLAGS = '';
  end

  if isdef('mexfflags') then
    MEXFFLAGS = mexfflags;
  else
    MEXFFLAGS = '';
  end


  SCIDIR = SCI;
  LIBRARY = name;
  FILES_SRC = '';
  OBJS = '';
  OBJS_WITH_PATH = '';
  FORTRAN_RUNTIME = '';
  OTHERLIBS = '';
  CC = '';
  CFLAGS = cflags;
  FFLAGS = fflags;
  
  LDFLAGS = ldflags;
  
  SCILAB_INCLUDES = dlwGetScilabIncludes();
  SCILAB_INCLUDES = "-I""" + SCILAB_INCLUDES + """";
  SCILAB_INCLUDES = [SCILAB_INCLUDES(1:$-1) + " \"; SCILAB_INCLUDES($)];
  SCILAB_INCLUDES = strcat(SCILAB_INCLUDES, ascii(10));
  
  SCILAB_LIBS = dlwGetScilabLibraries();
  SCILAB_LIBS = """$(SCIDIR)/bin/" + SCILAB_LIBS + """";
  SCILAB_LIBS = [SCILAB_LIBS(1:$-1) + " \"; SCILAB_LIBS($)];
  SCILAB_LIBS = strcat(SCILAB_LIBS, ascii(10));

  if isempty(libname) then
    LIBRARY = name;
  else
    LIBRARY = libname;
  end

  if isempty(files) | ~and(isfile(files)) then
    error(999, msprintf(_("%s: Wrong value for input argument #%d: existing file(s) expected.\n"), "ilib_gen_Make", 3));
  end

  FILES_SRC = strcat(FILES_SRC_MATRIX,' ');

  OBJ_DEST_PATH = '';
  if (getenv("DEBUG_SCILAB_DYNAMIC_LINK","NO") == "NO") then
    OBJ_DEST_PATH = "Release/";
  else
    OBJ_DEST_PATH = "Debug/";
  end

  OBJS_MATRIX = [];
  OBJS_WITH_PATH_MATRIX = [];

  for y = 1:size(files,'*')
    [path_f, file_f, ext_f] = fileparts(files(y));
    OBJS_MATRIX = [OBJS_MATRIX, path_f + file_f + '.obj'];
    OBJS_WITH_PATH_MATRIX = [OBJS_WITH_PATH_MATRIX, OBJ_DEST_PATH + path_f + file_f + '.obj'];
  end

  OBJS = strcat(OBJS_MATRIX, ' ');
  OBJS_WITH_PATH =  strcat(OBJS_WITH_PATH_MATRIX, ' ');

  if ( or(fileext(files) == '.f90') | or(fileext(files) == '.f') ) then
    if findmsifortcompiler() <> 'unknown' then
      if (getenv("DEBUG_SCILAB_DYNAMIC_LINK","NO") == "NO") then
        FORTRAN_RUNTIME = 'libifcoremd.lib libmmd.lib';
      else
        FORTRAN_RUNTIME = 'libifcoremdd.lib libmmdd.lib';
      end
    end
  end

  for x=libs(:)'
     if (x <> [] & x <> '') then
       if OTHERLIBS <> '' then
         OTHERLIBS = OTHERLIBS + ' ' + x + '.lib';
       else
         OTHERLIBS = x + '.lib';
       end
     end
  end

  if isfile(SCI+'/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.VC') then
    MAKEFILE_VC = mgetl(SCI+'/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.VC');
  else
    MAKEFILE_VC = '';
  end

  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__SCI__", SCIDIR);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__SCILAB_INCLUDES__", SCILAB_INCLUDES);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__SCILAB_LIBS__",SCILAB_LIBS);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__LIBNAME__", LIBRARY);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__FILES_SRC__", FILES_SRC);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__OBJS__", OBJS);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__OBJS_WITH_PATH__", OBJS_WITH_PATH);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__FORTRAN_RUNTIME__", FORTRAN_RUNTIME);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__OTHERSLIBS__", OTHERLIBS);

  if CC <> '' then
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__CC__",CC);
  else
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "CC = __CC__","#CC = ");
  end

  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__CFLAGS__", CFLAGS);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__MEXCFLAGS__", MEXCFLAGS);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__FFLAGS__", FFLAGS);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__MEXFFLAGS__", MEXFFLAGS);
  MAKEFILE_VC = strsubst(MAKEFILE_VC, "__LDFLAGS__", LDFLAGS);

  if or(MAKEFILE_VC <> '') then
    mputl(MAKEFILE_VC, Makename);

    if ilib_verbose() > 1 then
      disp(MAKEFILE_VC);
    end

  else
    // TEMPLATE_MAKEFILE.VC not found
    warning(SCI+'/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.VC'+ _('not found.') );
  end
endfunction
//=============================================================================
