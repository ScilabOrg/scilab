function y=setw(x)
 mu=size(x,1)
 for i=1:mu
     for j=1:mu
	 y(i)(j)=0
     end
 end
 for j=1:mu
     for i=1:mu-j+1
	 y(i)(j)=x(i+j-1)
     end
 end
endfunction