// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1994-2011 - INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [M] = %hm_e(varargin)
// extract an sub_hypermatrix. 
// Note that extraction for hypermatrix of  floats, booleans and integers
// is coded in C in hmops.c excepted for 
  rhs=size(varargin)
  M=varargin(rhs)
  dims=size(M)
  nbdims = size(dims,"*");
  nb_index_vect = rhs-1;

  //handle special cases
  if prod(dims)==0 then //M is an empty matrix

    // M(i,j,k,..)  or M(I) with I hypermatrix and M an empty matrix

    //check if all indices evaluates to []
    for k=1:nb_index_vect
      Ik=evaluate_index(varargin(k),0,k)
      if min(size(ik))>0 then //ik<>[] and ik<>:
         error(msprintf(_("Extraction: Index #%d exceeds matrix dimensions: %d"),k,0))
      end
    end
    M=[]
    return
  end

  if  nb_index_vect==1 //single index,  
    // I hypermatrix or M hypermatrix of strings, polynomials (the other cases hard coded)
    I=varargin(1)
    if prod(dims)==1 then //M is a scalar results inherits the dimensions of the index
      //case M(I), I hypermatrix of ones 
      if and(I.entries==1) then
        M(1:size(I,'*'))=M;
      else
        error(msprintf(_("Extraction: Index #%d exceeds matrix dimensions: %d"),1,1))
      end
      M=matrix(M,size(I))
    else 
      if typeof(I)=="hypermat" then I=I.entries,end
      if typeof(M)=="hypermat" then M=M.entries,end
      if nbdims==2&dims(1)==1 then //M row vector; return a row vector
        M=matrix(M(I),1,-1)
      else //return a column vector
        M=M(I)
      end
    end
    return
  end
  
  if typeof(M)=="hypermat" then M=M.entries,end


  //increase or reduce the number of dimension of M according to the
  //number of indices
  if nb_index_vect > nbdims then //increase
    dims( nbdims+1:nb_index_vect)=1;
  elseif nb_index_vect < nbdims then //reduce
    nbdims = nb_index_vect;
    dims(nbdims) = prod(dims(nbdims:$))
  end

  I = 0;
  dims1 = zeros(1,nb_index_vect);
  
  for k=nb_index_vect:-1:1
    ik=evaluate_index(varargin(k),dims(k),k)
    if min(ik)<1|max(ik)>dims(k) then
      error(msprintf(_("Extraction: Index #%d exceeds matrix dimensions: %d"),k,dims(k)))
    end

    nik = size(ik, "*");
    if nik == 0 then, M=[], return, end
    
    dims1(k) = nik;
    if nik > 1 then
      if size(I,'*') > 1 then
        I=(dims(k)*I).*.ones(ik)+ones(I).*.(ik-1)
      else
        I=dims(k)*I+ik-1
      end
    else
      I=dims(k)*I+ik-1
    end
  end
  // remove highest dimensions equal to 1 (Matlab compatibility)
  dims1(max(find(dims1>1))+1:$)=[]  

  select size(dims1,'*')
  case 0
    M = M(I+1)
  case 1
    M = M(I+1)
  case 2
    M = matrix(M(I+1),dims1(1),dims1(2))
  else
    M = mlist(['hm','dims','entries'],int32(dims1),M(I+1))
  end

endfunction

function I=evaluate_index(I,n,pos)
//converts index like $, 1:$, boolean array into regular vector of indices
//I is returned as a row vector
  if typeof(I)=="hypermat" then I=I.entries,end
  type_I = type(I)
  if type_I==2 then
    I=round(horner(I,n)),
  elseif type_I==129 then
    t=horner(I(1:3),n)
    I=t(1):t(2):t(3)
  elseif or(type_I==[4 6]) then
    I=find(I)
  elseif type_I==8 then
    I=double(I)
  elseif type_I==1  then
    if min(size(I))<0 then I=1:n,end
  else
    error(msprintf(_("Extraction: invalid index #%d"),pos))
  end
  I=round(matrix(I,-1,1))

endfunction
