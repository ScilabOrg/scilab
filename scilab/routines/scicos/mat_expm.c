# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
#include <math.h>
extern int C2F(dexpm1)();
typedef struct
{	  int *iwork;
          double *dwork;
} mat_exp_struct ;
void mat_expm(scicos_block *block,int flag)
{
 double *u;
 double *y;
 int nu;
 int ierr;
 mat_exp_struct *ptr;

 nu =GetInPortCols(block,1);
 u=GetRealInPortPtrs(block,1);
 y=GetRealOutPortPtrs(block,1);

             /*init : initialization*/
if (flag==4)
   {*(block->work)=(mat_exp_struct*) malloc(sizeof(mat_exp_struct));
    ptr=*(block->work);
    ptr->iwork=(int*) malloc(sizeof(int)*(2*nu));
    ptr->dwork=(double*) malloc(sizeof(double)*(nu*(2*nu+2*nu+5)));
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    free(ptr->iwork);
    free(ptr->dwork);
    free(ptr);
    return;
   }

else
   {ptr=*(block->work);
    C2F(dexpm1)(&nu,&nu,u,y,&nu,ptr->dwork,ptr->iwork,&ierr);
    if (ierr!=0)
	{if (flag!=6)
	{set_block_error(-7);
	return;}
	}
   }
}
