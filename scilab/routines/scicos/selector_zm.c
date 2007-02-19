#include "scicos_block4.h"
#include "../machine.h"

void selector_zm(scicos_block *block,int flag)
{
  double *ur,*ui;
  double *yr,*yi;
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
      ur=GetRealInPortPtrs(block,ic);
      ui=GetImagInPortPtrs(block,ic);
      yr=GetRealOutPortPtrs(block,1);
      yi=GetImagOutPortPtrs(block,1);
      for(i=0;i<mu*nu;i++)
	 {*(yr+i)=*(ur+i);
          *(yi+i)=*(ui+i);
         }
      }
   else
      {
       mu=GetInPortRows(block,1);
       nu=GetInPortCols(block,1);
       ur=GetRealInPortPtrs(block,1);
       ui=GetImagInPortPtrs(block,1);
       yr=GetRealOutPortPtrs(block,ic);
       yi=GetImagOutPortPtrs(block,ic);
       for(i=0;i<mu*nu;i++)
	 {*(yr+i)=*(ur+i);
          *(yi+i)=*(ui+i);
         }
       }
}
