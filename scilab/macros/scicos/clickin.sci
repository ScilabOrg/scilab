function [o, modified, newparameters, needcompile, edited] = clickin(o)
//** Copyright INRIA
//** Comments by Simone Mannori
//**
//
//  o             : structure of clicked object, may be modified
//
//  modified      : boolean, indicates if simulation time parameters
//                  have been changed
//
//  newparameters : only defined for super blocks, gives pointers to
//                  sub-blocks for which parameters or states have been changed
//
//  needcompile   : indicates if modification implies a new compilation
//
//   

if needcompile==4 then
      %cpr=list()
end  // for standard_document to work

modified = %f;          //** not very clear internal flags 
newparameters = list();
needcompile = 0 ;

//**
if %diagram_open then //** %diagram_open is a global variable that signal if the diagram is show

  Cmenu = check_edge(o, Cmenu, %pt); 

  if Cmenu==("Link") then
    //we have clicked near a port
    [Cmenu] = resume("Link") ; //** EXIT with Link operation 
  end

end
//** 

//**---------------------------------------------------------------------
if typeof(o)=="Block" then  
  //** ----------------------------- Block ------------------------------
  
  //**----------------Look for a SuperBlock ----------------------------
  if o.model.sim=="super" then

      lastwin = curwin; // save the current window

      global inactive_windows
      jkk=[]

      for jk=1:size(inactive_windows(2),'*')
         if isequal(inactive_windows(1)(jk),super_path) then 
           jkk=[jkk,jk]
         end
      end
      curwinc=-1

      for jk=jkk 
        curwinc=inactive_windows(2)(jk),
//        ha=gcf(curwinc)
//        if diffobjs(o.model.rpar,ha.user_data) then
//           pause
//           xdel(curwinc)
//           curwinc=-1
//        else
           inactive_windows(1)(jk)=null();inactive_windows(2)(jk)=[]
           curwin=curwinc           
//        end
      end
      if curwinc <0 then
        curwin = get_new_window(windows) ; //** need a brand new window where open the 
      end

                                         //** super block
    if %diagram_open then     //** if the window is open open 
      gh_curwin = scf(curwin); //**   
    end                       //** 


    //** Check if this data structure is used in others parts of the code  
    //    execstr('scs_m_'+string(slevel)+'=scs_m'); //** extract the 'scs_m' of the superblock
    
    //** Inside the 'set' section of 'scicos_blocs/Misc/SUPER_f.sci' there is a recursive call
    //** at 'scicos' with the sub->scs_m as parameter 
    execstr('[o_n,needcompile,newparameters]='+o.gui+'(''set'',o)') ; //** this is the key of 
    //** the recursive superblock opening 
    


    //** Check is this comments is still valid 
    //edited variable is returned by SUPER_f -- NO LONGER TRUE
    if ~%exit then
      edited = ~isequalbitwise(o,o_n) //diffobjs(o,o_n)
      if edited then
	o = o_n
	modified = prod( size(newparameters) )>0 ; 
      end
    
    end
    
    curwin = lastwin
    if (~(or(curwin==winsid()))) then
          Cmenu = resume("Open/Set"); //** if the curwin is not present 
    end                               
    
    gh_curwin = scf(curwin); 
  

  //**-------------------- Mask C superblock  -----------------------------  
  elseif o.model.sim=="csuper"& o.model.ipar==1 then
    execstr('[o_n,needcompile,newparameters]='+o.gui+'(''set'',o)')
    modified = prod(size(newparameters))>0 // never used because if there is a change
                                           // needcompile>=2 and newparams not used 
    edited = ~isequalbitwise(o,o_n) 
    if edited then
      o = o_n
    end

  //**-------------------- C superblock ??? -----------------------------  
  elseif o.model.sim=="csuper" then
    execstr('[o_n,needcompile,newparameters]='+o.gui+'(''set'',o)')
    modified = prod(size(newparameters))>0
    edited = modified  // Not sure it is correct. 
    if edited then
      o = o_n
    end


  
  //**--------------------- Standard block -------------------------------  
  else

    execstr('o_n='+o.gui+'(''set'',o)') ;
    //Alan - 09/02/07 : replace <> operator by ~isequal
    //because <> operator crash for sublist with int elements
    //edited = or(o<>o_n) ;
    edited = ~isequal(o,o_n) ;
    if edited then
      model = o.model
      model_n = o_n.model
      if ~is_modelica_block(o) then
	modified=or(model.sim<>model_n.sim)|..
		 ~isequal(model.state,model_n.state)|..
		 ~isequal(model.dstate,model_n.dstate)|..
		 ~isequal(model.odstate,model_n.odstate)|..
		 ~isequal(model.rpar,model_n.rpar)|..
		 ~isequal(model.ipar,model_n.ipar)|..
		 ~isequal(model.opar,model_n.opar)|..
		 ~isequal(model.label,model_n.label)
	if ~modified then
	  for i=1:lstsize(model.opar)
	    if typeof(model.opar(i))<>typeof(model_n.opar(i)) then
	      modified=%t
	      break
	    end
	  end
	end
	if or(model.in<>model_n.in)|or(model.out<>model_n.out)|...
	   or(model.in2<>model_n.in2)|or(model.out2<>model_n.out2)|...
           or(model.outtyp<>model_n.outtyp)|or(model.intyp<>model_n.intyp) then
	  // input or output port sizes or type changed
	  needcompile=1
	end
	if or(model.firing<>model_n.firing)  then 
	  // initexe changed
	  needcompile=2
	end
	if (size(model.in,'*')<>size(model_n.in,'*'))|..
	      (size(model.out,'*')<>size(model_n.out,'*')) then  
	  // number of input or output  changed
	  needcompile=4
	end
	if model.sim=='input'|model.sim=='output' then
	  if model.ipar<>model_n.ipar then
	    needcompile=4
	  end
	end
	if or(model.blocktype<>model_n.blocktype)|..
	      or(model.dep_ut<>model_n.dep_ut)  then 
	  // type 'c','d','z','l' or dep_ut changed
	  needcompile=4
	end
	if (model.nzcross<>model_n.nzcross)|(model.nmode<>model_n.nmode) then 
	  // size of zero cross changed
	  needcompile=4
	end
	if prod(size(model_n.sim))>1 then
	  if model_n.sim(2)>1000 then  // Fortran or C Block
	    if model.sim(1)<>model_n.sim(1) then  //function name has changed
	      needcompile=4
	    end
	  end
	end
      else //implicit block
	//force compilation if an implicit block has been edited
	modified=or(model_n<>model)
	eq=model.equations;eqn=model_n.equations;

	if or(eq.model<>eqn.model)|or(eq.inputs<>eqn.inputs)|..
				      or(eq.outputs<>eqn.outputs) then  
	  needcompile=4
	end
	if (size(o.model.sim)>1) then
	  if (o.model.sim(2)==30004) then // only if it is the Modelica generic block
	    if or(o.graphics.exprs<>o_n.graphics.exprs) then  // if equation in generic Modelica Mblock change
	      needcompile=4
	      modified=%t;
	    end
	  end
	end
      end
      o=o_n
    end
  end

//**---------------------- Link -------------------------------------------------
elseif typeof(o)=="Link" then  
  
  [Cmenu] = resume("Link")

//**---------------------- Text -------------------------------------------------  
elseif typeof(o)=="Text" then
  
  execstr('o_n='+o.gui+'(''set'',o)') ;
  edited = or(o<>o_n) ; 
  o = o_n ; 

end

endfunction
