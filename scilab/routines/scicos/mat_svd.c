# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
extern int C2F(dgesvd)();
extern int C2F(dlaset)();
extern int C2F(dlacpy)();
extern int C2F(dmmul)();

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

typedef struct
{         double *l0;
	  double *LA;
          double *LSV;
          double *LVT;
          double *dwork;
} mat_sdv_struct ;
void mat_svd(scicos_block *block,int flag)
{
 double *u;
 double *y1,*y2,*y3;
 int nu,mu;
 int info;
 int i,j,ij,ji,ii,lwork;
 mat_sdv_struct *ptr;
 mu=GetInPortRows(block,1);
 nu=GetInPortCols(block,1);
 u=GetRealInPortPtrs(block,1);
 y1=GetRealOutPortPtrs(block,1);
 y2=GetRealOutPortPtrs(block,2);
 y3=GetRealOutPortPtrs(block,3);
 lwork=max(3*min(mu,nu)+max(mu,nu),5*min(mu,nu)-4);
             /*init : initialization*/
if (flag==4)
   {*(block->work)=(mat_sdv_struct*) scicos_malloc(sizeof(mat_sdv_struct));
    ptr=*(block->work);
    ptr->l0=(double*) scicos_malloc(sizeof(double));
    ptr->LA=(double*) scicos_malloc(sizeof(double)*(mu*nu));
    ptr->LSV=(double*) scicos_malloc(sizeof(double)*(min(mu,nu)));
    ptr->LVT=(double*) scicos_malloc(sizeof(double)*(nu*nu));
    ptr->dwork=(double*) scicos_malloc(sizeof(double)*lwork);
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    scicos_free(ptr->l0);
    scicos_free(ptr->LA);
    scicos_free(ptr->LSV);
    scicos_free(ptr->LVT);
    scicos_free(ptr->dwork);
    scicos_free(ptr);
    return;
   }

else
   {
    ptr=*(block->work);
    C2F(dlacpy)("F",&mu,&nu,u,&mu,ptr->LA,&mu);
    C2F(dgesvd)("A","A",&mu,&nu,ptr->LA,&mu,ptr->LSV,y1,&mu,ptr->LVT,&nu,ptr->dwork,&lwork,&info);
     if (info !=0)
       {if (flag!=6)
   	{set_block_error(-7);
        return;}}
    
    *(ptr->l0)=0;
     C2F(dlaset)("F",&mu,&nu,ptr->l0,ptr->l0,y2,&mu);
    for (i=0;i<min(mu,nu);i++)
	{ii=i+i*mu;
	 *(y2+ii)=*(ptr->LSV+i);}
    for (j=0;j<nu;j++)
	{for (i=j;i<nu;i++)
		{ij=i+j*nu;
	 	 ji=j+i*nu;
		 *(y3+ij)=*(ptr->LVT+ji);
		 *(y3+ji)=*(ptr->LVT+ij);}
	}
   }
}
