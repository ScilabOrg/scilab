['Checking the result'
	   ' //comparison with linear model computed by hand';
	   ''
	   ' f1=[0 1        0                0'
	   '     0 0    -3*mb*9.81/m         0'
	   '     0 0        0                1'
	   '     0 0  6*(mb+mc)*9.81/(m*l)   0];'
	   ' '
	   ' g1=[0 ; 4/m ; 0 ; -6/(m*l)];'
	   ' '
	   ' h1=[1 0 0 0'
	   '     0 0 1 0];'
	   ' '
	   ' err=norm(f-f1,1)+norm(g-g1,1)+norm(h-h1,1)+norm(j,1)']
