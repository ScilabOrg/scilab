// Copyright INRIA

s=poly(0,'s');z=poly(0,'z');

exec(path+'pid_ch_1.sce');
[n]=demo_choose(path+'pid_ch_1.bin');

select n
 case 0
  warning('Demo stops!');return;
 case 1
  mode(1)
  dom='c';
  s=poly(0,'s');
  
  exec(path+'pid_dial_2.sce');
  [rep]=demo_mdialog(path+'pid_dial_2.bin');
  
  if rep==[] then return,end
  Plant=evstr(rep); 
  Plant=syslin('c',Plant);
  mode(-1)
case 2
  mode(1)  
  dom='d'
  z=poly(0,'z');
  
  exec(path+'pid_dial_3.sce');
  [rep]=demo_mdialog(path+'pid_dial_3.bin');
  
  if rep==[] then return,end
  Plant=evstr(rep)
  Plant=syslin('d',Plant);
  mode(-1)
end   
//Nominal Plant
P22=tf2ss(Plant);    //...in state-space form
[ny,nu,nx]=size(P22);
defv=['-1.2','1','0.1'];
if dom=='d' then defv=['-10','1','0.1'];end
while %t
  mode(1)
  if dom=='c' then
    Title='Enter your PID controller K(s)=Kp*(1+T0/s+T1*s)';
  end
  if dom=='d' then
    Title='Enter your PID controller K(z)=Kp*(1+T0/z+T1*z)';
  end
  
  exec(path+'pid_dial_4.sce');
  [defv]=demo_mdialog(path+'pid_dial_4.bin');  
  
  if defv==[] then warning('Demo stops!');return;end
  Kp=evstr(defv(1));T0=evstr(defv(2));T1=evstr(defv(3));
  if dom=='c' then
    Kpid=tf2ss(Kp*(1+T0/s+T1*s));
  end
  if dom=='d' then
    Kpid=tf2ss(Kp*(1+T0/z+T1*z));
  end
  W=[1, -P22;
      Kpid,1];Winv=inv(W);

  disp(spec(Winv(2)),'closed loop eigenvalues');//Check internal stability
  if maxi(real(spec(Winv(2)))) > 0 then 
    demo_message(path+'pid_4.sce');
  else
    demo_message(path+'pid_5.sce');
    break;
  end
  mode(-1)
end
mode(1)
[Spid,Rpid,Tpid]=sensi(P22,Kpid);  //Sensitivity functions
Tpid(5)=clean(Tpid(5));

disp(clean(ss2tf(Spid)),'Sensitivity function');
disp(clean(ss2tf(Tpid)),'Complementary sensitivity function');

while %t do

  exec(path+'pid_ch_2.sce');
  [n]=demo_choose(path+'pid_ch_2.bin');

  if degree(Tpid(5))>0 then
    warning('Improper transfer function! D(s) set to D(0)')
    Tpid(5)=coeff(Tpid(5),0);
  end
  Tpid(5)=coeff(Tpid(5));
  select n
  case 0
    break
  case 1
    mode(1)
    xbasc(1);xset("window",1);xselect();bode(Tpid);
    mode(-1)
  case 2
    if Plant(4)=='c' then
      mode(1)
      
      exec('SCI/demos/control/pid/pid_dial_1.sce');
      [rep]=demo_mdialog('SCI/demos/control/pid/pid_dial_1.bin');
      
      if rep==[] then break,end
      dttmax=evstr(rep);
      dt=evstr(dttmax(1));tmax=evstr(dttmax(2));
      t=0:dt/5:tmax;

      exec(path+'pid_ch_3.sce');
      [n1]=demo_choose(path+'pid_ch_3.bin');

      if n1==0 then
	warning('Demo stops!');return;
      end
      if n1==1 then 
	xbasc(1);xset("window",1);xselect();
	plot2d([t',t'],[(csim('step',t,Tpid))',ones(t')])
  a=gca();
  a.title.text='PID, step response'
  a.title.font_size=3;
      end
      if n1==2 then
	xbasc(1);xset("window",1);xselect();
	plot2d([t',t'],[(csim('impul',t,Tpid))',0*t'])
	a=gca();
  a.title.text='PID, impulse response'
  a.title.font_size=3;
      end
      mode(-1)
      
    elseif Plant(4)=='d' then
      mode(1);
      
      exec('SCI/demos/control/pid/pid_dial_5.sce');
      [rep]=demo_mdialog('SCI/demos/control/pid/pid_dial_5.bin');

      if rep==[] then break,end
      Tmax=evstr(rep);
      mode(-1)
      while %t do
 
  exec(path+'pid_ch_4.sce');
  [n]=demo_choose(path+'pid_ch_4.bin');
  
	select n
	case 0 then
	  break
	case 1 then
	  mode(1)
	  u=ones(1,Tmax);u(1)=0;
	  xbasc(1);xset("window",1);xselect();
	  plot2d([(1:Tmax)',(1:Tmax)'],[(dsimul(Tpid,u))',(ones(1:Tmax)')])
	  a=gca();
    a.title.text='PID, step response'
    a.title.font_size=3;
	  
	  mode(-1)
	case 2 then
	  mode(1)
	  u=zeros(1,Tmax);u(1)=1;
	  xbasc(1);xset("window",1);xselect();
	  plot2d((1:Tmax)',(dsimul(Tpid,u))')
	  a=gca();
    a.title.text='PID, impulse response'
    a.title.font_size=3;
	  mode(-1)
	end
      end
    end
  end
end
