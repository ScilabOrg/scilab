# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
#include <math.h>
extern int C2F(wexpm1)();
typedef struct
{	  int *iwork;
          double *dwork;
} mat_exp_struct ;
void matz_expm(scicos_block *block,int flag)
{
 double *ur,*ui;
 double *yr,*yi;
 int nu;
 int ierr;
 mat_exp_struct *ptr;

 nu =GetInPortCols(block,1);
 ur=GetRealInPortPtrs(block,1);
 ui=GetImagInPortPtrs(block,1);
 yr=GetRealOutPortPtrs(block,1);
 yi=GetImagOutPortPtrs(block,1);
             /*init : initialization*/
if (flag==4)
   {*(block->work)=(mat_exp_struct*) scicos_malloc(sizeof(mat_exp_struct));
    ptr=*(block->work);
    ptr->iwork=(int*) scicos_malloc(sizeof(int)*(2*nu));
    ptr->dwork=(double*) scicos_malloc(sizeof(double)*(nu*(4*nu+4*nu+7)));
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    scicos_free(ptr->iwork);
    scicos_free(ptr->dwork);
    scicos_free(ptr);
    return;
   }

else
   {ptr=*(block->work);
    C2F(wexpm1)(&nu,ur,ui,&nu,yr,yi,&nu,ptr->dwork,ptr->iwork,&ierr);
    if (ierr!=0)
	{if (flag!=6)
	{set_block_error(-7);
	return;}
	}
   }
}
