// This is just a graphic demo of the bike
// Copyright INRIA


clf();
curFig = gcf();
pix = curFig.pixmap;
curFig.figure_style = "new";
curFont = [curFig.children.font_style,curFig.children.font_size];
          
go_on=%t;

while go_on then
	
	exec('SCI/demos/simulation/bike/bike_ch.sce');
	[num]=demo_choose('SCI/demos/simulation/bike/bike_ch.bin');
	
	if num==0 then
		go_on=%f;
    clf();
		curFig.children.font_style = curFont(1);
		curFig.children.font_size = curFont(2);		
		curFig.pixmap = pix;
		
	else
		select num
		case 1 then //unstable		  
			getf('SCI/demos/simulation/bike/macros/'+'velo2.sci');
			x=read('SCI/demos/simulation/bike/data/'+'x1.dat',46,368,"(e10.4)");
			clf();
			xselect();
			
			toolbar(curFig.figure_id,"off");
			curAxe = gca();
			curAxe.title.font_style = curFont(1);
			curAxe.title.font_size = 3;
			curAxe.title.text = 'bike simulation, unstable trajectory';

			show(x(:,300:368),1,1,velo4p);
			toolbar(curFig.figure_id,"on");
		case 2 then //stable
		  getf('SCI/demos/simulation/bike/macros/'+'velod.sci');
			getf('SCI/demos/simulation/bike/macros/'+'velo1.sci');
			x=read('SCI/demos/simulation/bike/data/'+'xdemo.dat',46,401,"(e10.4)");
			clf();
			xselect();
			curFig = gcf();
			toolbar(curFig.figure_id,"off");
			curFig.pixmap = "on";
			curAxe = gca();
			curAxe.title.font_style = curFont(1);
			curAxe.title.font_size = 3;
			curAxe.title.text = 'bike simulation, stable trajectory';
			show(x(:,1:340),1.6,1.47,velo1);
			toolbar(curFig.figure_id,"on");
		end	
		
	end
end
