# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
#include <math.h>
extern int C2F(zgetrf)();
extern int C2F(mtran)();
extern double C2F(dlamch)();
extern double C2F(zlange)();
extern int C2F(zgecon)();
extern int C2F(zgetrs)();
extern int C2F(zgelsy1)();
typedef struct
{         int *ipiv;
          int *rank;
          int *jpvt;
          double *iwork;
          double *dwork;
	  double *IN1F;
	  double *IN1;
	  double *urT1,*uiT1;
	  double *IN2X;
	  double *IN2;
	  double *urT2,*uiT2;
	  double *yrT,*yiT;
} mat_bksl_struct ;
void matz_div(scicos_block *block,int flag)
{
 double *u1r,*u1i;
 double *u2r,*u2i;
 double *yr,*yi;
 int mu1;
 int nu;
 int mu2;
 int info;
 int i,j,l,lw,lu,rw;
 mat_bksl_struct *ptr;
 double rcond, ANORM, EPS,po;

 mu1 =GetInPortRows(block,2);
 nu =GetInPortCols(block,1);
 mu2 =GetInPortRows(block,1);
 u1r=GetRealInPortPtrs(block,2);
 u1i=GetImagInPortPtrs(block,2);
 u2r=GetRealInPortPtrs(block,1);
 u2i=GetImagInPortPtrs(block,1);
 yr=GetRealOutPortPtrs(block,1);
 yi=GetImagOutPortPtrs(block,1);
 l=max(mu1,nu);
 lw=max(2*min(mu1,nu),mu1+1);
 lu=max(lw,min(mu1,nu)+mu2);
 lw=max(2*nu,min(mu1,nu)+lu);
 rw=2*nu;
             /*init : initialization*/
if (flag==4)
   {*(block->work)=(mat_bksl_struct*) malloc(sizeof(mat_bksl_struct));
    ptr=*(block->work);
    ptr->ipiv=(int*) malloc(sizeof(int)*nu);
    ptr->rank=(int*) malloc(sizeof(int));
    ptr->jpvt=(int*) malloc(sizeof(int)*mu1);
    ptr->iwork=(double*) malloc(sizeof(double)*2*mu1);
    ptr->dwork=(double*) malloc(sizeof(double)*2*lw);
    ptr->IN1F=(double*) malloc(sizeof(double)*(2*mu1*nu));
    ptr->IN1=(double*) malloc(sizeof(double)*(2*mu1*nu));
    ptr->urT1=(double*) malloc(sizeof(double)*(mu1*nu));
    ptr->uiT1=(double*) malloc(sizeof(double)*(mu1*nu));
    ptr->IN2X=(double*) malloc(sizeof(double)*(2*l*mu2));
    ptr->IN2=(double*) malloc(sizeof(double)*(2*mu2*nu));
    ptr->urT2=(double*) malloc(sizeof(double)*(mu2*nu));
    ptr->uiT2=(double*) malloc(sizeof(double)*(mu2*nu));
    ptr->yiT=(double*) malloc(sizeof(double)*(mu2*l));
    ptr->yrT=(double*) malloc(sizeof(double)*(mu2*l));
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    free(ptr->ipiv);
    free(ptr->rank);
    free(ptr->jpvt);
    free(ptr->iwork);
    free(ptr->IN1F);
    free(ptr->IN1);
    free(ptr->urT1);
    free(ptr->uiT1);
    free(ptr->urT2);
    free(ptr->uiT2);
    free(ptr->yrT);
    free(ptr->yiT);
    free(ptr->IN2X);
    free(ptr->IN2);
    free(ptr->dwork);
    free(ptr);
    return;
   }

else
   {
    ptr=*(block->work);
    C2F(mtran)(u1r,&mu1,ptr->urT1,&nu,&mu1,&nu);
    C2F(mtran)(u1i,&mu1,ptr->uiT1,&nu,&mu1,&nu);
    C2F(mtran)(u2r,&mu2,ptr->urT2,&nu,&mu2,&nu);
    C2F(mtran)(u2i,&mu2,ptr->uiT2,&nu,&mu2,&nu);
    for (i=0;i<(mu1*nu);i++)   
	{ptr->IN1[2*i]=ptr->urT1[i];
	 ptr->IN1[2*i+1]=-ptr->uiT1[i];}

    for (i=0;i<(mu2*nu);i++)   
	{ptr->IN2[2*i]=ptr->urT2[i];
	 ptr->IN2[2*i+1]=-ptr->uiT2[i];}
    EPS=C2F(dlamch)("e",1L);
    ANORM=C2F(zlange)("1",&nu,&mu1,ptr->IN1,&nu,ptr->dwork);
    if (mu1==nu)
	{C2F(zlacpy)("F",&nu,&nu,ptr->IN1,&nu,ptr->IN1F,&nu);
	 C2F(zgetrf)(&nu,&nu,ptr->IN1F,&nu,ptr->ipiv,&info);
	 rcond=0;
 	 if (info==0)
	    {C2F(zgecon)("1",&nu,ptr->IN1F,&nu,&ANORM,&rcond,ptr->dwork,ptr->iwork,&info);
	     if (rcond>pow(EPS,0.5))
		{
		 C2F(zgetrs)("N",&nu,&mu2,ptr->IN1F,&nu,ptr->ipiv,ptr->IN2,&nu,&info);
		 for (i=0;i<(mu2*nu);i++)
	   	 {*(ptr->yrT+i)=*(ptr->IN2+2*i);
	    	  *(ptr->yiT+i)=-(*(ptr->IN2+(2*i)+1));}
		  C2F(mtran)(ptr->yrT,&mu1,yr,&mu2,&mu1,&mu2);
		  C2F(mtran)(ptr->yiT,&mu1,yi,&mu2,&mu1,&mu2);
		 return;
		}
	    }
	}
    rcond=pow(EPS,0.5);
    for (i=0;i<mu1;i++)    *(ptr->jpvt+i)=0;
    C2F(zlacpy)("F",&nu,&mu2,ptr->IN2,&nu,ptr->IN2X,&l);
    C2F(zgelsy1)(&nu,&mu1,&mu2,ptr->IN1,&nu,ptr->IN2X,&l,ptr->jpvt,&rcond,ptr->rank,ptr->dwork,&lw,ptr->iwork,&info);
    if (info!=0)
	{if (flag!=6)
	    {set_block_error(-7);
             return;
	    }
	}
	for (i=0;i<(l*mu2);i++)
	   {*(ptr->yrT+i)=*(ptr->IN2X+2*i);
	    *(ptr->yiT+i)=-(*(ptr->IN2X+(2*i)+1));}
	    C2F(mtran)(ptr->yrT,&l,yr,&mu2,&mu1,&mu2);
	    C2F(mtran)(ptr->yiT,&l,yi,&mu2,&mu1,&mu2);
    }
}
