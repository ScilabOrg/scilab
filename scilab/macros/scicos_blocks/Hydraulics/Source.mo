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
 * See the file ../license.txt
*/

model Source
  parameter Real P0=300000 "Pression de la source";
  parameter Real T0=290 "Temp�rature de la source";
  parameter Real H0=100000 "Enthalpie sp�cifique de la source";
  parameter Real option_temperature=1 "1:temp�rature fix�e - 2:enthalpie fix�e";
   
  Real Pm "Pression moyenne";
  Real Q "D�bit";
  Real Tm "Temp�rature moyenne";
  Real Hm "Enthalpie sp�cifique moyenne";

public 
  PortPHQ2 C ;
equation 
  
  C.P = Pm;
  C.Q = Q;
  C.Hm = Hm;

  Pm = P0;

  Hm = H0;
  
  Tm = if (option_temperature <= 1) then T0 else Hm/4187 + 273.15;
  
end Source;
