// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function Makename = dlwGenerateMakefile(name, ..
                                 tables, ..
                                 files, ..
                                 libs, ..
                                 libname, ..
                                 with_gateway, ..
                                 ldflags, ..
                                 cflags, ..
                                 fflags, ..
                                 cc)

  [lhs,rhs] = argn(0);
  if rhs < 4 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): at least %d expected.\n"), "dlwGenerateMakefile", 4));
  end

  if argn(2) < 6 then
    with_gateway = %t;
    ldflags = '';
    cflags = '';
    fflags = '';
    cc = '';
  end

  managed_ext = ['.cxx', '.cpp', '.c', '.f90', '.f'];

  // change table if necessary
  if tables <> [] then
    if typeof(tables)<>'list' then
      tables = list(tables)
    end
    L = length(tables);

    for it = 1:L
      table = tables(it);
      [mt, nt] = size(table);

      if nt == 2 then
        col= "csci";
        table = [table, col(ones(mt,1))];
        nt=3;
      end

      if nt<>3 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d.\n") ,"dlwGenerateMakefile", 2));
      end
      tables(it) = table;
    end
  end

  if length(libname) > 0  & strncpy(libname, 3) <> 'lib' then
    libname = 'lib' + libname;
  end
  
  FILES_SRC_MATRIX = [];

  [path_Make, file_Make, ext_Make] = fileparts(dlwGetDefltMakefileName() + dlwGetMakefileExt());

  for i=1:size(files,'*')
    [path_f, file_f, ext_f] = fileparts(files(i));
    FILENAME = [];
    FILE_FOUNDED = %f;
    for y = managed_ext(:)'
      if (FILE_FOUNDED == %f) then
        if (fileinfo(path_f + file_f + y) <> []) | (fileinfo(path_Make + file_f + y) <> []) then
          FILENAME = path_f + file_f + y;
          FILE_FOUNDED = %t;
        end
      end
    end
    FILES_SRC_MATRIX = [FILES_SRC_MATRIX , FILENAME];
  end

  if typeof(tables) <> 'list' then
    tables = list(tables);
  end
  L = length(tables);

  if with_gateway then
    if L == 1 then
      FILES_SRC_MATRIX = [FILES_SRC_MATRIX , name + '.c'];
    else
       for i=1:L
        FILES_SRC_MATRIX = [FILES_SRC_MATRIX , name + string(i) + '.c'];
       end
    end
  end
  
  MEXCFLAGS = '';
  MEXFFLAGS = '';
  
  for it=1:L
    table = tables(it);
    [mt,nt] = size(table);

    for i=1:mt
      if table(i,3)=='cmex' | table(i,3)=='fmex' | table(i,3)=='Fmex' then
        MEXCFLAGS = "-Dmexfunction_=mex" + table(i,2) + "_ -DmexFunction=mex_" + table(i,2);
        MEXFFLAGS = "-Dmexfunction=mex" + table(i,2);
        if table(i,3)=='cmex' then
          filenameMex = table(i,2) + '.c';
        else
          filenameMex = table(i,2) + '.f';
        end
        if grep(FILES_SRC_MATRIX,filenameMex) == [] then
          FILES_SRC_MATRIX = [FILES_SRC_MATRIX , filenameMex];
        end
      end
    end
  end

  Makename = dlwCreateMakefile(name, FILES_SRC_MATRIX, ..
                               libs, libname, ..
                               ldflags, cflags, fflags, ..
                               MEXCFLAGS, MEXFFLAGS);

endfunction
//=============================================================================
