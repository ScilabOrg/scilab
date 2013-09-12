//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Vincent COUVERT
// Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
// Copyright (C) 2009-2010 - DIGITEO - Clément DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

function importScicosPal(palFiles, outPath)
    // Export all palettes to a path as H5 files
    //
    // Calling Sequence
    //   importScicosPal(palFiles, outPath);
    //
    // Parameters
    //   palFiles: string array; the palette files to export
    //   outPath: string; path where to export the palettes
    //
    // Description
    // To export Scicos palettes to an HDF5 instance, use this macro.
    //
    // Examples
    //   palFiles = ls(SCI + "/modules/scicos/palettes/*.cosf");
    //   importScicosPal(palFiles, SCI + "/modules/scicos_blocks/blocks");
    //
    // See also
    //   xcosPal
    //   xcosPalAddBlock
    //
    // Authors
    //   Vincent COUVERT
    //   Antoine ELIAS
    //   Clément DAVID

    rhs = argn(2);

    if ~exists("scicos_diagram") then
        loadXcosLibs();
    end

    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "importScicosPal", 2));
        return
    end

    // global variables
    exportedBlocks = 0;

    // foreach file
    for fIndex = 1:size(palFiles, "*")
        if ~isfile(palFiles(fIndex)) then
            error(msprintf(gettext("%s: File ''%s'' does not exist.\n"), "importScicosPal", palFiles(fIndex)));
            return
        end

        mprintf("%s\n", palFiles(fIndex));

        // get the block names
        varsToLoad = [];
        exec(palFiles(fIndex), -1);

        blockLst = scs_m.objs;
        lstIndex = 1;
        while length(blockLst) >= lstIndex,
            block = blockLst(lstIndex);

            // add PAL_f children blocks
            if typeof(block)=="Block" & block.gui == "PAL_f" then
                blockLst = lstcat(blockLst, block.model.rpar.objs);
                blockLst(lstIndex) = null()
                mprintf("\t%s\n", block.model.sim)
            elseif typeof(block)=="Block" then
                // old scicos palettes doesn't have a PAL_f block but directly
                // the reference instances instead.
                varsToLoad($+1) = block.gui
            end

            lstIndex = lstIndex + 1;
        end

        // export the blocks
        for i = 1:size(varsToLoad, "*")
            block_name = varsToLoad(i) + ".sod";
            blockFile = outPath + "/" + block_name

            // instanciate a block
            // /!\ may cause an error depending on the implementation
            execstr("out = " + varsToLoad(i) + "(""define"")");

            // Update on super block
            if out.model.sim == "super" | out.model.sim == "csuper" then
                [ierr,scicos_ver,scs_m]=update_version(out.model.rpar);

                if ierr <> 0 then
                    mprintf("FAILED TO UPDATE AND EXPORT: %s\n", out.gui);
                else
                    mprintf("%s updated from %s\n", out.gui, scicos_ver);
                end

                out.model.rpar = scs_m;
            end

            doExport = %t;
            if isfile(blockFile) then
                //if the file already exists try to load data and compare
                out2 = out;
                bImport = import_from_hdf5(blockFile);

                //data are identical
                if bImport == %t & isequal(out, out2) then
                    doExport = %f;
                else
                    out = out2;
                end
            end

            if doExport == %t then
                mprintf("%d: %s\n", i, block_name);
                bexport = export_to_hdf5(blockFile, "out");
                if (~bexport) then
                    mprintf("FAILED TO EXPORT: %s\n", out.gui);
                end

                out2 = out;
                bImport = import_from_hdf5(blockFile);

                if bImport == %f | or(out2 <> out) then
                    mprintf("FAILED TO EXPORT: %s\n", out.gui);
                end
                exportedBlocks = exportedBlocks + 1;
            end
        end
    end
    disp("exportedBlocks : " + string(exportedBlocks));
endfunction

