# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
extern int C2F(dgesvd)();
extern int C2F(dlaset)();
typedef struct
{         double *l0;
	  double *LA;
	  double *LU;
          double *LSV;
          double *LVT;
          double *dwork;
          double *rwork;
} mat_sdv_struct ;
void matz_svd(scicos_block *block,int flag)
{
 double *ur,*ui;
 double *y1r,*y2r,*y3r;
 double *y1i,*y2i,*y3i;
 int nu,mu;
 int info;
 int i,j,ij,ji,ii,lwork,rw;
 mat_sdv_struct *ptr;
 mu=GetInPortRows(block,1);
 nu=GetInPortCols(block,1);
 ur=GetRealInPortPtrs(block,1);
 ui=GetImagInPortPtrs(block,1);
 y1r=GetRealOutPortPtrs(block,1);
 y2r=GetRealOutPortPtrs(block,2);
 y3r=GetRealOutPortPtrs(block,3);
 y1i=GetImagOutPortPtrs(block,1);
 //y2i=GetImagOutPortPtrs(block,2);
 y3i=GetImagOutPortPtrs(block,3);
 lwork=max(3*min(mu,nu)+max(mu,nu),5*min(mu,nu)-4);
 rw=5*min(mu,nu);
             /*init : initialization*/
if (flag==4)
   {*(block->work)=(mat_sdv_struct*) malloc(sizeof(mat_sdv_struct));
    ptr=*(block->work);
    ptr->l0=(double*) malloc(sizeof(double));
    ptr->LA=(double*) malloc(sizeof(double)*(2*mu*nu));
    ptr->LU=(double*) malloc(sizeof(double)*(2*mu*mu));
    ptr->LSV=(double*) malloc(sizeof(double)*(min(mu,nu)));
    ptr->LVT=(double*) malloc(sizeof(double)*(2*nu*nu));
    ptr->dwork=(double*) malloc(sizeof(double)*2*lwork);
    ptr->rwork=(double*) malloc(sizeof(double)*2*rw);
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    free(ptr->l0);
    free(ptr->LA);
    free(ptr->LU);
    free(ptr->LSV);
    free(ptr->LVT);
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
    C2F(zgesvd)("A","A",&mu,&nu,ptr->LA,&mu,ptr->LSV,ptr->LU,&mu,ptr->LVT,&nu,ptr->dwork,&lwork,ptr->rwork,&info);
     if (info !=0)
       {if (flag!=6)
   	{set_block_error(-7);
        return;}}
    
    *(ptr->l0)=0;
     C2F(dlaset)("F",&mu,&nu,ptr->l0,ptr->l0,y2r,&mu);
    for (i=0;i<min(mu,nu);i++)
	{ii=i+i*mu;
	 *(y2r+ii)=*(ptr->LSV+i);}
    for (j=0;j<nu;j++)
	{for (i=j;i<nu;i++)
		{ij=i+j*nu;
	 	 ji=j+i*nu;
		 *(y3r+ij)=*(ptr->LVT+2*ji);
		 *(y3r+ji)=*(ptr->LVT+2*ij);
		 *(y3i+ij)=-(*(ptr->LVT+2*ji+1));
		 *(y3i+ji)=-(*(ptr->LVT+2*ij+1));}
	}
    for (i=0;i<mu*mu;i++)
	{*(y1r+i)=*(ptr->LU+2*i);
	 *(y1i+i)=*(ptr->LU+2*i+1);}
   }
}
