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
 	  double *LC;
          double *LS;
          double *LSV;
          double *LSW;
          double *LU;
          double *LUT;
          double *LV;
          double *LVT;
          double *dwork;
} mat_pinv_struct ;
void mat_pinv(scicos_block *block,int flag)
{
 double *u;
 double *y;
 int nu,mu;
 int info;
 int i,j,ij,ji,ii,lwork;
 mat_pinv_struct *ptr;
 mu=GetInPortRows(block,1);
 nu=GetInPortCols(block,1);
 u=GetRealInPortPtrs(block,1);
 y=GetRealOutPortPtrs(block,1);
 lwork=max(3*min(mu,nu)+max(mu,nu),5*min(mu,nu)-4); 
             /*init : initialization*/
if (flag==4)
   {*(block->work)=(mat_pinv_struct*) malloc(sizeof(mat_pinv_struct));
    ptr=*(block->work);
    ptr->l0=(double*) malloc(sizeof(double));
    ptr->LA=(double*) malloc(sizeof(double)*(mu*nu));
    ptr->LC=(double*) malloc(sizeof(double)*(nu*mu));
    ptr->LS=(double*) malloc(sizeof(double)*(nu*mu));
    ptr->LSV=(double*) malloc(sizeof(double)*(min(mu,nu)));
    ptr->LSW=(double*) malloc(sizeof(double)*(min(mu,nu)));
    ptr->LU=(double*) malloc(sizeof(double)*(mu*mu));
    ptr->LUT=(double*) malloc(sizeof(double)*(mu*mu));
    ptr->LV=(double*) malloc(sizeof(double)*(nu*nu));
    ptr->LVT=(double*) malloc(sizeof(double)*(nu*nu));
    ptr->dwork=(double*) malloc(sizeof(double)*lwork);
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    free(ptr->LC);
    free(ptr->l0);
    free(ptr->LA);
    free(ptr->LSW);
    free(ptr->LS);
    free(ptr->LSV);
    free(ptr->LU);
    free(ptr->LUT);
    free(ptr->LV);
    free(ptr->LVT);
    free(ptr->dwork);
    free(ptr);
    return;
   }

else
   {
    ptr=*(block->work);
    C2F(dlacpy)("F",&mu,&nu,u,&mu,ptr->LA,&mu);
    C2F(dgesvd)("A","A",&mu,&nu,ptr->LA,&mu,ptr->LSV,ptr->LU,&mu,ptr->LVT,&nu,ptr->dwork,&lwork,&info);
     if (info !=0)
       {if (flag!=6)
   	{set_block_error(-7);
        return;}}
    for (i=0;i<min(mu,nu);i++)  
	 {if (*(ptr->LSV+i)!=0)
	      {*(ptr->LSW+i)=1/(*(ptr->LSV+i));}}
    *(ptr->l0)=0;
     C2F(dlaset)("F",&nu,&mu,ptr->l0,ptr->l0,ptr->LS,&nu);
    for (i=0;i<min(mu,nu);i++)
	{ii=i+i*nu;
	 *(ptr->LS+ii)=*(ptr->LSW+i);}
    for (j=0;j<mu;j++)
	{for (i=j;i<mu;i++)
		{ij=i+j*mu;
	 	 ji=j+i*mu;
		 *(ptr->LUT+ij)=*(ptr->LU+ji);
		 *(ptr->LUT+ji)=*(ptr->LU+ij);}
	}
    for (j=0;j<nu;j++)
	{for (i=j;i<nu;i++)
		{ij=i+j*nu;
	 	 ji=j+i*nu;
		 *(ptr->LV+ij)=*(ptr->LVT+ji);
		 *(ptr->LV+ji)=*(ptr->LVT+ij);}
	}
      C2F(dmmul)(ptr->LV,&nu,ptr->LS,&nu,ptr->LC,&nu,&nu,&nu,&mu);
      C2F(dmmul)(ptr->LC,&nu,ptr->LUT,&mu,y,&nu,&nu,&mu,&mu);
   }
}
