// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function bOK = dlwCreateF2cVcxprojFlts(targetName, ..
                                       filenamesList, ..
                                       vcxprojDirectory)
  bOK = %f;

  if isfile(SCI+'/modules/dynamic_link/src/scripts/vs_templates/vs_2010_f2c_template.vcxproj.filters') then
    FILTERS = mgetl(SCI+'/modules/dynamic_link/src/scripts/vs_templates/vs_2010_f2c_template.vcxproj.filters');
  else
    FILTERS = '';
  end

  for i = 1:size(filenamesList, '*')
    filenamesListF(i) = getrelativefilename(vcxprojDirectory, fullpath(filenamesList(i)));
    [p,f,e] = fileparts(filenamesListF(i));
    filenamesListC(i) = p + f + '.c';
  end

  FILTERS(grep(FILTERS, '<Filter>Source Files</Filter>')) = [];
  FILTERS(grep(FILTERS, '</ClCompile>')) = [];
  FILTERS = strsubst(FILTERS, '    <ClCompile Include=""fortran_template.c"">', ..
    strcat('    <ClCompile Include=""' + filenamesListC + '"">' + ascii(10) + '      <Filter>Source Files</Filter>' + ascii(10) + '    </ClCompile>', ascii(10)));

//  FILTERS(grep(FILTERS, '<Filter>Fortran files</Filter>')) = [];
//  FILTERS(grep(FILTERS, '</ClCompile>')) = [];
//  FILTERS = strsubst(FILTERS, '    <f2c_rule Include=""fortran_template.f"">', ..
//    strcat('    <f2c_rule Includee=""' + filenamesListF + '"">' + ascii(10) + '      <Filter>Fortran files</Filter>' + ascii(10) + '    </f2c_rule>', ascii(10)));
  
  if or(FILTERS <> '') then
    filtersfullname = fullpath(vcxprojDirectory + '/' + targetName + '.vcxproj.filters');
    bOK = mputl(FILTERS, filtersfullname);
    if ilib_verbose() > 1 then
      disp(FILTERS);
    end
  else
    // vs_2010_template.vcxproj.filters not found
    warning(SCI+'/modules/dynamic_link/src/scripts/vs_templates/vs_2010_f2c_template.vcxproj.filters'+ _('not found.') );
  end  
endfunction
