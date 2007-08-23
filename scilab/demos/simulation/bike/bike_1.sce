// This is just a graphic demo of the bike
// Copyright INRIA

fs = get('figure_style');
curFont=xget("font");

xbasc();
          
go_on=%t;

while go_on then
	
	exec('SCI/demos/simulation/bike/bike_ch.sce');
	[num]=demo_choose('SCI/demos/simulation/bike/bike_ch.bin');
	
	if num==0 then
		go_on=%f;
    set('figure_style',fs);
	else
		select num
		case 1 then 
			getf('SCI/demos/simulation/bike/macros/'+'velo4p.sci');
			//getf(path+'macros/'+'velo5.sci');
			x=read('SCI/demos/simulation/bike/data/x1.dat',46,368,"(e10.4)");
			set figure_style old;
			xbasc();
			xset("font",curFont(1),3);
			xtitle('bike simulation, unstable trajectory');
			show(x(:,300:368),1,1,velo4p);
		case 2 then 
			getf('SCI/demos/simulation/bike/macros/'+'velo1.sci');
			x=read('SCI/demos/simulation/bike/data/'+'xdemo.dat',46,401,"(e10.4)");
			set figure_style old;
			xbasc();
			xset("font",curFont(1),3);
			xtitle('bike simulation, stable trajectory');
			show(x(:,1:340),1.6,1.47,velo1);
		end
		xset('font',curFont(1),curFont(2));
	end
end
