// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque, S. Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function s=%ce_6(varargin)
    // called in the context of complex struct insertion
    // to realize partial extraction

    nind=size(varargin)-1; //the number of indexes
    s=varargin($); // the struct

    if size(varargin)==2 & type(varargin(1))==10 then  //X.b extraction assumed
        f=getfield(1,s);
        if or(varargin(1)==f(3:$)) then //field exists
            // perform standard field extraction
            s=%st_e(varargin(:));
        else //field does not exists
            // return empty struct
            s=mlist(["ce","dims"],int32([0 0]))
        end

    else //X(i,j,k) extraction assumed
        //check if indexes are in the dimension bounds
        dims=s.dims
        if size(dims,"*")<nind then dims($:nind)=1;end
        if size(dims,"*")>nind then dims=[dims(1:nind-1) prod(dims(nind:$))];end // Bug 3833 fix: This line where present in %st_6 but not there
        indmax=ones(1,nind);
        for k=1:nind,indmax(k)=max(varargin(k));end

        if and(indmax<=dims) then //Yes
            // perform standard sub-matrix extraction
            s=%st_e(varargin(:));
        else // i,j,k out of bounds
            f=getfield(1,s);f=f(3:$);
            if f==[] then
                // the original struct has no field return an empty struct
                s.dims=int32([0 0]);
            else
                // return a 1x1 struct with fields set to []
                for k=1:size(f,"*"),setfield(k+2,[],s);end
                s.dims=int32([1 1]);
            end
        end
    end
endfunction
