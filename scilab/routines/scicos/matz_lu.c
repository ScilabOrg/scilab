# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
extern int C2F(zgetrf)();
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
void matz_lu(scicos_block *block,int flag)
{
 double *ur;
 double *ui;
 double *y1r;
 double *y1i;
 double *y2r;
 double *y2i;
 int mu;
 int nu;
 int info;
 int i,j,l,ij,ik,ij1;
 mat_lu_struct *ptr;
 
 mu =GetInPortRows(block,1);
 nu =GetInPortCols(block,1);
 ur=GetRealInPortPtrs(block,1);
 ui=GetImagInPortPtrs(block,1);
 y1r=GetRealOutPortPtrs(block,1);
 y1i=GetImagOutPortPtrs(block,1);
 y2r=GetRealOutPortPtrs(block,2);
 y2i=GetImagOutPortPtrs(block,2);
 l=min(mu,nu);
             /*init : initialization*/
if (flag==4)
   {*(block->work)=(mat_lu_struct*) malloc(sizeof(mat_lu_struct));
    ptr=*(block->work);
    ptr->ipiv=(int*) malloc(sizeof(int)*nu);
    ptr->dwork=(double*) malloc(sizeof(double)*(2*mu*nu));
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
    for (i=0,j=0;i<(mu*nu),j<(2*mu*nu);i++,j+=2)   
	{ptr->dwork[j]=ur[i];
	ptr->dwork[j+1]=ui[i];}
    C2F(zgetrf)(&mu,&nu,ptr->dwork,&mu,ptr->ipiv,&info);
    if (info !=0)
       {if (flag!=6)
   	{set_block_error(-7);
        return;}}
   for (j=0;j<l;j++)
	{for (i=0;i<mu;i++)
	     {ij=i+j*mu;
	      ij1=2*ij;
	      if (i==j)
		{*(y2r+ij)=1;
		 *(y2i+ij)=0;}
	      else if (i>j)
		{*(y2r+ij)=*(ptr->dwork+ij1);
		 *(y2i+ij)=*(ptr->dwork+ij1+1);}
	      else 
		{*(y2r+ij)=0;
		 *(y2i+ij)=0;}
	      }
	}
	for (j=0;j<nu;j++)
	{for (i=0;i<l;i++)
	     {ij=i+j*l;
	      ik=2*(i+j*mu);
	      if (i<=j)
		{*(y1r+ij)=*(ptr->dwork+ik);
		 *(y1i+ij)=*(ptr->dwork+ik+1);}
	      else
		{*(y1r+ij)=0;
		 *(y1i+ij)=0;}
	     }
 	}
   }
}
