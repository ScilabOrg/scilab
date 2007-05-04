# include "scicos_block4.h"
# include "../machine.h"
# include <math.h>

void shift_i32(scicos_block *block,int flag)
{
  long *u,*y,v;
  unsigned long k; 
  int *ipar;
  int mu,nu,i,j;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  u=Getint32InPortPtrs(block,1);
  y=Getint32OutPortPtrs(block,1);
  ipar=GetIparPtrs(block);
  k=pow(2,31);
  if(ipar[0]>=0)
      {for (i=0;i<mu*nu;i++)
	    {if (ipar[1]==0)
            	{y[i]=u[i]<<ipar[0];}
	     else{
		 v=u[i];
		 for(j=0;j<ipar[0];j++)
		     {
		      y[i]=v&k;
                      if (y[i]==0)  y[i]=v<<1;
		      else {
		          y[i]=v<<1;
		          y[i]=(y[i])|(1);
		          }
		      v=y[i];
		      }
                   }
              }
       }
  else
      {for (i=0;i<mu*nu;i++)
            {if (ipar[1]==0)
            	{y[i]=u[i]>>-ipar[0];}
	     else{
		 v=u[i];
		 for(j=0;j<-ipar[0];j++)
		     {
		      y[i]=v&1;
                      if (y[i]==0)
			 {y[i]=v>>1;
			  y[i]=y[i]&(k-1);}
		      else {
		          y[i]=v>>1;
		          y[i]=(y[i])|(k);
		          }
		      v=y[i];
		      }
                   }
              }
      }
}
