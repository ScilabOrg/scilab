function scipad(varargin)
// start Scipad editor

// find out if we are in scilab4 or scilab5, and
//  make some workarounds for scilab4
    if listfiles(SCI+"/modules/scipad/")<>[] then
      scilab5=%t;
    else
      scilab5=%f;
    end 
    if ~scilab5 then
      function s=gettext(scope,sss)
        labels=["scipad_message_"+string(1:6), "scipad_error_1"]
        results=[" Scilab in no window no interactive mode : Scipad unavailable"
                 "Function ";
                 "is not contained in a loaded library, ";
                 "Scipad doesn''t know where to find its source";
                 "Scipad cannot open a ";" object!";
                 " Scilab has not been built with tk: Scipad unavailable"
        ]
        s=results(labels==sss)
      endfunction
    end
  
    global SCIPADISSTARTING
    // ensure that no concurrent launching occurs
    // this fixes the issue that shows up when quickly
    // clicking twice the menu or icon button in the
    // Scilab window (bug 2226) [relies on exists(..."nolocal"), only in scilab5]
    if exists("SCIPADISSTARTING","nolocal") & scilab5 then
        return
    end
    SCIPADISSTARTING = 1;  // guard variable

    // check if Scipad can be launched, given the Scilab configuration
    nwnimode=find(sciargs()=="-nwni");
    noguimode=find(sciargs()=="-nogui");
    if (nwnimode <>[] | noguimode <>[]) then
        clear nwnimode noguimode
        warning(gettext('messages','scipad_message_1'));
        abort;
    end;
    clear nwnimode noguimode

    if with_tk() then
        if ~TCL_ExistInterp("scipad") then    
            TCL_EvalStr("interp create scipad")
            TCL_EvalStr("load {'+gettklib()+'} Tk scipad")
            TCL_EvalStr("wm withdraw .","scipad")
            TCL_EvalStr("scipad alias ScilabEval ScilabEval")
        end
        if exists("SCIHOME") then
            if MSDOS then
                TCL_EvalStr("set env(SCIHOME) """+strsubst(SCIHOME,"\","/")+"""","scipad")
            else
                TCL_EvalStr("set env(SCIHOME) """+pathconvert(SCIHOME,%f,%t)+"""","scipad")
            end
        end
        // set Scipad's tmpdir to Scilab's TMPDIR
        // note that global TMPDIR is not needed since TMPDIR is not a global
        // but an environment variable, in fact a local variable of the main
        // level that is inherited/visible in this macro (same as SCI)
        if MSDOS then
            TCL_EvalStr("set tmpdir """+strsubst(TMPDIR,"\","/")+"""","scipad")
            TCL_EvalStr("set env(SCIINSTALLPATH) """+strsubst(SCI,"\","/")+"""","scipad")
        else
            TCL_EvalStr("set tmpdir """+pathconvert(TMPDIR,%f,%t)+"""","scipad")
            TCL_EvalStr("set env(SCIINSTALLPATH) """+pathconvert(SCI,%f,%t)+"""","scipad")
        end
        if scilab5 then
          TCL_EvalStr("source """+SCI+"/modules/scipad/tcl/scipad.tcl""","scipad")
        else
          TCL_EvalStr("source """+SCI+"/tcl/scipadsources/scipad.tcl""","scipad")
        end        
        nfiles=argn(2)
        if nfiles>0 then
            for i=1:nfiles
                validfile=%f;
                f=varargin(i)
                select type(f)        
                case 1 then filetoopen=string(f); validfile=%t;
                case 8 then filetoopen=string(f); validfile=%t;
                case 10 then filetoopen=f; validfile=%t;
////nice try, but can't be done. The assignement to the argument
//// overrides the original function name, and this is correct.
//              case 13 then
//                  b=macr2tree(f); funname=b(2) //DOESN'T WORK. How else?
//                  libmacro=whereis(funname)
//                  disp(libmacro,funname)
//                  if libmacro<>[] & funname<>[] then
//                      a=string(eval(libmacro));libpath=a(1)
//                      if libpath<>[] then
//                          validfile=%t;
//                          //the correspondance between function and file name it is tacitly assumed
//                          filetoopen=pathconvert(libpath+funname+".sci",%f)
//                      else
//                          warning(gettext('messages','scipad_message_2')+funname+gettext('messages','scipad_message_3') ..
//                                  +gettext('messages','scipad_message_4'))
//                      end
//                  end
                else
                    warning(gettext('messages','scipad_message_5')+typeof(f)+gettext('messages','scipad_message_6'))
                end
                if validfile then
                    // Tcl handles the filenames correctly with any path separator but on Windows the separator
                    // is \ and in Tcl this is an escape. Therefore filepaths are always converted to the unix
                    // form to avoid this problem
                    filetoopen=pathconvert(filetoopen,%f,%t,"u")
                    // The complication below is needed to comply with ScilabEval sync limitations: what
                    // is executed during a sync is not available in the interpreter after the sync has
                    // ended (because sync launches a new interpreter - same thing as running in a function),
                    // thus sync cannot be used for loading colorization arrays chset and words, and a seq
                    // is used. But then the file opening should be queued after the loading of keywords,
                    // which means the following nested complication is needed
                    TCL_EvalStr("ScilabEval {TCL_EvalStr(""openfile {"+ filetoopen +"}"",""scipad"")} ""seq"" ")     
//                    TCL_EvalStr("openfile """+filetoopen+"""","scipad")
                end
            end  // end of "for i=1:nfiles"
        end  // end of "if nfiles>0"
    else 
        // with_tk() is %f
        clearglobal SCIPADISSTARTING
        error(gettext('errors','scipad_error_1'))
    end
    clearglobal SCIPADISSTARTING
endfunction
