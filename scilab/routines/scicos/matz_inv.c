# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
typedef struct FCOMPLEX {float r,i;} fcomplex;
extern int C2F(zgetrf)();
extern int C2F(zgetri)();
typedef struct
{         int *ipiv;
          double *wrk;
	  double *LX;
} mat_inv_struct ;
void matz_inv(scicos_block *block,int flag)
{
 double *ur;
 double *yr;
 double *ui;
 double *yi;
 int nu;
 int *ipiv;
 int info;
 int i,j;
 fcomplex D,l;
 mat_inv_struct *ptr;
 
 nu =GetInPortRows(block,1);
 ur=GetRealInPortPtrs(block,1);
 ui=GetImagInPortPtrs(block,1);
 yr=GetRealOutPortPtrs(block,1);
 yi=GetImagOutPortPtrs(block,1);
             /*init : initialization*/
if (flag==4)

      { *(block->work)=(mat_inv_struct*) malloc(sizeof(mat_inv_struct));
    ptr=*(block->work);
    ptr->ipiv=(int*) malloc(sizeof(int)*nu);
    ptr->wrk=(double*) malloc(sizeof(double)*(2*nu*nu));
    ptr->LX=(double*) malloc(sizeof(double)*(2*nu*nu));
      }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    free(ptr->ipiv);
    free(ptr->LX);
    free(ptr->wrk);
    free(ptr);

    return;
   }

else
   {
    ptr=*(block->work);
    for (i=0,j=0;i<(nu*nu),j<(2*nu*nu);i++,j+=2)   
	{ptr->LX[j]=ur[i];
	ptr->LX[j+1]=ui[i];}
     C2F(zgetrf)(&nu,&nu,ptr->LX,&nu,ptr->ipiv,&info);
    if (info !=0)
       {if (flag!=6)
    	{set_block_error(-7);
        return;}}
      C2F(zgetri)(&nu,ptr->LX,&nu,ptr->ipiv,ptr->wrk,&nu,&info);
	for (i=0,j=0;i<(nu*nu),j<(2*nu*nu);i++,j+=2)   
	   {yr[i]=ptr->LX[j];
	    yi[i]=ptr->LX[j+1];}
    }
 }
