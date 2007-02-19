#include "scicos_block4.h"
#include "../machine.h"

void selector_m(scicos_block *block,int flag)
{
  double *u;
  double *y;
  double *z;
  int nu,mu,ic,i,nev,nin;

  z=GetDstate(block);
  nin=GetNin(block);
  ic=(int)z[0];
  if (flag<3)
     {ic=0;
      nev=GetNev(block);
      while (nev>=1) 
           {
	    ic=ic+1;
	    nev=nev/2;
	   }
     }
  if (nin>1)
     {
      mu=GetInPortRows(block,ic);
      nu=GetInPortCols(block,ic);
      u=GetRealInPortPtrs(block,ic);
      y=GetRealOutPortPtrs(block,1);
      for(i=0;i<mu*nu;i++)
	 {*(y+i)=*(u+i);}
      }
   else
      {
       mu=GetInPortRows(block,1);
       nu=GetInPortCols(block,1);
       u=GetRealInPortPtrs(block,1);
       y=GetRealOutPortPtrs(block,ic);
       for(i=0;i<mu*nu;i++)
	 {*(y+i)=*(u+i);}
       }
}
