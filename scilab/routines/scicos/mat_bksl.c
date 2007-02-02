# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
#include <math.h>
extern int C2F(dgetrf)();
extern double C2F(dlamch)();
extern double C2F(dlange)();
extern int C2F(dlacpy)();
extern int C2F(dgecon)();
extern int C2F(dgetrs)();
extern int C2F(dgelsy1)();
typedef struct
{         int *ipiv;
          int *rank;
          int *jpvt;
          int *iwork;
          double *dwork;
	  double *LAF;
	  double *LA;
	  double *LXB;
} mat_bksl_struct ;
void mat_bksl(scicos_block *block,int flag)
{
 double *u1;
 double *u2;
 double *y;
 int mu;
 int nu1;
 int nu2;
 int info;
 int i,j,l,lw,lu;
 mat_bksl_struct *ptr;
 double rcond, ANORM, EPS,po;

 mu =GetInPortRows(block,1);
 nu1 =GetInPortCols(block,1);
 nu2 =GetInPortCols(block,2);
 u1=GetRealInPortPtrs(block,1);
 u2=GetRealInPortPtrs(block,2);
 y=GetRealOutPortPtrs(block,1);
 l=max(mu,nu1);
 lu=max(4*nu1,min(mu,nu1)+3*nu1+1);
 lw=max(lu,2*min(mu,nu1)+nu2);
             /*init : initialization*/
if (flag==4)
   {*(block->work)=(mat_bksl_struct*) malloc(sizeof(mat_bksl_struct));
    ptr=*(block->work);
    ptr->ipiv=(int*) malloc(sizeof(int)*nu1);
    ptr->rank=(int*) malloc(sizeof(int));
    ptr->jpvt=(int*) malloc(sizeof(int)*nu1);
    ptr->iwork=(int*) malloc(sizeof(int)*nu1);
    ptr->dwork=(double*) malloc(sizeof(double)*lw);
    ptr->LAF=(double*) malloc(sizeof(double)*(mu*nu1));
    ptr->LA=(double*) malloc(sizeof(double)*(mu*nu1));
    ptr->LXB=(double*) malloc(sizeof(double)*(l*nu2));
    
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    free(ptr->ipiv);
    free(ptr->rank);
    free(ptr->jpvt);
    free(ptr->iwork);
    free(ptr->LAF);
    free(ptr->LA);
    free(ptr->LXB);
    free(ptr->dwork);
    free(ptr);
    return;
   }

else
   {
    ptr=*(block->work);
    EPS=C2F(dlamch)("e",1L);
    ANORM=C2F(dlange)("1",&mu,&nu1,u1,&mu,ptr->dwork);
    C2F(dlacpy)("F",&mu,&nu1,u1,&mu,ptr->LA,&mu);
    if (mu==nu1)
	{C2F(dlacpy)("F",&mu,&nu1,ptr->LA,&mu,ptr->LAF,&mu);
	 C2F(dgetrf)(&nu1,&nu1,ptr->LAF,&nu1,ptr->ipiv,&info);
	 rcond=0;
 	 if (info==0)
	    {C2F(dgecon)("1",&nu1,ptr->LAF,&nu1,&ANORM,&rcond,ptr->dwork,ptr->iwork,&info);
	     if (rcond>pow(EPS,0.5))
		{C2F(dlacpy)("F",&nu1,&nu2,u2,&nu1,ptr->LXB,&nu1);
		 C2F(dgetrs)("N",&nu1,&nu2,ptr->LAF,&nu1,ptr->ipiv,ptr->LXB,&nu1,&info);
		 C2F(dlacpy)("F",&nu1,&nu2,ptr->LXB,&nu1,y,&nu1);
		 return;
		}
	    }
	}
    rcond=pow(EPS,0.5);
    C2F(dlacpy)("F",&mu,&nu2,u2,&mu,ptr->LXB,&l);
    for (i=0;i<nu1;i++)    *(ptr->jpvt+i)=0;
    
    C2F(dgelsy1)(&mu,&nu1,&nu2,ptr->LA,&mu,ptr->LXB,&l,ptr->jpvt,&rcond,ptr->rank,ptr->dwork,&lw,&info);
    if (info!=0)
	{if (flag!=6)
	    {set_block_error(-7);
             return;
	    }
	}
    C2F(dlacpy)("F",&nu1,&nu2,ptr->LXB,&l,y,&nu1);
    }
}
