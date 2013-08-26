// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function mtlb_mesh(varargin)
    //la couleur n'est pas traitee
    nv=size(varargin)
    select nv
    case 1 then //mesh(Z)
        Z=varargin(1)
        [m,n]=size(Z)
        plot3d(1:m,1:n,Z,45,35," ",[8,2,4])
    case 2 then //mesh(Z,C) or  //mesh(Z,view)
        if min(size(varargin(2)))==1 then //mesh(Z,view)
            Z=varargin(1)
            view=varargin(2)
            [m,n]=size(Z)
            plot3d(1:m,1:n,Z,view(1),view(2)," ",[8,2,4])
        else
            Z=varargin(1)
            [m,n]=size(Z)
            plot3d(1:m,1:n,Z,45,35," ",[8,2,4])
        end
    case 3 then //mesh(X,Y,Z) or mesh(x,y,Z)
        if min(size(varargin(1)))==1 then //mesh(x,y,Z)
            plot3d(varargin([2 1 3]),45,35," ",[8,2,4])
        else //mesh(X,Y,Z)
            [X,Y,Z]=varargin(1:3)
            [nr,nc]=size(Z)
            indy=(ones(1,nc-1).*.[0;1;1;0]+(1:nc-1).*.[1;1;1;1]).*.ones(1,nr-1);
            indx=ones(1,nc-1).*.(ones(1,nr-1).*.[0;0;1;1]+(1:nr-1).*.[1;1;1;1]);
            [nrl,nrc]=size(indx)
            indx=matrix(indx,nrl*nrc,1);
            indy=matrix(indy,nrl*nrc,1);
            indz=indx+(nr)*(indy-1*ones(indy));
            Z=matrix(Z(indz),4,nrl*nrc/4);
            X=matrix(X(indz),4,nrl*nrc/4);
            Y=matrix(Y(indz),4,nrl*nrc/4);
            plot3d(X,Y,Z,45,35," ",[8,2,4])
        end
    case 4 then //mesh(X,Y,Z,C) or mesh(x,y,Z,C)  or mesh(X,Y,Z,view) or mesh(x,y,Z,view)
        if min(size(varargin(4)))==1 then //mesh(..,view)
            view=varargin(4)
        else
            view=[45,35]
        end
        if min(size(varargin(1)))==1 then //mesh(x,y,Z,..)
            plot3d(varargin([2 1 3]),view(1),view(2)," ",[8,2,4])
        else //mesh(X,Y,Z,..)
            [X,Y,Z]=varargin(1:3)
            [nr,nc]=size(Z)
            indy=(ones(1,nc-1).*.[0;1;1;0]+(1:nc-1).*.[1;1;1;1]).*.ones(1,nr-1);
            indx=ones(1,nc-1).*.(ones(1,nr-1).*.[0;0;1;1]+(1:nr-1).*.[1;1;1;1]);
            [nrl,nrc]=size(indx)
            indx=matrix(indx,nrl*nrc,1);
            indy=matrix(indy,nrl*nrc,1);
            indz=indx+(nr)*(indy-1*ones(indy));
            Z=matrix(Z(indz),4,nrl*nrc/4);
            X=matrix(X(indz),4,nrl*nrc/4);
            Y=matrix(Y(indz),4,nrl*nrc/4);
            plot3d(X,Y,Z,view(1),view(2)," ",[8,2,4])
        end
    end
endfunction
