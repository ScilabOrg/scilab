function B = repmat(A,varargin)
  if type(A)>10 then
    if typeof(A)=="rational" then
      B=rlist(repmat(A.num,varargin(:)),repmat(A.den,varargin(:)),A.dt)
      return
    else
      execstr('B=%'+typeof(A)+"_repmat(A,varargin(:))")
    end
  end
  narg=size(varargin)
  //ajouter test sur les type des elements de varargin
  if narg<1 then
     error(msprintf(_("%s: Wrong number of input arguments: at least %d expected.\n"),"repmat",2))
  end
  if narg==1 then 
    if size(varargin(1),'*')==1 then
      siz=list(varargin(1),varargin(1))
    else //convert array into list
      tmp=varargin(1)
      siz=list();
      for i=1:size(tmp,'*'),siz(i)=tmp(i); end
    end
  else
    siz=list();
    for i=1:narg
      if size(varargin(i),'*')<>1 then
        error('sfd')
      else
        siz(i)=varargin(i)
      end
    end
  end
  
  nd=size(siz)
  if or(type(A)==[5 6]) then //sparse matrices
    if nd>2 then
      error(msprintf(_("%s : Wrong number of output matrix dimensions"+...
                       " required: %d expected for sparse matrices"),"repmat",2))
    end
  end
  for i=size(siz):-1:3
    if siz(i)>1 then break,end
    nd=nd-1
  end
  sizA=size(A)
 
  if and(sizA==1) then //scalar case
    if nd<=2 then
      B=A(ones(siz(1:nd)))
    else
      s=1;for k=1:nd;s=s*siz(i),end
      B=matrix(A(ones(s,1)),siz(1:nd))
    end
  else
    sizA(3:nd)=1;
    I=list()
    for i=1:nd
      ind=matrix(1:sizA(i),-1,1)
      ind=ind(:,ones(1,siz(i)))
      I(i)=ind
    end
    B=A(I(:))
  end
endfunction
