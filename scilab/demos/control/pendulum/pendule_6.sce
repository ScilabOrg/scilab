['Synthesis of a stabilizing controller using poles placement'
	   ' '
	   '// only x and theta are observed  : contruction of an observer'
	   '// to estimate the state : z''=(f-k*h)*z+k*y+g*u'
	   ' to=0.1; ' 
	   ' k=ppol(f'',h'',-ones(4,1)/to)''  //observer gain'
	   '// compute the compensator gain'
	   ' kr=ppol(f,g,-ones(4,1)/to)']
