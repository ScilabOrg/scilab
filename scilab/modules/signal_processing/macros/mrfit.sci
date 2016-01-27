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

function [num,den]=mrfit(w,mod,r)
    //Calling sequence:
    //[num,den]=mrfit(w,mod,r)
    //sys = mrfit(w,mod,r)
    //
    //w: vector of frequencies
    //mod: vector of frequency response magnitude at these frequencies.
    //weight: vector of weights given to each point
    //
    //Fits frequency response magnitude data points by a bi-stable transfer
    //function
    //G(s) = num(s)/den(s) of order r.
    //
    //  abs(freq(num,den,%i*w)) should be close to mod
    //

    function ww=mrfitdiff(ww)
        //Utility fct
        p=size(ww(:),"*");
        ww=ww(2:p)-ww(1:p-1);
    endfunction

    w=w(:);mod=mod(:);
    [LHS,RHS]=argn(0);
    if w(1)==0 then w(1)=%eps;end

    if r==0 then num=sum(mod)/size(mod,"*");den=1;return;end

    sl0=round(log10(mod(2)/mod(1))/log10(w(2)/w(1)));w(1)=w(2)/10;
    if sl0~=0 then mod(1)=mod(2)/10^sl0;end

    n=length(w);
    slinf=round(log10(mod($)/mod($-1))/log10(w($)/w($-1)));
    w($)=w($-1)*10;
    if slinf~=0 then mod($)=mod($-1)*10^slinf;end
    logw=log10(w);logmod=log10(mod);delw=mrfitdiff(logw);delmod=mrfitdiff(logmod);

    weight=ones(length(w),1);

    junk=find(abs(mrfitdiff(delmod./delw)) > .6);
    if isempty(junk) then
        ind = [];
    else
        ind = 1 + junk;
    end
    weight(ind)=10*ones(length(ind),1);

    lwnew=[]; modnew=[];
    for i=1:length(w)-1,
        nadd=floor(delw(i)/.3)-1;
        if nadd>0 then
            slope=delmod(i)/delw(i);
            lwnew=[lwnew logw(i)+.3*(1:nadd)];
            modnew=[modnew logmod(i)+.3*slope*(1:nadd)];
            weight=[weight ; ones(nadd,1)];
        end
    end
    log10is=log(10);
    w=[w ; exp(lwnew'*log10is)];
    mod=[mod ; exp(modnew'*log10is)];

    [w,ind] = gsort(-w);
    w=-w; mod=mod(ind); weight=weight(ind);
    ind=find(mrfitdiff(w)>0); ind=[ind(:);length(w)];
    w=w(ind); mod=mod(ind); weight=weight(ind);

    fresp=cepstrum(w,mod);

    [num,den]=frfit(w,fresp,r,weight);
    if LHS==1 then
        num=syslin("c",num/den);
    end

endfunction
