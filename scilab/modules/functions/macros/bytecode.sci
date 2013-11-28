// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2003-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function x=bytecode(f)
    //returns the function f bytecode array in x
    //should be hard-coded

    if argn(2) < 1 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "bytecode", 1));
    end

    nsiz=6
    if type(f)==8 then //bytecode to function
        u=mopen(TMPDIR+"/%fun.bin","wb");
        name_x=[673720353 673720360 673720360 673720360 673720360 673720360]
        mput(int32([name_x  13]),"i",u)
        mput(f,"i",u)
        mclose(u)
        load(TMPDIR+"/%fun.bin")
    elseif type(f)==13 then //function to bytecode
        warnMode = warning("query");
        warning("off");
        save(TMPDIR+"/%fun.bin",f)
        warning(warnMode);
        I=fileinfo(TMPDIR+"/%fun.bin");
        u=mopen(TMPDIR+"/%fun.bin","rb");
        mgeti(1+nsiz,"i",u);
        x=mgeti(I(1)-(1+nsiz),"i",u);
        mclose(u)
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"bytecode",1))
    end
endfunction
