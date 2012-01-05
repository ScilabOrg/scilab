//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Clément DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function xcosExternalAction(scs_m, blk, blk_updated, command)
    deletefile(blk_updated);
    
    // import diagram
    import_from_hdf5(scs_m);
    // rename due to bug 10454
    d = scs_m;
    
    // import block
    import_from_hdf5(blk);
    // rename due to bug 10454
    blk = scs_m;
    scs_m = d;
    clear d;
    
    // call command
    ierr = execstr(command, 'errcatch');
    if ierr <> 0 then
        [msg, err] = lasterror();
        disp(msg);
        return;
    end

    // export the block
    if exists("blk") then
        status = export_to_hdf5(blk_updated, "blk");
    end
endfunction

