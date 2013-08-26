// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Bruno Pincon
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [varargout] = ndgrid(varargin)
    //
    //  CALLING SEQUENCES
    //       [X, Y] = ndgrid(x, y)
    //    or [X, Y, Z] = ndgrid(x, y, z)
    //    or [X, Y, Z, T] = ndgrid(x, y, z, t)
    //    etc ...
    //
    //  PURPOSE
    //     An utility similar to the Matlab 's one. For instance
    //     in the 3d case :
    //
    //     [X, Y, Z] = ndgrid(x, y, z)
    //
    //     with x, y, z three vectors of length nx, ny, nz, we have
    //     X, Y, Z which are hypermatrices of format nx x ny x nz
    //     and, forall  i in [1,nx], j in [1, ny], k in [1, nz]
    //         X(i,j,k) = x(i)
    //         Y(i,j,k) = y(j)
    //         Z(i,j,k) = z(k)
    //
    //     X, Y, Z are the coordinates of the grid points formed by
    //     the cartesian product of the one dimensional grids
    //     x, y and z.
    //
    //  AUTHOR
    //     Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
    //
    nbdim = length(varargin)
    if nbdim < 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: At least %d expected.\n"),"ndgrid",2));
    end
    dim = zeros(1,nbdim)
    for k = 1:nbdim
        if type(varargin(k)) ~= 1 then
            error(msprintf(gettext("%s: Wrong type for argument #%d: Real expected.\n"),"ndgrid",k));
        end
        dim(k) = length(varargin(k))
        varargin(k) = matrix(varargin(k),1,dim(k)) // force row form
    end

    varargout = list()
    for k = 1:nbdim
        varargout(k) = matrix(     ones(1,prod(dim(k+1:$))) ...
        .*. varargin(k)              ...
        .*. ones(1,prod(dim(1:k-1))) , dim )
    end
endfunction
