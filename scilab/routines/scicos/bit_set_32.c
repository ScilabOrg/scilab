#include "scicos_block4.h"
#include <math.h>

void bit_set_32(scicos_block *block,int flag)
{
  int n;
  int *ipar;
  long *u,*y;
  ipar=GetIparPtrs(block);
  u=Getint32InPortPtrs(block,1);
  y=Getint32OutPortPtrs(block,1);
  n=pow(2,*ipar);
  *y=((*u)|(n));
}
