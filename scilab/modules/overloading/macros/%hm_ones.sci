// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function M=%hm_ones(varargin)

    nv=size(varargin)
    if nv==1 then
        M=varargin(1)
        if type(M)==1 then
            dims=M
        else
            dims=M("dims")
        end
        M=hypermat(dims,ones(prod(double(dims)),1))
    else
        dims=ones(1,nv)
        for k=1:nv,dims(k)=int(varargin(k)),end
        //remove highest singleton dimensions
        ks=find(dims==1);
        ns=size(ks,"*");
        while ns>0&nv>2&ks(ns)==nv then ns=ns-1,nv=nv-1,end
        if nv==2 then
            M=ones(varargin(1:nv))
        else
            M=hypermat(dims(1:nv),ones(prod(dims(1:nv)),1))
        end
    end
endfunction
