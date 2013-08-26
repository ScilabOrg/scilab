// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function tbx_build_blocks(module, names, macros_path)
    // Build a default block instance
    //
    // Calling Sequence
    //   tbx_build_blocks(module, names)
    //
    // Parameters
    // module: toolbox base directory
    // names: list of block names (sci file name without extension)

    if argn(2) < 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: At least %d expected.\n"),"tbx_build_blocks",2));
    end
    if argn(2) > 3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: At most %d expected.\n"),"tbx_build_blocks",3));
    end


    // checking module argument
    if type(module) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"tbx_build_blocks",1));
    end
    if size(module,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"),"tbx_build_blocks",1));
    end
    if ~isdir(module) then
        error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_build_blocks",module));
    end

    // checking names argument
    if type(names) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"tbx_build_blocks",2));
    end

    // checking optional macros_path argument
    if ~exists("macros_path", "l") then
        macros_path = module + "/macros/";
    end
    if type(macros_path) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"tbx_build_blocks",3));
    end
    if size(macros_path,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"),"tbx_build_blocks",3));
    end
    if ~isdir(macros_path) then
        error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_build_blocks",macros_path));
    end

    mprintf(gettext("Building blocks...\n"));

    // load Xcos libraries when not already loaded.
    if ~exists("scicos_diagram") then loadXcosLibs(); end

    // create directories
    if ~isdir(module + "/images") then
        createdir(module + "/images");
    end
    if ~isdir(module + "/images/h5") then
        createdir(module + "/images/h5");
    end
    if ~isdir(module + "/images/gif") then
        createdir(module + "/images/gif");
    end
    if ~isdir(module + "/images/svg") then
        createdir(module + "/images/svg");
    end

    names = names(:);
    sciFiles = pathconvert(macros_path + "/") + names + ".sci";
    h5Files = pathconvert(module + "/images/h5/") + names + ".sod";
    gif_tlbx = pathconvert(module + "/images/gif");
    svg_tlbx = pathconvert(module + "/images/svg");
    for i=1:size(names, "*")
        // load the interface function
        exec(sciFiles(i), -1);

        // export the instance
        execstr(msprintf("scs_m = %s (''define'');", names(i)));
        if ~export_to_hdf5(h5Files(i), "scs_m") then
            error(msprintf(gettext("%s: Unable to export %s to %s.\n"),"tbx_build_blocks",names(i), h5Files(i)));
        end

        blk = scs_m;

        // export a schema file if it doesn't exist
        files = svg_tlbx + "/" + names(i) + [".svg" ".png" ".jpg" ".gif"];
        files = files(isfile(files));
        if files == [] then
            filename = svg_tlbx + "/" + names(i) + ".svg";
            if ~generateBlockImage(blk, svg_tlbx, names(i), "svg", %f) then
                error(msprintf(gettext("%s: Unable to export %s to %s .\n"),"tbx_build_blocks",names(i), filename));
            end
        end

        // export an image file if it doesn't exist
        files = gif_tlbx + "/" + names(i) + [".png" ".jpg" ".gif"];
        files = files(isfile(files));
        if files == [] then
            filename = gif_tlbx + "/" + names(i) + ".gif";
            xcosPalGenerateIcon(filename);
        end
    end
endfunction
