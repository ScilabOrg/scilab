// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Serge STEER
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function matfile2sci(mat_file_path, result_file_path, overwrite)
    // Translate a Matlab MAT file into a Scilab file
    //
    // mat_file_path : path of the Matlab MAT file
    // result_file_path : path of the generated Scilab file
    //
    //This function has been developed following the "MAT-File Format" description:
    //www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf

    if ~with_module("matio") then
        error(msprintf(gettext("%s: %s module is not installed.\n"), "matfile2sci", "matio"));
    end

    [lhs,rhs]=argn(0)
    if rhs<2 | rhs>3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "matfile2sci", 2, 3));
    end

    //overwrite is false by default
    if rhs==2
        overwrite = %F;
    end

    //overwrite must be a boolean
    if type(overwrite)<>4
        error(msprintf(_("%s: Wrong type for argument #%d: Boolean matrix expected.\n"), "matfile2sci", 3));
    end

    if size(overwrite, "*")<>1
        error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"), "matfile2sci", 3, 1, 1));
    end

    //--file opening
    input_file_path = pathconvert(mat_file_path, %F, %T)
    fdi=matfile_open(input_file_path, "r");
    fdo_path=pathconvert(result_file_path, %F, %T);

    //Wipe file if overwrite is true and the output file previously existed
    if isfile(fdo_path) & overwrite
        fdo = mopen(fdo_path, "wb");
    end

    //-- Read first variable
    ierr = execstr("[Name, Matrix, Class] = matfile_varreadnext(fdi);", "errcatch");
    ierrsave = 0;

    if (ierr~=0) | ~exists("Name")
        error(msprintf(_("%s: Could not read variables in %s"), "matfile2sci", input_file_path))
    end

    if Name==""
        error(msprintf(_("%s: Could not read variables in %s"), "matfile2sci", input_file_path))
    end

    //--loop on the stored variables
    while Name<>"" & ierr==0 & ierrsave==0
        ierrsave = execstr(Name + " = Matrix; save(fdo_path, ""-append"", """+Name+""")", "errcatch")
        if ierrsave==0 then
            //-- Read next variable
            ierr = execstr("[Name, Matrix, Class] = matfile_varreadnext(fdi);", "errcatch");
        end
    end

    if exists("fdo")==1
        mclose(fdo);
    end

    //--file closing
    matfile_close(fdi);
endfunction
