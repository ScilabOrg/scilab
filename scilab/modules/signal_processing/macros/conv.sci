// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge STEER
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function h=conv(u,v,Shape)
    if and(size(u)>1) then
        error( msprintf(_("%s: Wrong size for argument #%d: Vector expected.\n"),"conv",1))
    end
    if and(size(v)>1) then
        error( msprintf(_("%s: Wrong size for argument #%d: Vector expected.\n"),"conv",2))
    end
    if argn(2)==2 then
        Shape="full",
    elseif and(Shape<>["full","same","valid"]) then
        error(msprintf(_("%s: Wrong value for input argument #%d: ""%s"" or ""%s"" expected.\n"),"conv",3, """full"", ""same""","""valid"""));
    end

    h=conv2(u(:),v(:),Shape);
    //set result orientation
    if Shape=="full" then
        if size(u,"*")>size(v,"*") then
            if size(u,1)==1 then h=matrix(h,1,-1);end
        else
            if size(v,1)==1 then h=matrix(h,1,-1);end
        end
    else
        if size(u,1)==1 then h=matrix(h,1,-1);end
    end
endfunction

