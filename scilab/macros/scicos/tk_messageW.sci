//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function num=tk_messageW(strings,buttons)
//** tk_scs_message : overlad function for x_message
//**                  inside scicos
//**
//** Alan, 12/10/07 : initial rev

  //** check lhs/rhs arg
  [lhs,rhs]=argn(0)

  if rhs==0 then
    error(77)
  end

  if type(strings)<>10 then
    error(207,1)
  end

  if rhs==1 then
    buttons="Ok"
  end

  if rhs==2 then
    if type(buttons)<>10 then
      error(207,2)
    end
  end

  if rhs>2 then
    error(77)
  end

  //** format param
  buttons=buttons(:)
  if size(buttons,1)>2 then
    buttons=buttons(1)
  end

  strings=strings(:);

  //** create tcl/tk box txt
  txt=create_message_box(strings,buttons)
    TCL_EvalStr(txt)
  //**
  done="0";

  while done<>"1" & done<>"2" then
    xpause(10000)
    if MSDOS then TCL_EvalStr('catch {wm attributes .form -topmost 1}'),end
    done=TCL_GetVar('done')
    if done=="1" | done=="2" then
      TCL_EvalStr('destroy .form')
    elseif done=="3" & (rhs==1 | size(buttons,1)==1) then
      done="1"
      TCL_EvalStr('destroy .form')
    elseif done=="3"
      TCL_EvalStr(txt)
      done="0"
    end
  end

  //**
  if rhs==1 | size(buttons,1)==1 then
    num=1;
  else
    num=evstr(done);
  end

endfunction

//** create_message_box : create txt of the Tcl/Tk box
//**
//** Alan, 13/10/07 : initial rev
function str_out=create_message_box(str_in,but_lab)

  //** check lhs/rhs arg
  [lhs,rhs]=argn(0)

  //** retrieve current postion of the last dialog box
  //** potential TCL global variables numx/numy
  if TCL_ExistVar('numx') then
    numx=TCL_GetVar('numx')
    numx_tt='set numx '+numx
  else
    numx_tt='set numx [winfo pointerx .]'
  end

  if TCL_ExistVar('numy') then
    numy=TCL_GetVar('numy')
    numy_tt='set numy '+numy
  else
    numy_tt='set numy [winfo pointery .]'
  end

  //** substitute special characters
  str_in=sci2tcl(str_in)

  //**generate widgets for buttons
  but_txt=[];
  but_tt='';
  for i=1:size(but_lab,1)
     but_txt=[but_txt;
              'button $w.bot.but'++string(i)+' -text ""'+sci2tcl(but_lab(i))+...
                 '"" -command {set done '+string(i)+'}']
     but_tt=but_tt+'$w.bot.but'+string(i)+' ';
  end

  but_tt = 'grid '+but_tt+'-sticky nw'

  //** if only one button then press return is allowed
  if size(but_lab,1)==1 then
    bind_tt='bind $w <Return> {set numx [winfo x $w];set numy [winfo y $w];set done 1}'
  else
    bind_tt=""
  end


  str_out=['set w .form'
           'catch {destroy $w}'
           'toplevel $w'
            numx_tt
            numy_tt
           'wm geometry $w +$numx+$numy'
           'wm title $w '"Scicos Info'"'
           'wm iconname $w '"form'"'
           'wm attributes $w -topmost'
           '#### create two frame ####'
           'frame $w.top -relief raised -borderwidth 2'
           'frame $w.bot -relief raised -borderwidth 2'
           '#### create buttons in frame .bot ####'
            but_txt
           '#### create a label for the text in frame .top ####'
           'label $w.top.msg -wraplength 5i -justify left -text ""'+str_in+'""'
           '#### grig configure for . .bot .top .top.msg ####'
           'grid rowconfigure $w 0 -weight 1"
           'grid columnconfigure $w 0 -weight 1"
           'grid rowconfigure $w.bot 0 -weight 1"
           'grid columnconfigure $w.bot 1 -weight 1"
           'grid columnconfigure $w.top 0 -weight 1"
           'grid configure $w.top -sticky nsew"
           'grid configure $w.bot -sticky sew -ipadx 70'
           'grid configure $w.top.msg -sticky nw'
           '##### place buttons in .bot ####'
            but_tt
           '#### place .top.msg ####'
           'grid $w.top.msg -ipady 15'
           'set done 0'
            bind_tt
           'bind $w <Destroy> {set numx [winfo x $w];set numy [winfo y $w];set done 3}']
           //'tkwait variable done']

endfunction
