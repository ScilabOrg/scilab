# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
extern int C2F(dgesvd)();
extern int C2F(dlaset)();
extern int C2F(dlacpy)();

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

typedef struct
{         double *l0;
	  double *LA;
          double *LU;
          double *LVT;
          double *dwork;
} mat_sing_struct ;
void mat_sing(scicos_block *block,int flag)
{
 double *u;
 double *y;
 int nu,mu;
 int info;
 int i,j,ij,ji,ii,lwork;
 mat_sing_struct *ptr;
 mu=GetInPortRows(block,1);
 nu=GetInPortCols(block,1);
 u=GetRealInPortPtrs(block,1);
 y=GetRealOutPortPtrs(block,1);
 lwork=max(3*min(mu,nu)+max(mu,nu),5*min(mu,nu)-4); 
             /*init : initialization*/
if (flag==4)
   {*(block->work)=(mat_sing_struct*) malloc(sizeof(mat_sing_struct));
    ptr=*(block->work);
    ptr->l0=(double*) malloc(sizeof(double));
    ptr->LA=(double*) malloc(sizeof(double)*(mu*nu));
    ptr->LU=(double*) malloc(sizeof(double)*(mu*mu));
    ptr->LVT=(double*) malloc(sizeof(double)*(nu*nu));
    ptr->dwork=(double*) malloc(sizeof(double)*lwork);
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    free(ptr->l0);
    free(ptr->LA);
    free(ptr->LU);
    free(ptr->LVT);
    free(ptr->dwork);
    free(ptr);
    return;
   }

else
   {
    ptr=*(block->work);
    C2F(dlacpy)("F",&mu,&nu,u,&mu,ptr->LA,&mu);
    C2F(dgesvd)("A","A",&mu,&nu,ptr->LA,&mu,y,ptr->LU,&mu,ptr->LVT,&nu,ptr->dwork,&lwork,&info);
     if (info !=0)
       {if (flag!=6)
   	{set_block_error(-7);
         return;}}
    *(ptr->l0)=0;
   }
}
