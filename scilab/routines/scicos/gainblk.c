#include "scicos_block4.h"
#include "../machine.h"

extern int C2F(dmmul)();
extern int C2F(dmmul1)();


void gainblk(scicos_block *block,int flag)
{
  int i;

  double *u; double *y;
  int nu,mu,ny,my;

  nu=GetInPortRows(block,0);
  ny=GetOutPortRows(block,0);
  my=GetOutPortCols(block,0);

  u=GetRealInPortPtrs(block,0);
  y=GetRealOutPortPtrs(block,0);

  if (block->nrpar==1){
    for (i=0;i<nu*my;++i){
     y[i]=block->rpar[0]*u[i];
    }    
  }else{   
    C2F(dmmul)(block->rpar,&ny,u,&nu,y,&ny,&ny,&nu,&my);
  }
}
