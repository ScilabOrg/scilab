#include "scicos_block4.h"
#include <math.h>

void bit_clear_8(scicos_block *block,int flag)
{
  int n;
  int *ipar;
  char *u,*y;
  ipar=GetIparPtrs(block);
  u=Getint8InPortPtrs(block,1);
  y=Getint8OutPortPtrs(block,1);
  n=(pow(2,8)-1)-pow(2,*ipar);
  *y=((*u)&(n));
}
