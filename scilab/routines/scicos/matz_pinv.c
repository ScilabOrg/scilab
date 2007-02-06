# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
extern int C2F(dgesvd)();
extern int C2F(wmmul)();
extern int C2F(dlaset)();

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
	  double *LUr,*LUi;
	  double *LCr,*LCi;
          double *LSV;
          double *LSr,*LSi;
	  double *LSW;
          double *LVT;
	  double *LVr,*LVi;
          double *dwork;
          double *rwork;
} mat_pinv_struct ;
void matz_pinv(scicos_block *block,int flag)
{
 double *ur,*ui;
 double *yr,*y2r,*y3r;
 double *yi,*y2i,*y3i;
 int nu,mu;
 int info;
 int i,j,ij,ji,ii,lwork,rw;
 mat_pinv_struct *ptr;
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
   {*(block->work)=(mat_pinv_struct*) malloc(sizeof(mat_pinv_struct));
    ptr=*(block->work);
    ptr->l0=(double*) malloc(sizeof(double));
    ptr->LA=(double*) malloc(sizeof(double)*(2*mu*nu));
    ptr->LU=(double*) malloc(sizeof(double)*(2*mu*mu));
    ptr->LCr=(double*) malloc(sizeof(double)*(nu*mu));
    ptr->LCi=(double*) malloc(sizeof(double)*(nu*mu));
    ptr->LUr=(double*) malloc(sizeof(double)*(mu*mu));
    ptr->LUi=(double*) malloc(sizeof(double)*(mu*mu));
    ptr->LSV=(double*) malloc(sizeof(double)*(min(mu,nu)));
    ptr->LSW=(double*) malloc(sizeof(double)*(min(mu,nu)));
    ptr->LSr=(double*) malloc(sizeof(double)*(mu*nu));
    ptr->LSi=(double*) malloc(sizeof(double)*(mu*nu));
    ptr->LVT=(double*) malloc(sizeof(double)*(2*nu*nu));
    ptr->LVi=(double*) malloc(sizeof(double)*(nu*nu));
    ptr->LVr=(double*) malloc(sizeof(double)*(nu*nu));
    ptr->dwork=(double*) malloc(sizeof(double)*2*lwork);
    ptr->rwork=(double*) malloc(sizeof(double)*2*rw);
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    free(ptr->l0);
    free(ptr->LA);
    free(ptr->LU);
    free(ptr->LUr);
    free(ptr->LUi);
    free(ptr->LCr);
    free(ptr->LCi);
    free(ptr->LSV);
    free(ptr->LSW);
    free(ptr->LSr);
    free(ptr->LSi);
    free(ptr->LVT);
    free(ptr->LVr);
    free(ptr->LVi);
    free(ptr->dwork);
    free(ptr->rwork);
    free(ptr);
    return;
   }

else
   {
    ptr=*(block->work);
    for (i=0,j=0;i<(mu*nu),j<(2*mu*nu);i++,j+=2)   
	{ptr->LA[j]=ur[i];
	 ptr->LA[j+1]=ui[i];}
    C2F(zgesvd)("A","A",&mu,&nu,ptr->LA,&mu,ptr->LSV,ptr->LU,&mu,ptr->LVT,&nu,ptr->dwork,&lwork,ptr->rwork,&info);
     if (info !=0)
       {if (flag!=6)
   	{set_block_error(-7);
	return;}}
    
    *(ptr->l0)=0;
     C2F(dlaset)("F",&nu,&mu,ptr->l0,ptr->l0,ptr->LSi,&nu);
     C2F(dlaset)("F",&nu,&mu,ptr->l0,ptr->l0,ptr->LSr,&nu);
    for (i=0;i<min(mu,nu);i++)  
	 {if (*(ptr->LSV+i)!=0)
	      {*(ptr->LSW+i)=1/(*(ptr->LSV+i));}}
    for (i=0;i<min(mu,nu);i++)
	{ii=i+i*nu;
	 *(ptr->LSr+ii)=*(ptr->LSW+i);}
    for (j=0;j<nu;j++)
	{for (i=j;i<nu;i++)
		{ij=i+j*nu;
	 	 ji=j+i*nu;
		 *(ptr->LVr+ij)=*(ptr->LVT+2*ji);
		 *(ptr->LVr+ji)=*(ptr->LVT+2*ij);
		 *(ptr->LVi+ij)=-(*(ptr->LVT+2*ji+1));
		 *(ptr->LVi+ji)=-(*(ptr->LVT+2*ij+1));}
	}
    for (j=0;j<mu;j++)
	{for (i=j;i<mu;i++)
		{ij=i+j*mu;
	 	 ji=j+i*mu;
		 *(ptr->LUr+ij)=*(ptr->LU+2*ji);
		 *(ptr->LUr+ji)=*(ptr->LU+2*ij);
		 *(ptr->LUi+ij)=-(*(ptr->LU+2*ji+1));
		  *(ptr->LUi+ji)=-(*(ptr->LU+2*ij+1));}
	}
    C2F(wmmul)(ptr->LVr,ptr->LVi,&nu,ptr->LSr,ptr->LSi,&nu,ptr->LCr,ptr->LCi,&nu,&nu,&nu,&mu);
    C2F(wmmul)(ptr->LCr,ptr->LCi,&nu,ptr->LUr,ptr->LUi,&mu,yr,yi,&nu,&nu,&mu,&mu);
   }
}
