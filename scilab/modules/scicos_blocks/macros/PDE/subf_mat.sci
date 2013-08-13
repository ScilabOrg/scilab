//  Scicos
//
//  Copyright (C) INRIA - Author : EADS-CCR
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function vect=subf_mat(M,N)
    // fonction pour la soustraction ele/ele matrice //
    // (string) M .- N matrice(string)               //
    // sortie :                                      //
    //    - vect (String) : Matrice de taille de M   //
    // entr�es :                                     //
    //    - M (String) : Matrice                     //
    //    - N (String) : Matrice                     //
    //-----------------------------------------------//
    vect=[];
    if (N ==[]) then
        vect=M;
        return;
    elseif (M==[]) then
        vect="-1".*N;
        return;
    end
    [n,m]=size(M);
    for i=1:n
        for j=1:m
            vect(i,j)=subf(M(i,j),N(i,j));
        end
    end
endfunction
