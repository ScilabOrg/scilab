// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - INRIA -Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function info=xcos_blocks_info(cpr);
    //creates a tabular of the blocks used
    sim=cpr.sim;
    nblk=sim.nb;
    funs=sim.funs;
    labels=sim.labels;
    ptr=sim.xptr;sel=find(ptr(1:$-1)<ptr(2:$));
    indx=emptystr(nblk,1);
    indx(sel)=string(ptr(sel))+":"+string(ptr(sel+1)-1);

    ptr=sim.zptr;sel=find(ptr(1:$-1)<ptr(2:$));
    indz=emptystr(nblk,1);
    indz(sel)=string(ptr(sel))+":"+string(ptr(sel+1)-1);

    ptr=sim.ozptr;sel=find(ptr(1:$-1)<ptr(2:$));
    indoz=emptystr(nblk,1);
    indoz(sel)=string(ptr(sel))+":"+string(ptr(sel+1)-1);

    ptr=sim.rpptr;sel=find(ptr(1:$-1)<ptr(2:$));
    indrp=emptystr(nblk,1);
    indrp(sel)=string(ptr(sel))+":"+string(ptr(sel+1)-1);

    ptr=sim.ipptr;sel=find(ptr(1:$-1)<ptr(2:$));
    indip=emptystr(nblk,1);
    indip(sel)=string(ptr(sel))+":"+string(ptr(sel+1)-1);

    ptr=sim.opptr;sel=find(ptr(1:$-1)<ptr(2:$));
    indop=emptystr(nblk,1);
    indop(sel)=string(ptr(sel))+":"+string(ptr(sel+1)-1);

    info=["block number","simulation function","uid","rpar index","ipar"+...
    " index","opar index","z index","oz index","x index"];
    for i=1:nblk
        info=[info;
        string(i),funs(i),labels(i),indrp(i),indip(i),indop(i),indz(i),indoz(i),indx(i)];
    end
endfunction

