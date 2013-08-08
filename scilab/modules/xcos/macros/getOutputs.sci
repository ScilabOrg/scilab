
function Y=getOutputs(outtb,pointo)
    Yind=1
    for k=pointo
        nk=size(outtb(k),"*")
        Y(Yind:Yind+nk-1) = outtb(k)(:);
        Yind = Yind+nk
    end
endfunction
