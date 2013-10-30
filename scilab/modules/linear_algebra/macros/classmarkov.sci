
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [perm,rec,tr,indsRec,indsT]=classmarkov(M)
    //returns a permutation vector perm such that
    //M(perm,perm) = [M11 0 0 0 0   0]
    //               [0 M22 0 0     0]
    //               [0 0 M33       0]
    //               [      ...      ]
    //               [0 0       Mrr 0]
    //               [* *        *  Q]
    //Each Mii is a Markov matrix of dimension rec(i)  i=1,..,r
    //Q is sub-Markov matrix of dimension tr
    //States 1 to sum(rec) are recurrent and states from r+1 to n
    //are transient.
    //perm=[indsRec,indsT] where indsRec is a  vector of size sum(rec)
    //and indsT is a vector of size indsT.

    if (type(M) <> 1 & typeof(M) <> "sparse") | ~isreal(M) then
        error(msprintf(_("%s: Wrong type for input argument #%d: A full or sparse real matrix expected.\n"), "classmarkov", 1));
    end

    if type(M)==1
        Mb=sparse(M<>0);
    else Mb=M<>0;
    end

    [ij,v,mn]=spget(Mb);
    n=mn(1)
    he=ij(:,1); ta=ij(:,2);
    // compute lp and ln
    [lp,la,ln]=ta2lpd(he',ta',n+1,n)
    // compute connexity
    [nc,ncomp]=m6compfc(lp,ln,n)

    indsRec=[];indsT=[];rec=[];tr=0;
    for i=1:nc
        inds=find(ncomp==i);
        nb=size(inds,"*");
        M1=M(inds,:); M1(:,inds)=[];
        if sum(M1)==0 then
            indsRec=[indsRec,inds];
            rec=[rec,nb];
        else
            indsT=[indsT,inds];
            tr=tr+nb;
        end
    end
    perm=[indsRec,indsT];
endfunction
