#include "scicos_block4.h"
#include <math.h>

void bit_set_16(scicos_block *block,int flag)
{
  int n;
  int *ipar;
  short *u,*y;
  ipar=GetIparPtrs(block);
  u=Getint16InPortPtrs(block,1);
  y=Getint16OutPortPtrs(block,1);
  n=pow(2,*ipar);
  *y=((*u)|(n));
}
