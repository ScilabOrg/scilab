['Open loop simulation'
	   ' '
	   ' y0 = [0;0;0.1;0]; //initial state [x x'' theta theta'']'
	   ' t = 0.03*(1:180); //observation dates'
	   ' y = ode(y0,0,0.03*(1:180),ivpd); //differential equation integration'
	   ' //Display'
	   ' P = initialize_display(y0(1),y0(3))'
	   ' for k=1:size(y,2), set_pendulum(P,y(1,k),y(3,k));end']
