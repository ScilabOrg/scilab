// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

function testOk = m2sciTestConversion(functionName)

    mfilesPath = "SCI/modules/m2sci/tests/unit_tests/mfiles/";

    exec("SCI/modules/m2sci/tests/unit_tests/utils/sci_m2sciUnknownDims.sci", -1);
    exec("SCI/modules/m2sci/tests/unit_tests/utils/sci_m2sciUnknownType.sci", -1);

    mfile2sci(mfilesPath + functionName + ".m", TMPDIR, %F, %F, -1, %T);

    loadmatfile(mfilesPath + functionName + ".mat");

    m2sciInfos = mgetl(TMPDIR + filesep() + functionName + "_varslist.dia.ref");

    m2sciInfos = m2sciInfos(grep(m2sciInfos,"//res"));

    exec("SCI/modules/m2sci/tests/unit_tests/utils/m2sciParseInfos.sci", -1);
    exec("SCI/modules/m2sci/tests/unit_tests/utils/m2sciCompareInfos.sci", -1);


    for kInfos = 1:size(m2sciInfos, "*")
        resName = part(m2sciInfos(kInfos), 3:(min(strindex(m2sciInfos(kInfos), "|"))-1));
        if ~m2sciCompareInfos(resName, m2sciInfos(kInfos), evstr(resName + "_Infos")) then
            testOk = %F;
            return
        end
    end

    testOk = %T;

endfunction
