# include "scicos_block4.h"
# include "../machine.h"
extern int C2F(riccsl)();
extern int C2F(riccms)();
extern int C2F(ricdsl)();
extern int C2F(ricdmf)();

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

typedef struct
{         int *bwork;
          int *iwork;
          double *dwork;
	  double *LX;
	  double *LWI;
	  double *LWR;
	  double *Rcond;
	  double *Ferr;
} ricc_struct ;
void ricc_m(scicos_block *block,int flag)
{
 double *u1;
 double *u2;
 double *u3;
 double *y;
 int *ipar;
 int nu;
 int info,i;
 int lw;
 ricc_struct *ptr;

 nu =GetInPortCols(block,1);
 u1=GetRealInPortPtrs(block,1);
 u2=GetRealInPortPtrs(block,2);
 u3=GetRealInPortPtrs(block,3);
 y=GetRealOutPortPtrs(block,1);
 ipar=GetIparPtrs(block);
 if (ipar[0]==1)
    {if (ipar[1]==1)
	 {lw=9*nu*nu+4*nu+max(1,6*nu);}
     else
	 {lw=9*nu*nu+7*nu+1;}}
 else
     {if (ipar[1]==1)
	 {lw=12*nu*nu+22*nu+max(21,4*nu);}
     else
	 {lw=28*nu*nu+2*nu+max(1,2*nu);}}

             /*init : initialization*/
if (flag==4)
   {*(block->work)=(ricc_struct*) malloc(sizeof(ricc_struct));
    ptr=*(block->work);
    ptr->bwork=(int*) malloc(sizeof(int)*2*nu);
    ptr->iwork=(int*) malloc(sizeof(int)*max(2*nu,nu*nu));
    ptr->dwork=(double*) malloc(sizeof(double)*lw);
    ptr->LWR=(double*) malloc(sizeof(double)*nu);
    ptr->LWI=(double*) malloc(sizeof(double)*nu);
    ptr->Rcond=(double*) malloc(sizeof(double));
    ptr->Ferr=(double*) malloc(sizeof(double));
    ptr->LX=(double*) malloc(sizeof(double)*nu*nu);
   }

       /* Terminaison */
else if (flag==5)
   {ptr=*(block->work);
    free(ptr->bwork);
    free(ptr->Ferr);
    free(ptr->Rcond);
    free(ptr->iwork);
    free(ptr->LWR);
    free(ptr->LWI);
    free(ptr->LX);
    free(ptr->dwork);
    free(ptr);
    return;
   }

else
   {
    ptr=*(block->work);
    if (ipar[0]==1)
       {if (ipar[1]==1)
            {C2F(riccsl)("N",&nu,u1,&nu,"U",u3,&nu,u2,&nu,ptr->LX,&nu,ptr->LWR,ptr->LWI,ptr->Rcond,ptr->Ferr,ptr->dwork,&lw,ptr->iwork,ptr->bwork,&info);}
         else
	    {C2F(riccms)("N",&nu,u1,&nu,"U",u3,&nu,u2,&nu,ptr->LX,&nu,ptr->LWR,ptr->LWI,ptr->Rcond,ptr->Ferr,ptr->dwork,&lw,ptr->iwork,&info);}}
     else
         {if (ipar[1]==1)
	     {C2F(ricdsl)("N",&nu,u1,&nu,"U",u3,&nu,u2,&nu,ptr->LX,&nu,ptr->LWR,ptr->LWI,ptr->Rcond,ptr->Ferr,ptr->dwork,&lw,ptr->iwork,ptr->bwork,&info);}
          else
	     {C2F(ricdmf)("N",&nu,u1,&nu,"U",u3,&nu,u2,&nu,ptr->LX,&nu,ptr->LWR,ptr->LWI,ptr->Rcond,ptr->Ferr,ptr->dwork,&lw,ptr->iwork,&info);}}
     if (info!=0)
	{if (flag!=6)
	    {set_block_error(-7);
             return;
	    }
	}
     for(i=0;i<nu*nu;i++) *(y+i)=*(ptr->LX+i);
    }
}
