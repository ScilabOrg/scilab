#include "scicos_block4.h"
#include "../machine.h"

void mattran_m(scicos_block *block,int flag)
{
  double *u;
  double *y;
  int nu,mu;

  nu=GetInPortRows(block,0);
  mu=GetInPortCols(block,0);

  u=GetRealInPortPtrs(block,0);
  y=GetRealOutPortPtrs(block,0);

  C2F(mtran)(u,&nu,y,&mu,&nu,&mu);
}
