#include <math.h>
#include "../machine.h"
#include "scicos_block4.h"

void gainblk_i16n(scicos_block *block,int flag)
{
 if ((flag==1)|(flag==6)){
  int i,j,l,ji,jl,il;
  short *u,*y;
  int mu,ny,my,mo,no;
  short *opar;
  double k,D,C,t;

  mo=GetOparSize(block,1,1);
  no=GetOparSize(block,1,2);
  mu=GetInPortRows(block,1);
  my=GetOutPortRows(block,1);
  ny=GetOutPortCols(block,1);
  u=Getint16InPortPtrs(block,1);
  y=Getint16OutPortPtrs(block,1);
  opar=Getint16OparPtrs(block,1);
  k=pow(2,16);
  if (mo*no==1){
    for (i=0;i<ny*mu;++i){
     D=(double)(opar[0])*(double)(u[i]);
     t=D-(int)(D/(k))*((k));
     if ((t>=k/2)|(-(t)>=k/2))
	 {if (t>=0) (t)=(-((k/2))+abs(t-((int)((t)/((k/2))))*((k/2))));
	  else (t)=-(-((k/2))+abs(t-((int)((t)/((k/2))))*((k/2))));}
     y[i]=(short)t;
    }
  }else{
     for (l=0;l<ny;l++)
	 {for (j=0;j<my;j++)
	      {D=0;
	       for (i=0;i<mu;i++)
		   {ji=j+i*my;
		    jl=j+l*my;
		    il=i+l*mu;
		    C=(double)(opar[ji])*(double)(u[il]);
		    D=D + C;}
		    t=D-(int)(D/(k))*((k));
		    if ((t>=k/2)|(-(t)>=k/2))
		       {if (t>=0) (t)=(-((k/2))+abs(t-((int)((t)/((k/2))))*((k/2))));
			else (t)=-(-((k/2))+abs(t-((int)((t)/((k/2))))*((k/2))));}
		    y[jl]=(short)t;
		  }
	     }
  }
 }
}
