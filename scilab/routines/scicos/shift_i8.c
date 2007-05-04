# include "scicos_block4.h"
# include "../machine.h"
# include <math.h>

void shift_i8(scicos_block *block,int flag)
{
  char *u,*y,v; 
  int *ipar;
  int mu,nu,i,j;

  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  u=Getint8InPortPtrs(block,1);
  y=Getint8OutPortPtrs(block,1);
  ipar=GetIparPtrs(block);
  if(ipar[0]>=0)
      {for (i=0;i<mu*nu;i++)
	    {if (ipar[1]==0)
            	{y[i]=u[i]<<ipar[0];}
	     else{
		 v=u[i];
		 for(j=0;j<ipar[0];j++)
		     {
		      y[i]=v&128;
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
			  y[i]=y[i]&(127);}
		      else {
		          y[i]=v>>1;
		          y[i]=(y[i])|(128);
		          }
		      v=y[i];
		      }
                   }
              }
      }
}
