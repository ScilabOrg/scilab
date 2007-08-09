xdel(0);
while %t

  exec(path+'lmi_ch.sce');
  [%demo_]=demo_choose(path+'lmi_ch.bin');

  select %demo_
  case 0
    return;
  case 1
    clc;
    mode(1)
    lmidem('SCI/demos/optimization/lmitool/normopt.sci');getf('SCI/demos/optimization/lmitool/normopt.sci');
    
    demo_message(path+'lmi_3.sce');

    [ok,A,B,C,D]=getvalue('Enter A, B, C, D matrices',['A';'B';'C';'D'],...
	list('mat',[3,3],'mat',[3,2],'mat',[2,3],'mat',[2,2]),...
	['[0,1,0;2,3,1;-1,-2,0]','[1,0;-2,1;0,1]','[1,2,0;0,1,-2]','[0,0;0,0]']);
    if ok then
      [X,gopt]=normopt(A,B,C,D);
      disp(gopt, 'optimal gama found is:')
      disp(gopt-h_norm(syslin('c',A,B,C,D)),...
	  'check: gopt-h_norm(syslin(''c'',A,B,C,D)=')
    end
  mode(-1)
  case 2
    clc;
    mode(1)
   lmidem('SCI/demos/optimization/lmitool/of.sci');getf('SCI/demos/optimization/lmitool/of.sci');
   
   demo_message(path+'lmi_4.sce');
    
    [ok,A,B,C]=getvalue('Edit below A, B, C matrices',['A';'B';'C'],...
	list('mat',[3,3],'mat',[3,2],'mat',[2,3]),...
	['[0,1,0;2,3,1;-1,-2,0]','[1,0;-2,1;0,1]','[1,2,0;0,1,-2]']);
    if ok then
      [P,Q]=of(A,B,C);
      disp(P,Q);
      end
    mode(-1)
  case 3
    clc;
    mode(1)
    lmidem('SCI/demos/optimization/lmitool/sylvester.sci');getf('SCI/demos/optimization/lmitool/sylvester.sci');
  
    demo_message(path+'lmi_4.sce');
    
    [ok,A,B,C]=getvalue('Edit below A, B, C matrices',['A';'B';'C'],...
	list('mat',[3,3],'mat',[2,2],'mat',[3,2]),...
	['[0,1,0;2,3,1;-1,-2,0]','[1,0;-2,1]','[1,2;0,1;1,-2]']);
    [X]=sylvester(A,B,C,'c');
    disp(X, 'X found is:')
    disp(A*X+X*B-C ,'Check: A*X+X*B-C =')
    mode(-1)
  end   
end
