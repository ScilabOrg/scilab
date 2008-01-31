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

connector PortPHQ1 
//  extends ThermoLib.Connecteurs.PortPHQ;
  Real P "Pression du fluide";
  Real H "Enthalpie sp�cifique du fluide";
  Real Q "D�bit massique du fluide";
  Real Hm "Enthalpie sp�cifique de m�lange";

//  input Boolean a=true "Pseudo-variable pour la v�rification du sens des connexions";
//  output Boolean b "Pseudo-variable pour la v�rification du sens des connexions";
end PortPHQ1;
