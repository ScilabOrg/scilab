// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=%ce_c_ce(a,b)
    da=a.dims;if size(da,"*")<2 then da(2)=1,end
    db=b.dims;if size(db,"*")<2 then db(2)=1,end
    da=double(da);db=double(db);
    if size(da,"*")<>size(db,"*") then
        error(msprintf(_("%s: In concatenation the number of dimensions for each component must match.\n"),"ce_c_ce"));
    end

    kd=find(da<>db)
    kd(find(kd==2))=[]
    if kd<>[] then
        error(5)
    end
    v=[matrix(1:prod(da),da(1)*da(2),prod(da)/(da(1)*da(2)));
    -matrix(1:prod(db),db(1)*db(2),prod(db)/(db(1)*db(2)))]
    val=list()
    for k=1:size(v,"*")
        if v(k)>0 then
            val(k)=a.entries(v(k))
        else
            val(k)=b.entries(-v(k))
        end
    end
    da(2)=da(2)+db(2)
    r=mlist(["ce","dims","entries"],int32(da),val)
endfunction



