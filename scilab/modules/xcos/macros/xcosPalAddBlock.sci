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

function pal = xcosPalAddBlock(pal, block, pal_block_img, style)
// Add a block to a Scilab/Xcos palette instance. Some optional properties can be added to customize the palette icon and the style of the block.
//
// Calling Sequence
//   pal = xcosPalAddBlock(pal, block);
//   pal = xcosPalAddBlock(pal, block, pal_block_img);
//   pal = xcosPalAddBlock(pal, block, [], style);
//   pal = xcosPalAddBlock(pal, block, pal_block_img, style);
//
// Parameters
//   pal: the palette to update
//   block: the block to add to the palette
//   pal_block_img: the block icon to use on the palette manager.
//   style: the style to apply to the block
//
// Description
// This macros add a block instance to a palette. This block parameter can be an instantiated block or a name (interface-function) or a path to a saved instance. Some operations are performed to load this block and check it's availability so it's interface-function must be loaded on Scilab. Some temporary files are also generated without full path arguments.
//
// The optional pal_block_img argument is generated on the <link linkend="TMPDIR">TMPDIR</link> using Scilab graphics if not specified. Be careful that if you use our palette to be persistent you then need to specify it. Otherwise the generated image will be deleted at the end of the Scilab session.
//
// The optional style argument allow the user to determine the kind of style to be used by this block. This argument can be typed as a path <link linkend="string">string</link> or a <link linkend="struct">struct</link>. If it is a string then a default style value is generated and formatted as a style else a struct is wrapped to a key-value jgraphx settings.
// 
// Examples
//   loadScicosLibs();
//   pal = xcosPal();
//   
//   sumPath = TMPDIR + "/sum.h5";
//   bigSomPath = TMPDIR + "/sum.h5";
//   
//   scs_m = SUM_f("define");
//   export_to_hdf5(sumPath, "scs_m");
//   scs_m = BIGSOM_f("define");
//   export_to_hdf5(bigSomPath, "scs_m");
//   
//   pal = xcosPalAddBlock(pal, sumPath);
//   pal = xcosPalAddBlock(pal, bigSomPath);
//   
//   xcosPalAdd(pal);
//
// See also
//   xcosPal
//   xcosPalAdd
//
// Authors
//   Clément DAVID

    // Checking arguments
    [lhs,rhs] = argn(0)
    if rhs < 2 | rhs > 5 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "xcosPalAddBlock", 2, 5));
    end
    
    if lhs > 1 then
        error(msprintf(gettext("%s: Wrong number of output arguments: %d expected.\n"), "xcosPalAddBlock", 1));
    end
    
    if typeof(pal) <> "palette" then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": palette type expected.\n"), "xcosPalAddBlock", "pal"));
    end
    
    // check and tranform block argument
    
    if typeof(block) == "Block" then
        path = TMPDIR + "/" + block.gui + ".h5";
        scs_m = block;
        err = export_to_hdf5(path, "scs_m");
        if err <> %T then
            error(msprintf(gettext("%s: Unable to export ""%s"" to ""%s"".\n"), "xcosPalAddBlock", "block", path));
        end
        
        block = path; // block mute to a full path string.
    end
    
    if typeof(block) == "string" then
        if exists(block) <> 0 & typeof(evstr(block)) == "function" then
            execstr("scs_m = " + block + "(""define"");");
            path = TMPDIR + "/" + block + ".h5";
            err = export_to_hdf5(path, "scs_m");
            if err <> %T then
                error(msprintf(gettext("%s: Unable to export ""%s"" to ""%s"".\n"), "xcosPalAddBlock", "block", path));
            end
            
            block = path; // block mute to a full path string.
        else // assume this is a path
            fd = mopen(block, "rb");
            [err, msg] = merror(fd);
            if err <> 0 then
                error(msg);
            end
            block = fullpath(block);
            mclose(fd);
            
            // store the block instance if not already saved
            if exists("scs_m", 'l') == 0 then
                status = import_from_hdf5(block);
                if ~status then
                    error(msprintf(gettext("%s: Unable to load block from ""%s"": hdf5 file expected.\n"), "xcosPalAddBlock", block));
                end
                
                if exists("scs_m", 'l') == 0 then
                    error(msprintf(gettext("%s: Unable to load block from ""%s"": no `scs_m'' variable found.\n"), "xcosPalAddBlock", block));
                end
            end
        end
    else
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": function as string or Block type or full path string expected.\n"), "xcosPalAddBlock", "block"));
    end
    
    // at this point we can assert that `block' is a full path string to a 
    // saved block reference instance (hdf5 format) also kept into 
    // `scs_m'.
    
    // now handle pal_block_img argument
    if ~exists("pal_block_img", 'l') | isempty(pal_block_img) then
        // block icon by default
        pal_block_img = TMPDIR + "/" + scs_m.gui + ".gif";
        if isfile(pal_block_img) then
            error(msprintf(gettext("%s: Unable to generate the palette icon : ""%s"" already exists.\n"), "xcosPalAddBlock", pal_block_img));
        end
    else
        // specified block icon
        if typeof(pal_block_img) <> "string" then
            error(msprintf(gettext("%s: Wrong type for input argument ""%s"": path string expected.\n"), "xcosPalAddBlock", "pal_block_img"));
        end
        currentFile = ls(pal_block_img); // evaluate wildcard and path
        if size(currentFile, '*') <> 1 then
            error(msprintf(gettext("%s: Wrong type for input argument ""%s"": path string expected.\n"), "xcosPalAddBlock", "pal_block_img"));
        end
        pal_block_img = fullpath(currentFile);
    end
    
    // now handle style argument
    if ~exists("style", 'l') | isempty(style) then
        // style by default
        block_img = TMPDIR + "/" + scs_m.gui + ".svg";
        style = "noLabel=1;image=file:" + block_img + ";";
        status = generateBlockImage(scs_m, TMPDIR, imageType="svg");
        if ~status then
            error(msprintf(gettext("%s: Unable to generate the image ""%s"".\n"), "xcosPalAddBlock", block_img));
        end
    else
        // apply the specified style to the block
        if typeof(style) <> "st" & typeof(style) <> "string" then
            error(msprintf(gettext("%s: Wrong type for input argument ""%s"": string or struct expected.\n"), "xcosPalAddBlock", "style"));
        elseif typeof(style) == "st" then
            formattedStyle = "";
            fields = fieldnames(style);
            fieldsSize = size(fields, '*');
            for i=1:fieldsSize
                formattedStyle = formattedStyle + fields(i);
                fieldValue = getfield(fields(i), style);
                if ~isempty(fieldValue) then
                    formattedStyle = formattedStyle+ "=" + string(fieldValue);
                end
                formattedStyle = formattedStyle + ";";
            end
            style = formattedStyle;
        elseif typeof(style) == "string" then
            currentFile = ls(style); // evaluate wildcard and path
            if size(currentFile, '*') == 1 then
                style = fullpath(currentFile);
                style = "noLabel=1;image=file:" + style + ";";
            else
                // assume well formatted string, do nothing
            end
        end
    end
    
    // Store the data into the palette structure
    pal.blockNames($+1) = scs_m.gui // block named class
    pal.blocks($+1) = block; // reference instance full path string
    pal.icons($+1) = pal_block_img; // palette icon full path string
    pal.style($+1) = style; // block style (linked to style definition)
endfunction

