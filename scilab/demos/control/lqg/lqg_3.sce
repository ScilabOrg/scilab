// Copyright INRIA

exec('SCI/demos/control/lqg/lqg_ch_1.sce');
[num]=demo_choose('SCI/demos/control/lqg/lqg_ch_1.bin');

select num
	case 0
		return
	case 1
		mode(1)
		s=poly(0,'s');
		
		exec('SCI/demos/control/lqg/lqg_dial_1.sce');
		[rep]=demo_mdialog('SCI/demos/control/lqg/lqg_dial_1.bin');
		
		if rep==[] then return,end
		Plant=evstr(rep);
		Plant=syslin('c',Plant);
		mode(-1)
	case 2
		mode(1)
		z=poly(0,'z');
		
		exec('SCI/demos/control/lqg/lqg_dial_2.sce');
		[rep]=demo_mdialog('SCI/demos/control/lqg/lqg_dial_2.bin');
		
		if rep==[] then return,end
		Plant=evstr(rep);
		Plant=syslin('d',Plant);
		mode(-1)
end

mode(1)

//Nominal Plant

P22=tf2ss(Plant);    //...in state-space form
[ny,nu,nx]=size(P22);

demo_message('SCI/demos/control/lqg/lqg_4.sce');

rep=x_matrix('x-weighting matrix',eye(nx,nx))
if rep==[] then return,end
Qx=evstr(rep);
rep=x_matrix('u-weighting matrix',eye(nu,nu));
if rep==[] then return,end
Qu=evstr(rep);
bigQ=sysdiag(Qx,Qu);
rep=x_matrix('x-noise covariance matrix',eye(nx,nx))
if rep==[] then return,end
Rx=evstr(rep);
rep=x_matrix('y-noise covariance matrix',eye(ny,ny))
if rep==[] then return,end
Ry=evstr(rep);
bigR=sysdiag(Rx,Ry);
[Plqg,r]=lqg2stan(P22,bigQ,bigR);     //LQG pb as a standard problem
Klqg=lqg(Plqg,r);                     //LQG compensator

disp(spec(h_cl(Plqg,r,Klqg)),'closed loop eigenvalues:');    //Check internal stability
[Slqg,Rlqg,Tlqg]=sensi(P22,Klqg);  //Sensitivity functions

disp(clean(ss2tf(Slqg)),'Sensitivity function');
disp(clean(ss2tf(Tlqg)),'Complementary sensitivity function');

mode(-1);

demo_message('SCI/demos/control/lqg/lqg_5.sce');

resp=['Frequency response';'Time response'];
while %t do

	exec('SCI/demos/control/lqg/lqg_ch_2.sce');
	[n]=demo_choose('SCI/demos/control/lqg/lqg_ch_2.bin');

	select n
		case 0
			disp("LQG demo stops!");break;
		case 1 then
			mode(1)
			clf(0);scf(0);xselect();bode(Tlqg);xend()
			mode(-1)
		case 2
			if Plant(4)=='c' then
			mode(1)
			
			exec('SCI/demos/control/lqg/lqg_dial_3.sce');
			[rep]=demo_mdialog('SCI/demos/control/lqg/lqg_dial_3.bin');
						
			if rep==[] then break,end
			dttmax=evstr(rep)
			dt=evstr(dttmax(1));tmax=evstr(dttmax(2));
			t=0:dt/5:tmax;
			
			exec('SCI/demos/control/lqg/lqg_ch_3.sce');
			[n1]=demo_choose('SCI/demos/control/lqg/lqg_ch_3.bin');
						
			select n1
				case 1 then
					scf(1);clf();xselect();
					plot2d([t',t'],[(csim('step',t,Tlqg))',ones(t')]);
				case 2 then
					scf(1);clf();xselect();
					plot2d([t',t'],[(csim('impul',t,Tlqg))',0*t']);
			end
			mode(-1)
		elseif Plant(4)=='d' then
			mode(1)
			
			exec('SCI/demos/control/lqg/lqg_dial_4.sce');
			[rep]=demo_mdialog('SCI/demos/control/lqg/lqg_dial_4.bin');
			
			if rep==[] then break,end
			Tmax=evstr(rep);
			mode(-1)
			
			exec('SCI/demos/control/lqg/lqg_ch_3.sce');
			[n2]=demo_choose('SCI/demos/control/lqg/lqg_ch_3.bin');
						
			select n2
				case 0 then
					break;
				case 1 then
					mode(1)
					u=ones(1,Tmax);u(1)=0;
					scf(1);clf();xselect();
					plot2d([(1:Tmax)',(1:Tmax)'],[(dsimul(Tlqg,u))',(ones(1:Tmax)')])
					a=gca();
					a.title.text='LQG, step response';
           a.title.font_size=3;
					mode(-1)
				case 2 then
					mode(1)
					u=zeros(1,Tmax);u(1)=1;
					scf(1);clf();xselect();
					plot2d((1:Tmax)',(dsimul(Tlqg,u))')
					a=gca();
					a.title.text='LQG, impulse response';
           a.title.font_size=3;
					mode(-1)
			end
		end
	end
end
