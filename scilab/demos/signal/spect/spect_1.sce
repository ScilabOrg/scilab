////////////////////////
//SPECTRAL ESTIMATION///
////////////////////////
mode(1)
clc;
// Copyright INRIA
xselect();clf(); 
 
//generate white data
   rand('normal');
   rand('seed',0);
   x=rand(1:1024-33+1);
 
//make low-pass filter with eqfir
   nf=33;
   bedge=[0 .1;.125 .5];
   des=[1 0];
   wate=[1 1];
   h=eqfir(nf,bedge,des,wate);
 
//filter white data to obtain colored data
   h1=[h 0*ones(1:maxi(size(x))-1)];
   x1=[x 0*ones(1:maxi(size(h))-1)];
   hf=fft(h1,-1);
   xf=fft(x1,-1);
   yf=hf.*xf;
   y=real(fft(yf,1));
 
//plot magnitude of filter
   h2=[h 0*ones(1:167)];
   hf2=fft(h2,-1);
   hf2=real(hf2.*conj(hf2));
   hsize=maxi(size(hf2));
   fr=(1:hsize)/hsize;
   plot2d(fr',log(hf2)');
   a=gca();
   a.title.text='data spectrum';
   a.x_label.text="frequency";
   a.y_label.text="magnitude";
   a.title.font_size=3;
   x_message('Click OK to continue.');
   ;clf(); 
 
//pspect example
   [sm1]=pspect(100,200,'tr',y);
   smsize=maxi(size(sm1));
   fr=(1:smsize)/smsize;
   plot2d(fr',log(sm1)')
   a=gca();
   a.title.text='spectral estimation';
   a.x_label.text="frequency";
   a.y_label.text="spectral power";
   a.title.font_size=3;
   x_message('Click OK to continue.');
   clf(); 
 
//cspect example
   [sm2]=cspect(100,200,'tr',y);
   smsize=maxi(size(sm2));
   fr=(1:smsize)/smsize;
   plot2d(fr',log(sm2)');
   a=gca();
   a.title.text='spectral estimation, periodogram method';
   a.title.font_size=3;
   x_message('Click OK to end.');;
