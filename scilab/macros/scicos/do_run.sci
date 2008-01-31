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

function [ok,%tcur,%cpr,alreadyran,needcompile,%state0,solver]=do_run(%cpr)
// realize action associated to the run button
// performs necessary diagram (re-)compilation
// performs simulation initialisation
// performs simulation error recovery
//
// state  : is current state
// %state0 : is initial state value
//
// define user possible choices

  //** extract tolerances from scs_m.props.tol
  tolerances=scs_m.props.tol
  //** extract solver from tolerances
  solver=tolerances(6)

  //** update parameters or compilation results
  [%cpr,%state0_n,needcompile,alreadyran,ok]=do_update(%cpr,%state0,needcompile)
  //** if an error has ocurred in do_update
  //** then we exit from do_run
  if ~ok then
    %tcur=[]
    alreadyran=%f
    return
  end

  //** if alreadyran then set the var choice
  if alreadyran then
    choix=['Continue';'Restart';'End']
  else
    choix=[]
  end

  issequal=%t;
  //** initial state has been changed
  if ~isequal(%state0_n,%state0) then
    issequal=%f
  else
    //** test typeof outtb element
    for i=1:lstsize(%state0_n.outtb)
      if typeof(%state0_n.outtb(i))<>typeof(%state0.outtb(i))
        issequal=%f
        break
      end
    end
    //** test typeof oz element
    for i=1:lstsize(%state0_n.oz)
      if typeof(%state0_n.oz(i))<>typeof(%state0.oz(i))
        issequal=%f
        break
      end
    end
  end

  //** if state have changed
  //** finish the simulation via do_terminate()
  if ~issequal then
     %state0=%state0_n
    [alreadyran,%cpr]=do_terminate()
    choix=[]
  end

  //** switch appropriate solver
  if %cpr.sim.xptr($)-1<size(%cpr.state.x,'*') & solver<100 then
    message(['Diagram has been compiled for implicit solver'
             'switching to implicit Solver'])
    solver=100
    tolerances(6)=solver
  elseif (%cpr.sim.xptr($)-1==size(%cpr.state.x,'*')) & ..
        (solver==100 & size(%cpr.state.x,'*')<>0) then
    message(['Diagram has been compiled for explicit solver'
             'switching to explicit Solver'])
    solver=0
    tolerances(6)=solver
  end

  //** ask user what to do
  if choix<>[] then
    //** open dialog box
    to_do=choose(choix,'What do you want to do')

    //** if cancel then exit
    if to_do==0 then
      ok=%f
      return
    end

    select choix(to_do)

      case 'Continue' then 
        needstart=%f
        state=%cpr.state

      case 'Restart' then 
        needstart=%t
        state=%state0

      case 'End' then 
        state=%cpr.state
        needstart=%t
        tf=scs_m.props.tf;

        //Alan: ONPEUTPASAPPELLERDOTERMINATEICI?
        //reponse : non, car do_terminate() ne rend
        //          pas forc�ment la main � l'utilisateur

        //** run scicosim via 'finish' flag
        ierr=execstr('[state,t]=scicosim(%cpr.state,%tcur,tf,%cpr.sim,'+..
                     '''finish'',tolerances)','errcatch')

        %cpr.state=state
        alreadyran=%f

        //** error case
        if ierr<>0 then
          str_err=split_lasterror(lasterror());

          kfun=curblock()
          corinv=%cpr.corinv

          if kfun<>0 then //** block error
            path=corinv(kfun)
            //** get error cmd for the block
            cmd=get_errorcmd(path,'End problem.',str_err);
            //** send error cmd to scicos via the Scicos_commands global variable
            global Scicos_commands
            Scicos_commands=cmd;
          else //** simulator error
            message(['End problem:';str_err])
            scf(curwin);
          end
          ok=%f
        end
        //xset('window',curwin)
        return
    end
  else
    needstart=%t
    state=%state0
  end

  //win=xget('window')
  gh_win = gcf();

 //** scicos initialisation
  if needstart then
    //** if the simulation have already ran
    //** and is not finished then call do_terminate
    if alreadyran then
      [alreadyran,%cpr]=do_terminate()
      alreadyran=%f
    end
    //** set initial values for a new simulation
    %tcur=0
    %cpr.state=%state0
    tf=scs_m.props.tf;
    if tf*tolerances==[] then 
      x_message(['Simulation parameters not set';'use setup button']);
      return;
    end

    //** run scicosim via 'start' flag
    ierr=execstr('[state,t]=scicosim(%cpr.state,%tcur,tf,%cpr.sim,'+..
                 '''start'',tolerances)','errcatch')

    %cpr.state=state
    //** error case
    if ierr<>0 then
      str_err=split_lasterror(lasterror());

      kfun=curblock()
      corinv=%cpr.corinv
      if kfun<>0 then  //** block error
        path=corinv(kfun)
        //** get error cmd for the block
        cmd=get_errorcmd(path,'Initialisation problem.',str_err);
        //** send error cmd to scicos via the Scicos_commands global variable
        global Scicos_commands
        Scicos_commands=cmd;

      else //** simulator error
        message(['Initialisation problem:';str_err])
        scf(curwin);
      end
      ok=%f
      //xset('window',curwin)
      return
    end
    scf(gh_win);
    //xset('window',win);
  end

  //** scicos simulation
  //needreplay=%t
  tf=scs_m.props.tf;
  setmenu(curwin,'stop')
  //timer()
  needreplay=%t

  //** run scicosim via 'start' flag
  ierr=execstr('[state,t]=scicosim(%cpr.state,%tcur,tf,%cpr.sim,'+..
	       '''run'',tolerances)','errcatch')

  %cpr.state=state
  //** no error
  if ierr==0 then
    alreadyran=%t
    //** we are at the end of the simulation
    //** finish the simulation via do_terminate()
    if tf-t<tolerances(3) then
      //disp('fin');
      //Alan : j'enl�ve do_terminate ici car do_terminate
      //       ne rend pas la main
      //[alreadyran,%cpr]=do_terminate()
      needstart=%t
      alreadyran=%f
      //** run scicosim via 'finish' flag
      ierr=execstr('[state,t]=scicosim(%cpr.state,tf,tf,%cpr.sim,'+..
                   '''finish'',tolerances)','errcatch')

      %cpr.state=state

      //** error case
      if ierr<>0 then
        str_err=split_lasterror(lasterror());

        kfun=curblock()
        corinv=%cpr.corinv

        if kfun<>0 then //** block error
          path=corinv(kfun)
          //** get error cmd for the block
          cmd=get_errorcmd(path,'End problem.',str_err);
          //** send error cmd to scicos via the Scicos_commands global variable
          global Scicos_commands
          Scicos_commands=cmd;
        else //** simulator error
          message(['End problem:';str_err])
          scf(curwin);
        end
      end

    else
      %tcur=t
    end
  //** error case
  else
    str_err=split_lasterror(lasterror());

    alreadyran=%f
    kfun=curblock()
    corinv=%cpr.corinv

    if kfun<>0 then //** block error
      path=corinv(kfun)
      //** get error cmd for the block
      cmd=get_errorcmd(path,'Simulation problem.',str_err);
      //** send error cmd to scicos via the Scicos_commands global variable
      global Scicos_commands
      Scicos_commands=cmd;

    else //** simulateur error
      message(['Simulation problem:';str_err])
      scf(curwin);
    end
    ok=%f
  end
  //xset('window',curwin)
  //disp(timer())
  unsetmenu(curwin,'stop')

  needreplay=resume(needreplay)
endfunction

