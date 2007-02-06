# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
extern int C2F(dgetrf)();
extern int C2F(dlaswp)();

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

typedef struct
{         int *ipiv;
          double *dwork;
	  double *IL;
	  double *IU;
} mat_lu_struct ;
void mat_lu(scicos_block *block,int flag)
{
 double *u;
 double *y1;
 double *y2;
 int mu;
 int nu;
 int info;
 int i,j,l,ij,ik;
 mat_lu_struct *ptr;
 
 mu =GetInPortRows(block,1);
 nu =GetInPortCols(block,1);
 u=GetRealInPortPtrs(block,1);
 y1=GetRealOutPortPtrs(block,1);
 y2=GetRealOutPortPtrs(block,2);
 l=min(mu,nu);
             /*init : initialization*/
if (flag==4)
   {*(block->work)=(mat_lu_struct*) malloc(sizeof(mat_lu_struct));
    ptr=*(block->work);
    ptr->ipiv=(int*) malloc(sizeof(int)*nu);
    ptr->dwork=(double*) malloc(sizeof(double)*(mu*nu));
    ptr->IL=(double*) malloc(sizeof(double)*(mu*l));
    ptr->IU=(double*) malloc(sizeof(double)*(l*nu));
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    free(ptr->ipiv);
    free(ptr->dwork);
    free(ptr->IL);
    free(ptr->IU);
    free(ptr);
    return;
   }

else
   {
    ptr=*(block->work);
    for (i=0;i<(mu*nu);i++)   { ptr->dwork[i]=u[i];}
    C2F(dgetrf)(&mu,&nu,ptr->dwork,&mu,ptr->ipiv,&info);
    if (info !=0)
       {if (flag!=6)
   	{set_block_error(-7);
        return;}}
   for (j=0;j<l;j++)
	{for (i=0;i<mu;i++)
	     {ij=i+j*mu;
	      if (i==j)
		{*(y2+ij)=1;}
	      else if (i>j)
		{*(y2+ij)=*(ptr->dwork+ij);}
	      else 
		{*(y2+ij)=0;}
	      }
	}
	for (j=0;j<nu;j++)
	{for (i=0;i<l;i++)
	     {ij=i+j*l;
	      ik=i+j*mu;
	      if (i<=j)
		{*(y1+ij)=*(ptr->dwork+ik);}
	      else
		{*(y1+ij)=0;}
	      }
 	}
   }
}
