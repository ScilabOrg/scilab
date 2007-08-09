// Copyright INRIA

mode(1)
s=poly(0,'s');

exec('SCI/demos/control/mixed/mixed_dial_1.sce');
[rep]=demo_mdialog('SCI/demos/control/mixed/mixed_dial_1.bin');

if rep==[] then return,end
Plant=evstr(rep);Plant=syslin('c',Plant);
 //Nominal Plant
P22=tf2ss(Plant);    //...in state-space form
[ny,nu,nx]=size(P22);
[Pms,r]=augment(P22);
while %t

  exec('SCI/demos/control/mixed/mixed_dial_2.sce');
  [rep]=demo_mdialog('SCI/demos/control/mixed/mixed_dial_2.bin');

  if rep==[] then return;end
  W1=evstr(rep(1));W2=evstr(rep(2));W3=evstr(rep(3));
  default=rep;
  Pms=sysdiag(W1,W2,W3,eye(P22))*Pms;

  exec('SCI/demos/control/mixed/mixed_dial_3.sce');
  [reps]=demo_mdialog('SCI/demos/control/mixed/mixed_dial_3.bin');
  
  if reps==[] then return;end
  mumin=1/evstr(reps(2))^2;
  mumax=1/evstr(reps(1))^2;
  iter=evstr(reps(3));
  [K,mu]=h_inf(Pms,r,mumin,mumax,iter);
  if K~=[] then break;end
end

disp(spec(h_cl(Pms,r,K)),'closed loop eigenvalues')    //Check internal stability
[Ssens,Rsens,Tsens]=sensi(P22,K);  //Sensitivity functions

demo_message(path+'mixed_4.sce');

exec(path+'mixed_dial_4.sce');
[www]=demo_mdialog(path+'mixed_dial_4.bin');

if www==[] then return,end

ww1=part(www(1),1)=='Y';
if ww1 then
  scf(1);clf();xselect();
  gainplot(Ssens);
  a=gca();
  a.title.text='S = sensitivity function';
  a.title.font_size=3;
end

ww2=part(www(2),1)=='Y';
if ww2 then
  scf(2);clf();xselect();
  gainplot(Rsens);
    a=gca();
  a.title.text='R (=G*S) = sensitivity function';
  a.title.font_size=3;
end

ww3=part(www(3),1)=='Y';
if ww3 then
  scf(3);clf();xselect();
  gainplot(Tsens);
  a=gca();
  a.title.text='T = complementary sensitivity function';
  a.title.font_size=3;
end
