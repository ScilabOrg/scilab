// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function operation=%l_i_operatio(field,inslist,operation)
    if field=="dims" then
        operation.out(1).dims=inslist
    elseif field=="type" then
        operation.out(1).type=inslist
    else
        error(gettext("Not yet implemented."))
    end
endfunction
