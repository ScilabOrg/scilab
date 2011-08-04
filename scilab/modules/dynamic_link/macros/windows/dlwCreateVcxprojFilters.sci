// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function bOK = dlwCreateVcxprojFilters(targetName, ..
                                       filenamesList, ..
                                       vcxprojDirectory)
  bOK = %f;

  if isfile(SCI+'/modules/dynamic_link/src/scripts/vs_templates/vs_2010_template.vcxproj.filters') then
    FILTERS = mgetl(SCI+'/modules/dynamic_link/src/scripts/vs_templates/vs_2010_template.vcxproj.filters');
  else
    FILTERS = '';
  end
  
  FILTERS(grep(FILTERS, '<Filter>Source Files</Filter>')) = [];
  FILTERS(grep(FILTERS, '</ClCompile>')) = [];
  FILTERS = strsubst(FILTERS, '    <ClCompile Include=""vs_2010_template.cpp"">', ..
    strcat('    <ClCompile Include=""' + filenamesList + '"" />' + ascii(10) + '      <Filter>Source Files</Filter>' + ascii(10) + '    </ClCompile>', ascii(10)));  
  
  if (FILTERS <> '') then
    filtersfullname = fullpath(vcxprojDirectory + '/' + targetName + '.vcxproj.filters');
    bOK = mputl(FILTERS, filtersfullname);
    if ilib_verbose() > 1 then
      disp(FILTERS);
    end
  else
    // vs_2010_template.vcxproj.filters not found
    warning(SCI+'/modules/dynamic_link/src/scripts/vs_templates/vs_2010_template.vcxproj.filters'+ _('not found.') );
  end  
endfunction
