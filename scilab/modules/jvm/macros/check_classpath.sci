//------------------------------------
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//------------------------------------
function ret=check_classpath(varargin)
    [lhs,rhs]=argn(0);
    ret=%T;
    file_to_check=SCI+"/etc/classpath.xml";
    printf("checking : "+file_to_check+"\n");
    try
        if getos() == "Windows" then
            stat = unix_w(SCI+"\modules\helptools\bin\xmllint\xmllint --noout --valid "+file_to_check);
        else
            stat = unix_w("xmllint --noout --valid "+file_to_check);
        end
    catch
        ret=%F;
    end
endfunction
//------------------------------------
