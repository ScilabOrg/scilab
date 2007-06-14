#include "scicos_block4.h"
#include <math.h>

void extract_bit_32_MSB0(scicos_block *block,int flag)
{
   int i,maxim;
   long *y,*u,ref,n;
   int *ipar;
   y=Getint32OutPortPtrs(block,1);
   u=Getint32InPortPtrs(block,1);
   ipar=GetIparPtrs(block);
   maxim=32;
   ref=0;
   for(i=0;i<*ipar;i++)
       {n=pow(2,maxim-1-i);
        ref=ref+n;}
   *y=(*u)&(ref);
}
