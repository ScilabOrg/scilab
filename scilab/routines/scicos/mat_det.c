# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
extern int C2F(dgetrf)();
typedef struct
{         int *ipiv;
          double *wrk;
} mat_det_struct ;
void mat_det(scicos_block *block,int flag)
{
 double *u;
 double *y;
double *v;
 int nu;
 int *ipiv;
 int info;
 int i;
 double D,l;
 mat_det_struct *mdet;
 
 nu =GetInPortRows(block,1);
 u=GetRealInPortPtrs(block,1);
 y=GetRealOutPortPtrs(block,1);

             /*init : initialization*/
if (flag==4)

      { *(block->work)=(mat_det_struct*) malloc(sizeof(mat_det_struct));
    mdet=*(block->work);
    mdet->ipiv=(int*) malloc(sizeof(int)*nu);
    mdet->wrk=(double*) malloc(sizeof(double)*(nu*nu));

      }

       /* Terminaison */
else if (flag==5)
   {mdet=*(block->work);
    free(mdet->ipiv);
    free(mdet->wrk);
    free(mdet);

    return;
   }

else
   {
    mdet=*(block->work);
    for (i=0;i<(nu*nu);i++)   mdet->wrk[i]=u[i];
     C2F(dgetrf)(&nu,&nu,mdet->wrk,&nu,mdet->ipiv,&info);
    if (info !=0)
       {if (flag!=6)
    	{set_block_error(-7);
        return;}}
      D=1;
    for (i=0;i<nu;i++)
       {if((*(mdet->ipiv+i))!=i+1) D=-D;
        l=*(mdet->wrk+i*(nu+1));
        D=D*l;
        };
     *y=D;
    }
 }
