#include "scicos_block4.h"
#include <math.h>

void summation(scicos_block *block,int flag)
{
  int j,k;

  double *u;
  int nu,mu;
  double *y;

  y=GetRealOutPortPtrs(block,0);
  nu=GetInPortRows(block,0);
  mu=GetInPortCols(block,0);

  if(flag==1){
    if (GetNin(block)==1){
      y[0]=0.0;
      u=GetRealInPortPtrs(block,0);
      for (j=0;j<nu*mu;j++) {
	y[0]=y[0]+u[j];
      }
    }
    else {
      for (j=0;j<nu*mu;j++) {
	y[j]=0.0;
	for (k=0;k<GetNin(block);k++) {
	  u=GetRealInPortPtrs(block,k);
	  if(block->ipar[k]>0){
	    y[j]=y[j]+u[j];
	  }else{
	    y[j]=y[j]-u[j];
	  }
	}
      }
    }
  }
}
