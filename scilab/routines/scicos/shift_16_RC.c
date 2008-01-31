/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
# include "scicos_block4.h"
# include "../machine.h"
# include <math.h>

void shift_16_RC(scicos_block *block,int flag)
{
  short *u,*y,v; 
  int *ipar;
  int mu,nu,i,j;
  unsigned short k;
  mu=GetInPortRows(block,1);
  nu=GetInPortCols(block,1);
  u=Getint16InPortPtrs(block,1);
  y=Getint16OutPortPtrs(block,1);
  ipar=GetIparPtrs(block);
  k=(unsigned short)pow(2,16-1);
  for (i=0;i<mu*nu;i++)
      { v=u[i];
	for(j=0;j<-ipar[0];j++)
	     {
	       y[i]=v&1;
               if (y[i]==0)  
		     {y[i]=v>>1;
		      y[i]=y[i]&(k-1);}
	       else {
	             y[i]=v>>1;
	             y[i]=(y[i])|(k);
	            }
	       v=y[i];
	      }
       }
}
