# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
extern int C2F(zgesvd)();
typedef struct
{         double *l0;
	  double *LA;
	  double *LX;
          double *LU;
          double *LVT;
          double *dwork;
	  double *rwork;
} mat_sing_struct ;
void matz_sing(scicos_block *block,int flag)
{
 double *ur;
 double *ui;
 double *yr;
 double *yi;
 int nu,mu,M;
 int info;
 int i,j,rw,ii,lwork;
 mat_sing_struct *ptr;
 mu=GetInPortRows(block,1);
 nu=GetInPortCols(block,1);
 ur=GetRealInPortPtrs(block,1);
 ui=GetImagInPortPtrs(block,1);
 yr=GetRealOutPortPtrs(block,1);
 yi=GetImagOutPortPtrs(block,1);
 lwork=max(3*min(mu,nu)+max(mu,nu),5*min(mu,nu)-4);
 rw=5*min(mu,nu);
             /*init : initialization*/
if (flag==4)
   {*(block->work)=(mat_sing_struct*) malloc(sizeof(mat_sing_struct));
    ptr=*(block->work);
    ptr->l0=(double*) malloc(sizeof(double));
    ptr->LA=(double*) malloc(sizeof(double)*(2*mu*nu));
    ptr->LU=(double*) malloc(sizeof(double)*(2*mu*mu));
    ptr->LVT=(double*) malloc(sizeof(double)*(2*nu*nu));
    ptr->LX=(double*) malloc(sizeof(double)*(2*mu));
    ptr->dwork=(double*) malloc(sizeof(double)*2*lwork);
    ptr->rwork=(double*) malloc(sizeof(double)*2*rw);
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    free(ptr->l0);
    free(ptr->LA);
    free(ptr->LU);
    free(ptr->LX);
    free(ptr->LVT);
    free(ptr->rwork);
    free(ptr->dwork);
    free(ptr);
    return;
   }

else
   {
    ptr=*(block->work);
     for (i=0,j=0;i<(mu*nu),j<(2*mu*nu);i++,j+=2)   
	{ptr->LA[j]=ur[i];
	 ptr->LA[j+1]=ui[i];}
    C2F(zgesvd)("A","A",&mu,&nu,ptr->LA,&mu,yr,ptr->LU,&mu,ptr->LVT,&nu,ptr->dwork,&lwork,ptr->rwork,&info);
     if (info !=0)
       {if (flag!=6)
   	{set_block_error(-7);
        return;}}
    *(ptr->l0)=0;
    }
}
