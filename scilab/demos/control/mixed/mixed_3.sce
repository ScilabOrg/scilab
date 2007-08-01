mode(1)
s=poly(0,'s');
str='[(s-1)/((s-1)^2*(s+2))]';
rep=x_dialog('Nominal plant?',str)
if rep==[] then return,end
Plant=evstr(rep);Plant=syslin('c',Plant);
 //Nominal Plant
P22=tf2ss(Plant);    //...in state-space form
[ny,nu,nx]=size(P22);
[Pms,r]=augment(P22);
txt=['W1 (sensitivity function S)';'W2 (K*S)';'W3 (complementary sensitivity T)'];
default=['s';'1';'1'];
while %t
  rep=x_mdialog('Set weighting functions',txt,default);
  if rep==[] then return;end
  W1=evstr(rep(1));W2=evstr(rep(2));W3=evstr(rep(3));
  default=rep;
  Pms=sysdiag(W1,W2,W3,eye(P22))*Pms;
  gms=['gamma min= ';'gamma max= ';'# iterations'];
  vals=['0.01';'1000';'50'];
  reps=x_mdialog('Set interval for gamma and #iterations',gms,vals);
  if reps==[] then return;end
  mumin=1/evstr(reps(2))^2;
  mumax=1/evstr(reps(1))^2;
  iter=evstr(reps(3));
  [K,mu]=h_inf(Pms,r,mumin,mumax,iter);
  if K~=[] then break;end
end

disp(spec(h_cl(Pms,r,K)),'closed loop eigenvalues')    //Check internal stability
[Ssens,Rsens,Tsens]=sensi(P22,K);  //Sensitivity functions
x_message('Singular values plot')
fcts=['S function';'R (=K*S) function';'T function'];
www=x_mdialog('Select sensitivity function',fcts,['Yes';'No';'Yes']);
if www==[] then return,end

ww1=part(www(1),1)=='Y';
if ww1 then
  scf(1);clf();xselect();
  gainplot(Ssens);
  //xtitle('S = Sensitivity function');
  a=gca();
  a.title.text='S = sensitivity function';
  a.title.font_size=3;
end

ww2=part(www(2),1)=='Y';
if ww2 then
  scf(2);clf();xselect();
  gainplot(Rsens);
  //xtitle('R (=G*S) Sensitivity function');
  a=gca();
  a.title.text='R (=G*S) = sensitivity function';
  a.title.font_size=3;
end

ww3=part(www(3),1)=='Y';
if ww3 then
  scf(3);clf();xselect();
  gainplot(Tsens);
  //xtitle('T = Complementary Sensitivity function');
  a=gca();
  a.title.text='T = complementary sensitivity function';
  a.title.font_size=3;
end
