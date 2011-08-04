// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function bOK = dlwCopyF2cRule(vcxprojDirectory)
  F2CRuleFilesToCopy = ["f2c.props", ..
                        "f2c.xml", ..
                        "f2c.targets"];

  src_path = SCI + "/modules/dynamic_link/src/scripts/vs_templates/"
  for i = 1:size(F2CRuleFilesToCopy, "*")
    dest = fullpath(vcxprojDirectory + "/" + F2CRuleFilesToCopy(i));
    if ~isfile(dest) then
      copyfile(src_path + F2CRuleFilesToCopy(i), fullpath(vcxprojDirectory + "/" + F2CRuleFilesToCopy(i)));
    end
  end
  bOK = %t;
endfunction
