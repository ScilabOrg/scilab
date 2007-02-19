#include "scicos_block4.h"
#include <math.h>

void  switch2_m(scicos_block *block,int flag)
{
  int i,j,phase,ipar,mu,nu;
  int *iparptrs;
  double *rpar;
  double *u2,*y,*u;
  iparptrs=GetIparPtrs(block);
  ipar=*iparptrs;
  rpar=GetRparPtrs(block);
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  u2=GetRealInPortPtrs(block,2);
  y=GetRealOutPortPtrs(block,1);
  if (flag == 1) {
    phase=get_phase_simulation();
    if (phase==1||block->ng==0){
      i=3;
      if (ipar==0){
	if (*u2>=*rpar) i=1;
      }else if (ipar==1){
	if (*u2>*rpar) i=1;
      }else {
	if (*u2!=*rpar) i=1;			     
      }
    }else{
      if(block->mode[0]==1){
	i=1;
      }else if(block->mode[0]==2){
	i=3;
      }
    }
    for (j=0;j<mu*nu;j++) {
       u=GetRealInPortPtrs(block,i);
      *(y+j)=*(u+j);
    }
  }else if(flag == 9){
    phase=get_phase_simulation();
    block->g[0]=*u2-*rpar;
    if (phase==1){
      i=3;
      if (ipar==0){
	if (block->g[0]>=0.0) i=1;
      }else if (ipar==1){
	if (block->g[0]>0.0) i=1;
      }else {
	if (block->g[0]!=0.0) i=1;			     
      }
      if(i==1) {
	block->mode[0]=1;
      }else{
	block->mode[0]=2;
      }
    }
  }
}
