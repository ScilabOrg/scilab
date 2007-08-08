function [Ts,bllst,corinv,ind,ok]=sample_clk(MAT,Ts,bllst,corinv,scs_m,ind)
//   [num]=x_choose(['event select';'multiple frequency'],..
//                    ["You have to choose a method for the sample time computation:";..
//                     "The first method is a periodic synchronize system";..
//                     "it uses the event select block to generate events"; "at every clock time (regular period)";..
//                     "The number of outputs is equal to the least common"; "multiple of the input frequencies ";..
//                     "The second method uses the multi-frequency block";..
//                     "it generates an event only on the used time";..
//                     "The default value is select method"])
  //num=scs_m.props.tol(8)
  num=2; // no choice to be done.For the next version the choice will be done in adding a new field in "Simulate/Setup"
  if num==2 then
  [Ts,bllst,corinv,ind,ok]=s_clk2(MAT,Ts,bllst,corinv,scs_m,ind)
  else
  [frequ,offset,freqdiv,den,flg,ok]=clock_major(MAT)
  if ok then 
    [Ts,corinv,bllst,ind,ok]=update_diag(scs_m,corinv,Ts,frequ,offset,freqdiv,bllst,den,flg,ind)
  end
  end
endfunction
 
function [frequ,offset,freqdiv,den,flg,ok]=clock_major(MAT)
//modification to support double
ok=%t;
flg=1;
index=find(MAT(:,5)==string(4));
if index<>[] then
     freq1=evstr(MAT(index,3));
     offset1=evstr(MAT(index,4));
end
freqdiv=unique(freq1);
if size(unique(offset1),'*')==1 then
    if size(freqdiv,'*')==1 then flg=0;end
    [frequ,den]=fixedpointgcd(freq1);
    offset=offset1(1);
else
   [frequ,den]=fixedpointgcd([freq1;offset1]);
   offset=0;
end
freqdiv=uint32(freqdiv*double(den));
frequ=double(frequ)/double(den);
if frequ==[] then frequ=0;end
if offset==[] then offset=0; end
if (offset > frequ) then
   offset=modulo(offset,frequ)
   if (offset~=0) then ok=%f; end
end
endfunction

function [Ts,corinv,bllst,ind,ok]=update_diag(scs_m,corinv,Ts,frequ,offset,freqdiv,bllst,den,flg,ind)
//modification to support double
ok=%t
n=lstsize(scs_m.objs)
bllst($+1)=scicos_model(sim=list("evtdly4",4),in=[],in2=[],intyp=1,out=[],out2=[],..
		     outtyp=1,evtin=1,evtout=1,state=[],dstate=[],odstate=list(),..
		     rpar=[frequ;offset],ipar=[],opar=list(),blocktype="d",firing=offset,..
		     dep_ut=[%f,%f],label="",nzcross=0,nmode=0,equations=list());
corinv($+1)=n+1
nc=size(corinv)
ind($+1)=nc;
nb=size(bllst);
Ts($+1:$+2,:)=[nb 1 -1 -1;..
		nb 1 1  -1]
if flg then
   
   nn=lcm(freqdiv)
   bllst($+1)=scicos_model(sim=list("counter",4),in=[],in2=[],intyp=1,out=1,out2=1,..
		     outtyp=1,evtin=1,evtout=[],state=[],dstate=0,odstate=list(),..
		     rpar=[],ipar=[1;double(nn);1],opar=list(),blocktype="c",firing=[],..
		     dep_ut=[%f,%f],label="",nzcross=0,nmode=0,equations=list());
    bllst($+1)=scicos_model(sim=list("eselect",-2),in=1,in2=1,intyp=-1,out=[],out2=[],..
		     outtyp=1,evtin=[],evtout=ones(nn,1),state=[],dstate=[],odstate=list(),..
		     rpar=[],ipar=[],opar=list(),blocktype="l",firing=-ones(nn,1),..
		     dep_ut=[%t,%f],label="",nzcross=0,nmode=0,equations=list());
    corinv($+1)=n+2
    corinv($+1)=n+3
    nb=size(bllst)
    nc=size(corinv)
    ind($+1)=nc-1;
    ind($+1)=nc;
    Ts($+1:$+4,:)=[nb-2 1 -1 -1;..
	 	    nb-1,1,1,-1;..
		    nb-1,1,-1,1;..
		    nb,1,1,1]
    index=find(MAT(:,5)==string(4))
    for i=index
         num=-evstr(MAT(i,1))
    	 Ts(find(Ts(:,1)==num),1)=-num
    	 K=0:nn-1;
    	 M=find(modulo(K,(evstr(MAT(i,3))*double(den)))==0)';
    	 ON=ones(size(M,'*'),1)
    	 Ts($+1:2:$+2*size(M,'*'),:)=[nb*ON M+ON*(evstr(MAT(i,4))-offset)*double(den) -ON -ON]
    	 N=[1:size(M,'*')]';
    	 Ts($+1-(2*size(M,'*')-2):2:$+1,:)=[-num*ON N ON -ON]
    end
else
    nb=size(corinv)
    index=find(MAT(:,5)==string(4))
    ON=ones(size(index,'*'),1)
    Ts($+1:2:$+2*size(index,'*'),:)=[nb*ON ON -ON -ON]
    num=-evstr(MAT(find(index),1))
 //   N=[1:size(index,'*')]';
    Ts($+1-(2*size(index,'*')-2):2:$+1,:)=[-num ON ON -ON]
    for i=index
    	 Ts(find(Ts(:,1)==num(i)),1)=-num(i)
    end
end
endfunction

// This function computes the PGCD of a double vector.
function [value,denom_com]=fixedpointgcd(v)
[N,D]=rat(v,1d-5);
denom_com=lcm(uint32(D));
N=uint32(N)*denom_com./uint32(D);
value=gcd(N);
endfunction

function [Ts,bllst,corinv,ind,ok]=s_clk2(MAT,Ts,bllst,corinv,scs_m,ind)
  ok=%t
  index=find(MAT(:,5)==string(4))
  MAT1=MAT(index,:);
  frequ=evstr(MAT1(:,3));
  offset=evstr(MAT1(:,4));
  offset=offset(:);frequ=frequ(:);
  [pgcd,den]=fixedpointgcd([frequ;offset]);
   off=0;
    [m1,k]=uni(frequ,offset);
    frd=uint32(m1.*double(den))
    frequ=frequ(k);
    frd1=uint32((frequ.*double(den)));
    ppcm=lcm(frd1);
    frd1=double(frd1);
    frd=double(frd);
    offset=offset(k);
    if size(frd,'*')>1 then
    mat=[];
    for i=1:size(frd,'*')
         mat1=[offset(i)*double(den):frd1(i):double(ppcm)]';
         mat=[mat;[mat1 2^(i-1)*ones(size(mat1,'*'),1)]];
    end
    [n,k]=gsort(mat(:,1),'g','i');
    mat=mat(k,:);
    while (find(mat(1:$-1,1)==mat(2:$,1))<>[]) then
           ind=find(mat(1:$-1,1)==mat(2:$,1));
           ind=ind(1);
	   mat(ind,2)=mat(ind,2)+mat(ind+1,2);
           mat(ind+1,:)=[];
    end
    m=[mat(2,1);mat(2:$,1)-mat(1:$-1,1)];
    m=[m,mat(:,2),mat(:,1)];
    count=1;
    if find(m(:,3)==0)<>[] then
       m(find(m(:,3)==0),:)=[];
       count=0;
    end
    mn=(2**size(m1,'*'))-1;
    fir=-ones(1,mn);
    fir(mat(1,2))=mat(1,1)/double(den);
    else
    m=[frd1 1 frd1];
    mat=m;
    count=0;
    off=offset;
    fir=off;
    end
    mn=(2**size(m1,'*'))-1;
    n=lstsize(scs_m.objs);
    bllst($+1)=scicos_model(sim=list("m_frequ",4),in=[],in2=[],intyp=1,out=[],out2=[],outtyp=1,..
                       evtin=1,evtout=ones(mn,1),state=[],dstate=[],odstate=list(),rpar=[],ipar=[],..
                       opar=list(m,double(den),off,count),blocktype="d",firing=fir,dep_ut=[%f,%f],..
                       label="",nzcross=0,nmode=0,equations=list());
   corinv($+1)=n+1;
   nb=size(corinv);
   nc=size(bllst);
   ind($+1)=nc;

   k=1:mn;
   Ts($+1:2:$+2*mn,:)=[nb*ones(mn,1) k' -ones(mn,2)]
   Ts($+1-(2*mn-2):2:$+1,:)=[nb*ones(mn,1) ones(mn,2) -ones(mn,1)]
   for i=1:size(frequ,'*')
        num=evstr(MAT1(find((evstr(MAT1(:,3))==frequ(i))&(evstr(MAT1(:,4))==offset(i))),1))
       for ii=num'
        Ts(find(Ts(:,1)==-ii),1)=ii;
        j=2**(i-1):2**i:mn;
        v=j;
        for k=1:2**(i-1)-1;
             v=[v,j+k]
        end
        v=(unique(v))
        ON=ones(size(v,'*'),1)
        N=[1:size(v,'*')]';
        Ts($+1:2:$+2*size(v,'*'),:)=[nb*ON v' -ON -ON]
        Ts($+1-(2*size(v,'*')-2):2:$+1,:)=[ii*ON N ON -ON]
       end
    end
endfunction

function [m,k]=uni(fr,of)
k=[];
m=[];
for i=1:size(fr,'*')
     ind=find(m==fr(i));
     if ind==[] then
         m=[m;fr(i)];
         k=[k;i];
     else
         for j=ind
              if of(i)<> of(j) then
                 m=[m;fr(i)];
                 k=[k;i];
              end
         end
      end
end
endfunction 
