model VsourceAC "Sin-wave voltage source"
	Pin p, n;
	Real v;
	parameter Real VA = 220 "Amplitude";
	parameter Real f = 50 "Frequency";
	parameter Real PI=3.141592653589793;
equation
	v = VA* Math.sin(2*PI*f*time);
	v = p.v - n.v;
	0 = p.i + n.i;
end VsourceAC;


