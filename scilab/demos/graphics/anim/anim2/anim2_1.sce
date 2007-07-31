mode(0);

// Hide the toolbar
toolbar(0,'off');
curFont=xget('font');

// Save the graphic context
kp = xget('pixmap');
	
	xset('pixmap',1);
	
	xselect(); //raise graphic window
	np=10;
	t=0:0.1:np*%pi;
	
	if driver()=='Pos' then
	  st=1.5;
	else
	  st=0.5;
	end
	
	I=1:st:30
	
	realtimeinit(0.03);

	for i=1:size(I,'*')
	  realtime(i)
	  xset('wwpc');
	  param3d((t/(np*%pi)*%pi).*sin(t),(t/(np*%pi)*%pi).*cos(t),I(i)*t/(np*%pi),35,45,'X@Y@Z',[2,4]);
  xset("font",curFont(1),3);
  xtitle("3d curve");
	  xset('wshow');
	end
	
xset("pixmap",0);

// Restore the graphic context
xset('pixmap',kp);
curFont=xset("font",curFont(1),curFont(2))

mode(-1);
