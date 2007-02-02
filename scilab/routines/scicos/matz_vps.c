# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
extern int C2F(zgeev)();
extern int C2F(zheev)();
typedef struct
{         double *LA;
          double *LX;
          double *LVR;
          double *dwork;
	  double *rwork;
          double *dwork1;
          double *rwork1;
} mat_vps_struct ;
void matz_vps(scicos_block *block,int flag)
{
 double *ur,*ui;
 double *yr,*yi;
 int nu;
 int info;
 int i,lwork,lwork1,j,ij,ji,rw;
 int hermitien;
 mat_vps_struct *ptr;
 
 nu =GetInPortRows(block,1);
 ur=GetRealInPortPtrs(block,1);
 ui=GetImagInPortPtrs(block,1);
 yr=GetRealOutPortPtrs(block,1);
 yi=GetImagOutPortPtrs(block,1);
 lwork1=2*nu;
 lwork=2*nu-1;
 rw=3*nu-2;
             /*init : initialization*/
if (flag==4)
   {*(block->work)=(mat_vps_struct*) malloc(sizeof(mat_vps_struct));
    ptr=*(block->work);
    ptr->LA=(double*) malloc(sizeof(double)*(2*nu*nu));
    ptr->LX=(double*) malloc(sizeof(double)*(2*nu));
    ptr->LVR=(double*) malloc(sizeof(double)*(2*nu*nu));
    ptr->dwork=(double*) malloc(sizeof(double)*2*lwork);
    ptr->rwork=(double*) malloc(sizeof(double)*2*rw);
    ptr->dwork1=(double*) malloc(sizeof(double)*2*lwork1);
    ptr->rwork1=(double*) malloc(sizeof(double)*2*lwork1); 
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    free(ptr->LA);
    free(ptr->LX);
    free(ptr->LVR);
    free(ptr->rwork);
    free(ptr->rwork1);
    free(ptr->dwork);
    free(ptr->dwork1);
    free(ptr);
    return;
   }

else
   {
    ptr=*(block->work);
    for (i=0,j=0;i<(nu*nu),j<(2*nu*nu);i++,j+=2)   
	{ptr->LA[j]=ur[i];
	 ptr->LA[j+1]=ui[i];}
   hermitien=1;
    for (j=0;j<nu;j++)
	{for (i=j;i<nu;i++)
		{ij=i+j*nu;
		 ji=j+i*nu;
		if (i!=j)
			{if ((*(ptr->LA+2*ij)==*(ptr->LA+2*ji))&&(*(ptr->LA+2*ij+1)==-(*(ptr->LA+2*ji+1)))) hermitien*= 1;
			 else { hermitien*=0;break;}}}}
    if (hermitien==1)
	{C2F(zheev)("N","U",&nu,ptr->LA,&nu,yr,ptr->dwork,&lwork,ptr->rwork,&info);
	 if (info!=0)
	    	{if (flag!=6)
		{set_block_error(-7);
		return;
		}}
	}
   else
	{C2F(zgeev)("N","N",&nu,ptr->LA,&nu,ptr->LX,ptr->dwork1,&nu,ptr->LVR,&nu,ptr->dwork1,&lwork1,ptr->rwork1,&info);
        if (info!=0)
	    	{if (flag!=6)
		{set_block_error(-7);
		free(ptr->LA);
		free(ptr->LX);
		free(ptr->LVR);
		free(ptr->dwork);
		free(ptr->rwork);
		free(ptr->rwork1);
		free(ptr->dwork1);
		free(ptr);
		return;
		}}
	for (i=0;i<nu;i++)
		{*(yr+i)=*(ptr->LX+2*i);
		 *(yi+i)=*(ptr->LX+2*i+1);}
	}
   }
}
