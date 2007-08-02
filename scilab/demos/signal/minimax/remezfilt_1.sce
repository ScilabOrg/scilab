////////////////////////
//MINIMAX FIR FILTERS///
////////////////////////
// Copyright INRIA
xbasc();xselect();
mode(1)
clc;
//Low pass filter (length 33, cut-off in [.23,.27], weight 1/.1)

   hn=eqfir(33,[0 .23;.27 .5],[1 0],[1 .1]);
   [hm,fr]=frmag(hn,256);
   plot2d(fr',hm'),
   a=gca();
   a.title.text=['Minimax FIR filter';
 	  'low pass  cut-off in [.23,.27]';
    'length 33']';
   a.title.font_size=3;
   x_message('Click Ok to continue.');
   xbasc();
 
//Triangular filter created using remezb
   nc=21;
   ngrid=nc*16;
   fg=.5*(0:(ngrid-1))/(ngrid-1);
   ds(1:ngrid/2)=(0:-1+ngrid/2)*2/(ngrid-2);
   ds(ngrid/2+1:ngrid)=ds(ngrid/2:-1:1);
   wt=ones(fg);
   [an]=remezb(nc,fg,ds,wt);
   hn(1:nc-1)=an(nc:-1:2)/2;
   hn(nc)=an(1);
   hn(nc+1:2*nc-1)=an(2:nc)/2;
   [hm,fr]=frmag(hn,maxi(size(ds)));
   plot2d(fr',hm'),
   a=gca();
   a.title.text='Remez triangular filter';
   a.title.font_size=3
   a.x_label.text='frequency';
   a.y_label.text='magnitude';
   x_message('Click Ok to continue.');
   xbasc(); 
   plot2d(fr',(hm-ds)'),
   xtitle(' ','frequency','magnitude error');
   x_message('Click Ok to end.');


