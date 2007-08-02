if resp<>[] then
  mode(1)	
  hz=iir(evstr(resp(2)),resp(1),resp(3),evstr(resp(4)),evstr(resp(5)));
  [hzm,fr]=frmag(hz,256);
  xselect();xbasc();
  plot2d(fr',hzm')
  a=gca();
  a.title.text='Discrete IIR filter '+'( '+ resp(3)+' ) '
  a.title.font_size=3;
end
