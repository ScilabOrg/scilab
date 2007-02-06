# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
extern int C2F(zgesvd)();

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

typedef struct
{	  double *LA;
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
 int nu,mu;
 int info;
 int i,rw,lwork;
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
   {*(block->work)=(mat_sing_struct*) scicos_malloc(sizeof(mat_sing_struct));
    ptr=*(block->work);
    ptr->LA=(double*) scicos_malloc(sizeof(double)*(2*mu*nu));
    ptr->LU=(double*) scicos_malloc(sizeof(double)*(2*mu*mu));
    ptr->LVT=(double*) scicos_malloc(sizeof(double)*(2*nu*nu));
    ptr->LX=(double*) scicos_malloc(sizeof(double)*(2*mu));
    ptr->dwork=(double*) scicos_malloc(sizeof(double)*2*lwork);
    ptr->rwork=(double*) scicos_malloc(sizeof(double)*2*rw);
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    scicos_free(ptr->LA);
    scicos_free(ptr->LU);
    scicos_free(ptr->LX);
    scicos_free(ptr->LVT);
    scicos_free(ptr->rwork);
    scicos_free(ptr->dwork);
    scicos_free(ptr);
    return;
   }

else
   {
    ptr=*(block->work);
     for (i=0;i<(mu*nu);i++)
	{ptr->LA[2*i]=ur[i];
	 ptr->LA[2*i+1]=ui[i];}
    C2F(zgesvd)("A","A",&mu,&nu,ptr->LA,&mu,yr,ptr->LU,&mu,ptr->LVT,&nu,ptr->dwork,&lwork,ptr->rwork,&info);
     if (info !=0)
       {if (flag!=6)
   	{set_block_error(-7);
        return;}}
    }
}
