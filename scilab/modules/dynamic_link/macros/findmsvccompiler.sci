// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function MSCompiler = findmsvccompiler()

  MSCompiler = 'unknown'; // unknown

  if getos() == 'Windows' then

    // Load dynamic_link Internal lib if it's not already loaded
    if ~ exists("dynamic_linkwindowslib") then
      load("SCI/modules/dynamic_link/macros/windows/lib");
    end

    // We use always last version of MS compiler

    if dlwIsVc10Pro() & dlwIsVc10Express() then
      MSCompiler = 'msvc100express';       // Microsoft Visual 2010 Express with SDK extension
      return;
    end

    if dlwIsVc10Pro() then
      MSCompiler = 'msvc100pro';           // Microsoft Visual 2010 Professional (or more)
      return;
    end

    if dlwIsVc10Express() then
      MSCompiler = 'msvc100express';       // Microsoft Visual 2010 Express
      return;
    end

    if dlwIsVc90Pro() then
      MSCompiler = 'msvc90pro';            // Microsoft Visual 2008 Studio Professional
      return;
    end

    if dlwIsVc90Std() then
      MSCompiler = 'msvc90std';            // Microsoft Visual 2008 Studio Standard
      return;
    end

    if dlwIsVc90Express() then
      MSCompiler = 'msvc90express';        // Microsoft Visual 2008 Express
      return;
    end

    if dlwIsVc80Pro() then
      MSCompiler = 'msvc80pro';            // Microsoft Visual 2005 Studio Professional
      return;
    end

    if dlwIsVc80Std() then
      MSCompiler = 'msvc80std';            // Microsoft Visual 2005 Studio Standard
      return;
    end

    if dlwIsVc80Express() then
      MSCompiler = 'msvc80express';        // Microsoft Visual 2005 Express
      return;
    end

    if dlwIsVc71() then
      MSCompiler = 'msvc71';               // Microsoft Visual Studio .NET 2003
      return;
    end

    if dlwIsVc70() then
      MSCompiler = 'msvc70';               // Microsoft Visual Studio .NET 2002
      return;
    end

  else // NOT WINDOWS
    MSCompiler = 'unknown'; // unknown
  end

endfunction
//=============================================================================

