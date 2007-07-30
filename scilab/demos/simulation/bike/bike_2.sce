demolist=['bike : unstable trajectory';
          'bike : stable trajectory!'];

while go_on then
	
	num=tk_choose(demolist(:,1),'Choose a bike');
	if num==0 then
		go_on=%f;
    set('figure_style',fs);
	else
		select num
		case 1 then 
			getf(path+'macros/'+'velo4p.sci');
			//getf(path+'macros/'+'velo5.sci');
			x=read(path+'data/x1.dat',46,368,"(e10.4)");
			set figure_style old;
			xbasc();
			xset("font",curFont(1),3);
			xtitle('bike simulation, unstable trajectory');
			show(x(:,300:368),1,1,velo4p);
		case 2 then 
			getf(path+'macros/'+'velo1.sci');
			x=read(path+'data/'+'xdemo.dat',46,401,"(e10.4)");
			set figure_style old;
			xbasc();
			xset("font",curFont(1),3);
			xtitle('bike simulation, stable trajectory');
			show(x(:,1:340),1.6,1.47,velo1);
		end
		xset('font',curFont(1),curFont(2));
	end
end
