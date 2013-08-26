// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function xclear(win_num)
    //xclear([win_num])
    // Clear the graphic window win_num and erase the recorded graphics of
    // window win_num
    // if win_num is omited, it's the current graphic window
    // win_num can also be a vector of window Id to clear a set of windows
    //!


    [lhs,rhs]=argn(0);
    if (rhs == 0) then
        // clear the current figure
        set(gcf(), "visible", "off");
    elseif (rhs == 1) then
        // clear a set of figures
        if (type(win_num) <> 1) then
            error(999, msprintf(gettext("%s: Wrong type for input argument #%d: A vector expected.\n"), "xclear", 1));
            return;
        end

        if (winsid() == []) then
            // no current figure
            curFig = [];
        else
            curFig = gcf();
        end

        for figNum = win_num,
            set(scf(figNum), "visible", "off");
        end

        // restore current figure
        if (curFig <> []) then
            scf(curFig);
        end

    else
        error(999, msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "xclear", 0, 1));
        return;
    end
endfunction

