# include "scicos_block4.h"
# include "../machine.h"
#include <stdio.h>
//#include <essl.h>
typedef struct FCOMPLEX {float r,i;} fcomplex;
#define cset(x,a,b) (x.r=a,x.i=b)
extern int C2F(zgetrf)();
typedef struct
{         int *ipiv;
          double *wrk;
} mat_det_struct ;
void matz_det(scicos_block *block,int flag)
{
 double *ur;
 double *yr;
 double *ui;
 double *yi;
 double *v;
 int nu;
 int *ipiv;
 int info;
 int i,j;
 fcomplex D,l;
 float A;
 mat_det_struct *mdet;
 
 nu =GetInPortRows(block,1);
 ur=GetRealInPortPtrs(block,1);
 ui=GetImagInPortPtrs(block,1);
 yr=GetRealOutPortPtrs(block,1);
 yi=GetImagOutPortPtrs(block,1);
             /*init : initialization*/
if (flag==4)

      { *(block->work)=(mat_det_struct*) malloc(sizeof(mat_det_struct));
    mdet=*(block->work);
    mdet->ipiv=(int*) malloc(sizeof(int)*nu);
    mdet->wrk=(double*) malloc(sizeof(double)*(2*nu*nu));

      }

       /* Terminaison */
else if (flag==5)
   {mdet=*(block->work);
    free(mdet->ipiv);
    free(mdet->wrk);
    free(mdet);

    return;
   }

else
   {
    mdet=*(block->work);
    for (i=0,j=0;i<(nu*nu),j<(2*nu*nu);i++,j+=2)   
	{mdet->wrk[j]=ur[i];
	mdet->wrk[j+1]=ui[i];}
     C2F(zgetrf)(&nu,&nu,mdet->wrk,&nu,mdet->ipiv,&info);
    if (info !=0)
       {if (flag!=6)
    	{set_block_error(-7);
        return;}}
      D.r=1;
      D.i=0;
    for (i=0;i<nu;i++)
       {if((*(mdet->ipiv+i))!=i+1) {D.r=-D.r;D.i=-D.i;}
        l.r=*(mdet->wrk+i*2*(nu+1));
	l.i=*(mdet->wrk+1+i*2*(nu+1));
	A=D.r;
	D.r=D.r*l.r-D.i*l.i;
	D.i=D.i*l.r+A*l.i;
        };
     *yr=D.r;
     *yi=D.i;
    }
 }
