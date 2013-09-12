// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function x=%hm_mean(m,d)
    if argn(2)==1 then
        x=mean(m.entries)
        return
    end
    dims=m.dims;
    if type(dims==8) then flag=1; dims=double(dims); else flag=0;end
    N=size(dims,"*");
    p1=prod(dims(1:d-1));// step to build one vector on which mean is applied
    p2=p1*dims(d);//step for beginning of next vectors
    ind=(0:p1:p2-1)';// selection for building one vector
    deb=(1:p1);
    I=ind*ones(deb)+ones(ind)*deb

    ind=(0:p2:prod(dims)-1);
    I=ones(ind).*.I+ind.*.ones(I)

    x=mean(matrix(m.entries(I),dims(d),-1),1)
    dims(d)=1
    if d==N then
        dims=dims(1:$)
    else
        dims(d)=1
    end
    if size(dims,"*")==2 then
        x=matrix(x,dims(1),dims(2))
    else
        if flag==1 then dims=int32(dims);end
        x=hypermat(dims,x(:))
    end
endfunction
