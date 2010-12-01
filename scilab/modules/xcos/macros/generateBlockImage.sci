//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

// Generate a block image from the instance.
//
// @param block the block instance
// @param path output file path
// @param[opt] file the file name to use (without extension).
//             The default is `block.gui'.
// @param[opt] handle the graphical handle to use
//             The default is to get the current handle with gcf().
// @param[opt] imageType the exported image type. only "svg" and "gif" is
//             supported. The default is to use "gif".
// @param[opt] withPort true if the exported image should contains the port,
//             false otherwise. The default is value is true.
// @return status %T if the operation has been sucessfull, %F otherwise.
function status = generateBlockImage(block, path, file, handle, imageType, withPort)
    status = %f;
    
    // call loadScicosLibs if not loaded
    if exists("scicos_diagram", 'a') == 0 then loadScicosLibs(); end

    [lhs,rhs] = argn(0)
    if rhs < 2 | rhs > 4 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "generateBlockImage", 2, 4));
    end
    
    if typeof(block) <> "Block" then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": Block type expected.\n"), "generateBlockImage", "block"));
    end
    
    if typeof(path) <> "string" | ~isdir(path) then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": directory path string expected.\n"), "generateBlockImage", "path"));
    end
    
    // generate a default graphic or clear the existing one
    if exists("imageType", 'l') == 0 then
        imageType = "gif";
    else
        if typeof(imageType) <> "string" | and(imageType <> ["svg", "gif"]) then
            error(msprintf(gettext("%s: Wrong input argument ""%s"": ""svg"" or ""gif"" expected.\n"), "generateBlockImage", "imageType"));
        end
    end
    
    if exists("withPort", 'l') == 0 then
        withPort = %t;
    end

    // set the default outFile
    if exists("file", 'l') == 0 then
        outFile = path + "/" + block.gui + "." + imageType;
    else
        if typeof(file) <> "string" | size(str, '*') <> 1 then
            error(msprintf(gettext("%s: Wrong type for input argument ""%s"": string expected.\n"), "generateBlockImage", "file"));
        end
        outFile = path + "/" + file + "." + imageType;
    end
    
    // generate a default graphic or clear the existing one
    if exists("handle", 'l') == 0 then
        handle = gcf();
        deleteHandle = %t;
    else
        if typeof(handle) <> "handle" then
            error(msprintf(gettext("%s: Wrong type for input argument ""%s"": handle type expected.\n"), "generateBlockImage", "handle"));
        end
        
        clf();
        deleteHandle = %f;
    end
    
    if withPort then
        // Execute the plot function of the block

        // constants
        diagram = scicos_diagram();
        options = diagram.props.options;
        options('3D')(1) = %f;
        sz = block.graphics.sz;
        orig = block.graphics.orig;
        
        gh_axes = gca();
        gh_curwin = handle;
        
        // draw settings
        // note that the gh_axes variable have to be known on the "plot" call
        gh_axes.fractional_font = "off";
        gh_axes.arc_drawing_method = "lines";
        o_size = size(gh_axes.children);
        gh_axes.data_bounds = [orig(1), orig(2); sz(1), orig(2)+sz(2)];
        gh_axes.isoview = "on";
        gh_axes.margins = 0.01 * ones(1, 4);
        gh_axes.box ="off";
        handle.axes_size = [max(20, 20 * sz(1)), max(20, 20 * sz(2))];
        
        // Create variable o because needed inside "plot"
        o = block;
        ierr = execstr("block  = " + o.gui + "(""plot"",o)", "errcatch");
        if (ierr <> 0) then
            return;
        end
    else
        // Execute the gr_i of the block
        xf = 60; yf = 40;
        nin = size(block.model.in,1);
        nout = size(block.model.out,1);
        clkin = size(block.model.evtin,1);
        clkout = size(block.model.evtout,1);
        o = block;

        [orig,sz,orient] = (block.graphics.orig,block.graphics.sz,block.graphics.flip);

        gr_i = block.graphics.gr_i;

        if type(gr_i) == 15 then
         [gr_i,coli] = gr_i(1:2);
        else
          coli = [] ;
        end

        gh_axes = gca();
        o_size=size(gh_axes.children);
        deff('c=scs_color(c)',' ') ;
        gr_i = [gr_i];
        model = block.model;

        //@@ Alan,28/11/08 : protect variables of standard_draw
        deff('ierr=exec_gr_i(gr_i)','ierr=execstr(gr_i,''errcatch'',''n'');');
        //** Check of the graphics object has generated any error
        if exec_gr_i(gr_i)<>0 then
            status = %f;
            disp(['Error in Icon definition :'; lasterror()]);
        end
    end

    if imageType == "svg" then
        xs2svg(handle.figure_id, outFile);
    elseif imageType == "gif" then
        xs2gif(handle.figure_id, outFile);
    end
    
    // post operations
    if deleteHandle then
        delete(handle);
    end
    status = %t;
endfunction

