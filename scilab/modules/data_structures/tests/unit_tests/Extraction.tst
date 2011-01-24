// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->

// Extraction tests
function linenumber()
  [linenum,mac]=where()
  mprintf("Problem detected at line #%d\n",linenum(2))
endfunction

current =%t; //set to current Scilab %f for expected behavior.

doubledata=list([],33,[1 2 3],[4;5;6],[1 2 3;4 5 6],matrix(1:24,[2 3 4]));
stringdata=list([],"33",["1" "2" "3"],["4";"5";"6"],["1" "2" "3";"4" "5" "6"],matrix(string(1:24),[2 3 4]));
intdata=list([],int8(33),int16([1 2 3]),uint32([4;5;6]),uint16([1 2 3;4 5 6]),matrix(uint8(1:24),[2 3 4]));
polydata=list([],1+33*%s,[1 2+2 3]*(1+%s),[4;5;6]*%s,[1 2 3;4 5 6]*(2+3*%s^2),matrix((1:24)*(-1+3*%s),[2 3 4]));
booldata=list([],%t,[%t %f %t],[%f;%t;%t],[%t %t %f;%f %f %t],matrix([%f,%t,%f,%f,%t,%t,%t,%t,%t,%f,%t,%t,%t,%f,%t,%f,%f,%f,%t,%t,%f,%t,%f,%f],[2 3 4]));
datas=list(doubledata,stringdata,intdata,polydata,booldata);

function extraction_test()
for data=datas
//disp(typeof(data($-1)))
  //A(I1) branch

  //  A([])
  for A=data
    if A([])<>[] then linenumber();pause,end
  end
  
  // A(:)
  for A=data
    if A(:)<>matrix(A,-1,1) then linenumber();pause,end
  end
  
  if %t then // A(ones(...))
    ind=list(1,[1 1],[1;1],[1 1;1 1],ones(3,4,2));

    //   empty matrix case
    A=data(1);
    if current then
      //there is an error in Scilab, here but it will be fixed in Scilab6
      //because it is an used feature
      for k1=1:size(ind)
        I1=ind(k1);
        if A(I1)<>[] then linenumber();pause,end
      end
    else
      for k1=1:size(ind)
        I1=ind(k1);
        if execstr("A(I1)","errcatch")==0 then linenumber();pause,end
      end
    end

    //   scalar case returns an array that inherits dimensions of I1
    A=data(2);
    for k1=1:size(ind)
      I1=ind(k1);
      B= A(I1);
      if or(size(B)<>size(I1)) then linenumber();pause,end
      if or(B<>A) then linenumber();pause,end
    end


    //   row vector case (return a row)
    A=data(3);
    for I1=ind
      if or(size(A(I1))<>[1 size(I1,'*')]) then linenumber();pause,end
    end

    //   column vector, matrix, hypermatrix cases (return a column)
    //   take care here Matlab returns a matrix that inherits dimensions of I1
    for k=4:6
      A=data(k);
      for I1=ind
        if or(size(A(I1))<>[size(I1,'*'),1]) then linenumber();pause,end
      end
    end
  end
  if %t then // A(I1)
    ind=list(2,[1 2],[2;1],[1 2;2 1],matrix([1,2,1,1,2,2,2,2,2,1,2,2,2,1,2,1,1,1,2,2,1,2,1,1],[2 3 4]));
    for k=3:6 //   empty and scalar matrix case had been checked above
      A=data(k); At=matrix(A,-1,1);

      if size(A,1)==1&ndims(A)==2 then //result is row
        for k1=1:size(ind)
          I1=ind(k1);
          B=A(I1);
          if or(size(B)<>[1,size(I1,'*')]) then linenumber();pause,end
          for k=1:size(I1,'*')
            if B(k)<>At(I1(k)) then linenumber();pause,end
          end
        end
      else //result is column
        for k1=1:size(ind)
          I1=ind(k1);
          B=A(I1);
          if or(size(B)<>[size(I1,'*'),1]) then linenumber();pause,end
          for k=1:size(I1,'*')
            if B(k)<>At(I1(k)) then linenumber();pause,end
          end
        end  
      end
    end
  end
  
  if %t then //testing out of bounds cases

    ind=list(50,[1 50],[50;1],[1 50;1 1],ones(3,4,2)+49);
    for k=2:6 //   empty matrix case had been checked above
      A=data(k);
      for k1=1:size(ind)
        I1=ind(k1);
        if execstr("B= A(I1);","errcatch")==0 then linenumber();pause,end
      end
    end
  end

  //A(I1,..,Iq) branch

  //  A=[]  case
  A=data(1);
  ind=list([],:,1:$,[%f %f],%f(ones(3,4,2)));

  for I1=ind
    for I2=ind
      if A(I1,I2)<>[] then linenumber();pause,end
    end
  end
  
   for I1=ind
    for I2=ind
      for I3=ind
        if A(I1,I2,I3)<>[] then linenumber();pause,end
      end
    end
   end
   
   //at least one empty index
   F=%f(ones(3,4,2));
   ind=list(..
             list([],[%f %f],F),..
             list(1,[1 1],[1;1],[1 1;1 1],ones(3,4,2),[],[%f %f],F),..
             list(1,[1 1],[1;1],[1 1;1 1],ones(3,4,2),[],[%f %f],F)..
             );
  
   for A=data
      for k=1:2
        ind1=ind(k);
        ind2=ind(3-k);
        for k1=1:size(ind1)
          I1=ind1(k1)
          for k2=1:size(ind2)
            I2=ind2(k2)
            if A(I1,I2)<>[] then linenumber();pause,end
          end
        end
      end
    end
    
    for A=list(data(2:$))
      for K=[ 1 2 3;2 3 1;3 1 2]
        ind1=ind(K(1));
        ind2=ind(K(2));
        ind3=ind(K(3));
        for k1=1:size(ind1)
          I1=ind1(k1)
          for k2=1:size(ind2)
            I2=ind2(k2)
            for k3=1:size(ind3)
              I3=ind3(k3)
              if A(I1,I2,I3)<>[] then linenumber();pause,end
            end
          end
        end
      end
    end

    //all non empty index
    F=%f(ones(3,4,2));F(1,1)=%t;
    ind=list(..
             list([%t %F],F),..
             list(1,[1 1],[1;1],[1 1;1 1],ones(3,4,2),[%T %f],F),..
             list(1,[1 1],[1;1],[1 1;1 1],ones(3,4,2),[%t %f],F)..
             );
  
    A=data(2); //scalar
    if or(A(1,[1 1])<>[A A]) then linenumber();pause,end
    if or(A(1,[1;1])<>[A A]) then linenumber();pause,end
    if or(A(:,[1 1])<>[A A]) then linenumber();pause,end
    if or(A(:,[1;1])<>[A A]) then linenumber();pause,end
    if or(A($,[1 1])<>[A A]) then linenumber();pause,end
    if or(A($,[1;1])<>[A A]) then linenumber();pause,end
    if or(A(%t,[1 1])<>[A A]) then linenumber();pause,end
    if or(A(%t,[1;1])<>[A A]) then linenumber();pause,end
    if or(A([%t %f],[1 1])<>[A A]) then linenumber();pause,end
    if or(A([%t %f],[1;1])<>[A A]) then linenumber();pause,end
   
    if or(A([1 1],1)<>[A;A]) then linenumber();pause,end
    if or(A([1;1],1)<>[A;A]) then linenumber();pause,end
    if or(A([1 1],:)<>[A;A]) then linenumber();pause,end
    if or(A([1;1],:)<>[A;A]) then linenumber();pause,end
    if or(A([1 1],$)<>[A;A]) then linenumber();pause,end
    if or(A([1;1],$)<>[A;A]) then linenumber();pause,end
    if or(A([1 1],%t)<>[A;A]) then linenumber();pause,end
    if or(A([1;1],%t)<>[A;A]) then linenumber();pause,end
    if or(A([1 1],[%t %f])<>[A;A]) then linenumber();pause,end
    if or(A([1;1],[%t %f])<>[A;A]) then linenumber();pause,end
    if execstr('A(2,[1 1])',"errcatch")==0 then linenumber();pause,end
    if execstr('A(2,[1 2])',"errcatch")==0 then linenumber();pause,end
    if execstr('A(1,[1 2])',"errcatch")==0 then linenumber();pause,end
    if execstr('A([%f %t],1)',"errcatch")==0 then linenumber();pause,end
   
    
    for I3=list(1,%t,:,1:$)
      if or(A(1,[1 1],I3)<>[A A]) then linenumber();pause,end
      if or(A(1,[1;1],I3)<>[A A]) then linenumber();pause,end
      if or(A(:,[1 1],I3)<>[A A]) then linenumber();pause,end
      if or(A(:,[1;1],I3)<>[A A]) then linenumber();pause,end
      if or(A($,[1 1],I3)<>[A A]) then linenumber();pause,end
      if or(A($,[1;1],I3)<>[A A]) then linenumber();pause,end
      if or(A(%t,[1 1],I3)<>[A A]) then linenumber();pause,end
      if or(A(%t,[1;1],I3)<>[A A]) then linenumber();pause,end
      if or(A([%t %f],[1 1],I3)<>[A A]) then linenumber();pause,end
      if or(A([%t %f],[1;1],I3)<>[A A]) then linenumber();pause,end
      
      if or(A([1 1],1,I3)<>[A;A]) then linenumber();pause,end
      if or(A([1;1],1,I3)<>[A;A]) then linenumber();pause,end
      if or(A([1 1],:,I3)<>[A;A]) then linenumber();pause,end
      if or(A([1;1],:,I3)<>[A;A]) then linenumber();pause,end
      if or(A([1 1],$,I3)<>[A;A]) then linenumber();pause,end
      if or(A([1;1],$,I3)<>[A;A]) then linenumber();pause,end
      if or(A([1 1],%t,I3)<>[A;A]) then linenumber();pause,end
      if or(A([1;1],%t,I3)<>[A;A]) then linenumber();pause,end
      if or(A([1 1],[%t %f],I3)<>[A;A]) then linenumber();pause,end
      if or(A([1;1],[%t %f],I3)<>[A;A]) then linenumber();pause,end
      if execstr('A(2,[1 1],I3)',"errcatch")==0 then linenumber();pause,end
      if execstr('A(2,[1 2],I3)',"errcatch")==0 then linenumber();pause,end
      if execstr('A(1,[1 2],I3)',"errcatch")==0 then linenumber();pause,end
      if execstr('A([%f %t],1,I3)',"errcatch")==0 then linenumber();pause,end
    end
    
    //matrix case  (2 by 3)
    A=data(5);    I=matrix([1 2 2 2 1 1],1,3,2);
    if or(A(2,[2 1])<>[A(2,2),A(2,1)]) then linenumber();pause,end
    if or(A(2,[2 1;1 2])<>[A(2,2),A(2,1),A(2,1),A(2,2)]) then linenumber();pause,end

    if or(A(I,I)<>A(I(:),I(:))) then linenumber();pause,end
    if or(A(1,[1;1])<>[A(1,1) A(1,1)]) then linenumber();pause,end
    if or(A(:,[1 3])<>[A(1,1) A(1,3);A(2,1) A(2,3)]) then linenumber();pause,end
    if or(A(1:$,[1 3])<>[A(1,1) A(1,3);A(2,1) A(2,3)]) then linenumber();pause,end
    if or(A([%f %t],:)<>[A(2,1) A(2,2)  A(2,3)]) then linenumber();pause,end
    if or(A([%f;%t;%f],:)<>[A(2,1) A(2,2)  A(2,3)]) then linenumber();pause,end
    if or(A([%f %t],[%t %t %t])<>[A(2,1) A(2,2)  A(2,3)]) then linenumber();pause,end
    if or(A([%f %t],[%t %t %t %f])<>[A(2,1) A(2,2)  A(2,3)]) then linenumber();pause,end

    if execstr('A(3,[2 1])',"errcatch")==0 then linenumber();pause,end
    if execstr('A(2,[1 2 4])',"errcatch")==0 then linenumber();pause,end
    if execstr('A([%f %f %t],[1 2])',"errcatch")==0 then linenumber();pause,end
    if execstr('A(2,[%f %t %t %t])',"errcatch")==0 then linenumber();pause,end
    if execstr('A(2,$+1)',"errcatch")==0 then linenumber();pause,end
    if execstr('A(2,1:($+1))',"errcatch")==0 then linenumber();pause,end
    if execstr('A(0,1)',"errcatch")==0 then linenumber();pause,end
    if execstr('A(1,[1 0 3])',"errcatch")==0 then linenumber();pause,end
    if execstr('A(1,1,2)',"errcatch")==0 then linenumber();pause,end
    
    for I3=list(1,%t,:,1:$)
      if or(A(2,[2 1],I3)<>[A(2,2),A(2,1)]) then linenumber();pause,end
      if or(A(2,[2 1;1 2],I3)<>[A(2,2),A(2,1),A(2,1),A(2,2)]) then linenumber();pause, end
      if or(A(I,I,I3)<>A(I(:),I(:))) then linenumber();pause,end
      if or(A(1,[1;1],I3)<>[A(1,1) A(1,1)]) then linenumber();pause,end
      if or(A(:,[1 3],I3)<>[A(1,1) A(1,3);A(2,1) A(2,3)]) then linenumber();pause,end
      if or(A(1:$,[1 3],I3)<>[A(1,1) A(1,3);A(2,1) A(2,3)]) then linenumber();pause,end
      if or(A([%f %t],:,I3)<>[A(2,1) A(2,2)  A(2,3)]) then linenumber();pause,end
      if or(A([%f;%t;%f],:,I3)<>[A(2,1) A(2,2)  A(2,3)]) then linenumber();pause,end
      if or(A([%f %t],[%t %t %t],I3)<>[A(2,1) A(2,2)  A(2,3)]) then linenumber();pause,end
      if or(A([%f %t],[%t %t %t %f],I3)<>[A(2,1) A(2,2)  A(2,3)]) then linenumber();pause,end

      if execstr('A(1,1,2,I3)',"errcatch")==0 then linenumber();pause,end
    end
    
    // hypermatrix case (2 by 3 by 4)
    A=data($);I=matrix([1 2 2 2 1 1],1,3,2);
    if or(A(2,:)<>matrix(A(2:2:$),1,-1))  then linenumber();pause,end
    if or(A([%f %t],:)<>matrix(A(2:2:$),1,-1))  then linenumber();pause,end
    if or(A([%f %t %f],:)<>matrix(A(2:2:$),1,-1))  then linenumber();pause,end
    if or(A(2,1:$)<>matrix(A(2:2:$),1,-1))  then linenumber();pause,end
    if or(A(:,2:3)<>[A(1,2:3,1);A(2,2:3,1)])  then linenumber();pause,end
    if or(A(1:$,2:3)<>[A(1,2:3,1);A(2,2:3,1)])  then linenumber();pause,end
    if or(A(1:$,2:3,1)<>[A(1,2:3,1);A(2,2:3,1)])  then linenumber();pause,end
    if or(A(2,2,3)<>A(16))  then linenumber();pause,end  
    if or(A(2,2,3,1)<>A(16))  then linenumber();pause,end  
    if or(A(2,2,3,%t)<>A(16))  then linenumber();pause,end  
    if or(A(2,2,3,[%t %f %f])<>A(16))  then linenumber();pause,end  
    if or(A(2,2,3,:)<>A(16))  then linenumber();pause,end  
    if or(A(2,2,3,$)<>A(16))  then linenumber();pause,end  

    if execstr('A(3,:)',"errcatch")==0 then linenumber();pause,end
    if execstr('A(3,$+1)',"errcatch")==0 then linenumber();pause,end
    if execstr('A([%f %t %t],:)',"errcatch")==0 then linenumber();pause,end
    if execstr('A(1:$,2:3,5)',"errcatch")==0 then linenumber();pause,end
    if execstr('A(1:$,4,1)',"errcatch")==0 then linenumber();pause,end
    if execstr('A(:,:,1,5)',"errcatch")==0 then linenumber();pause,end
    
    if or(A(I,2,3)<>A(I(:),2,3))  then linenumber();pause,end  
    if or(A(1,I,3)<>A(1,I(:),3))  then linenumber();pause,end  
    if or(A(1,3,I)<>A(1,3,I(:)))  then linenumber();pause,end  
end 
endfunction
extraction_test()
