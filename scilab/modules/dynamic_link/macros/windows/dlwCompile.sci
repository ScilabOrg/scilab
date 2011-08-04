// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011  - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function dlwCompile(makename, files, make_command)
  nf = size(files,"*");

  for i=1:nf
    if ( ilib_verbose() <> 0 ) then
      mprintf(_("   Compilation of ") + string(files(i)) +"\n");
    end
  end

  // then the shared library
  if ( ilib_verbose() <> 0 ) then
    mprintf(_("   Building shared library (be patient)\n"));
  end

  commandLineCompile = "";

  if (dlwIsVc10Express() | dlwIsVc10Pro()) & ..
      ~dlwCheckForceVCMakefile() & ..
      ~dlwIsMixedCppFortran(files) then
  
    if getenv('SCIDIR', '') == '' then
      setenv('SCIDIR', SCI);
    end
    
    solutionConfig = "";
    platformConfig = "";
    
    if dlwCheckDebugMode() then
      solutionConfig = "Debug";
    else
      solutionConfig = "Release";
    end
    
    if win64() then
      platformConfig = "x64";
    else
      platformConfig = "win32";
    end
    
    if dlwHaveMsBuild() & (dlwIsOnlyCppFiles(files) | dlwCheckForceF2c()) then
      nbCpus = string(dlwGetNbCpuCore());
      commandLineCompile = make_command + " " + makename + " /p:Platform=" + platformConfig + ..
                           " /p:Configuration=" + solutionConfig + " /m:" + nbCpus;
    else
      solutionConfig = solutionConfig + "|" + platformConfig;
      solutionConfig = """" + solutionConfig + """";
      commandLineCompile = make_command + " " + solutionConfig + " " + makename;
    end
    
  else
    commandLineCompile = make_command + makename + " all";
  end

  [msg, stat] = unix_g(commandLineCompile + " 2>&0");
  if stat <> 0 then
    // more feedback when compilation fails
    if dlwHaveMsBuild() & (dlwIsOnlyCppFiles(files) | dlwCheckForceF2c()) then
      [msg, stat, stderr] = unix_g(commandLineCompile + " 1>&2");
    else
      logname = tempname();
      [msg, stat, stderr] = unix_g(commandLineCompile + + " /Out " + logname);
      stderr = mgetl(logname);
    end
    
    if stat <> 0 then
      disp(stderr);
      error(msprintf(gettext("%s: Error while executing %s.\n"), "ilib_compile", makename));
    end
  else
    if ilib_verbose() > 1 then
      disp(msg);
    end
  end
endfunction
//=============================================================================
