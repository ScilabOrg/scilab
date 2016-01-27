// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
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

function M1=%hm_m_s(M1,M2)

    siz1 = size(M1);
    siz2 = size(M2);

    if size(M2,"*")<>1 then
        if length(siz1)<>3 then
            error(msprintf(_("%s: Wrong size for input argument #%d: 3D maximum expected.\n"),"%hm_m_s",2));
        end
        if siz1(2)<>siz2(1) then
            error(msprintf(_("%s: Wrong size for argument: Incompatible dimensions.\n"),"hm_m_s"));
        end
        res = zeros(siz1(1), siz2(2), siz1(3));
        for i=1:siz1(3)
            res(:, :, i) = M1(:, :, i)*M2;
        end
        M1 = res;
    else
        M1.entries = M1.entries*M2
    end

endfunction
