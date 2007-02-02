# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
//#include <essl.h>
//#define cset (x,a,b) (Re(x)=a,Im(x)=b)
extern int C2F(dlapcy)();
extern int C2F(dgeev)();
extern int C2F(zlaset)();
extern int C2F(issymmetric)();
extern int C2F(dsyev)();
typedef struct
{         double *LA;
          double *LVR;
          double *dwork;
          double *dwork1;
} mat_vps_struct ;
void mat_vps(scicos_block *block,int flag)
{
 double *u;
 double *y1;
 double *y2;
 int nu;
 int info;
 int i,lwork,lwork1,j,ij,ji;
 int symmetric;
 mat_vps_struct *ptr;
 
 nu =GetInPortRows(block,1);
 u=GetRealInPortPtrs(block,1);
 y1=GetRealOutPortPtrs(block,1);
 y2=GetImagOutPortPtrs(block,1);
 lwork1=3*nu;
 lwork=3*nu-1;
             /*init : initialization*/
if (flag==4)
   {*(block->work)=(mat_vps_struct*) malloc(sizeof(mat_vps_struct));
    ptr=*(block->work);
    ptr->LA=(double*) malloc(sizeof(double)*(nu*nu));
    ptr->LVR=(double*) malloc(sizeof(double)*(nu*nu));
    ptr->dwork=(double*) malloc(sizeof(double)*lwork);
    ptr->dwork1=(double*) malloc(sizeof(double)*lwork1);
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    free(ptr->LA);
    free(ptr->LVR);
    free(ptr->dwork);
    free(ptr->dwork1);
    free(ptr);
    return;
   }

else
   {
    ptr=*(block->work);
    C2F(dlacpy)("F",&nu,&nu,u,&nu,ptr->LA,&nu);
   symmetric=1;
    for (j=0;j<nu;j++)
	{for (i=j;i<nu;i++)
		{ij=i+j*nu;
		 ji=j+i*nu;
		if (i!=j)
			{if (*(ptr->LA+ij)==*(ptr->LA+ji)) symmetric*= 1;
			 else { symmetric*=0;break;}}}}
    if (symmetric==1)
	{C2F(dsyev)("N","U",&nu,ptr->LA,&nu,y1,ptr->dwork,&lwork,&info);for (i=0;i<nu;i++) sciprint("y(%i)=%f\n",i,*(y1+i));
	 if (info!=0)
	    	{if (flag!=6)
		{set_block_error(-7);
		return;
		}}
	}
   else
	{C2F(dgeev)("N","N",&nu,ptr->LA,&nu,y1,y2,ptr->dwork1,&nu,ptr->LVR,&nu,ptr->dwork1,&lwork1,&info);
        if (info!=0)
	    	{if (flag!=6)
		{set_block_error(-7);
		return;
		}}
	}
   }
}
