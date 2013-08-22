// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug G -->
//
// <-- OLD BUG -->

// C'est un cas non prevu le break est appele par while sans un if dans ce cas
// le while ne sert a rien.

correct = %F
I       = 0;

while 1
    while 1
        break;
    end

    // les lignes suivantes ne sont jamais executes !!!!!!
    correct = %T
    I=I+1;
    if I >= 5 then break; end

end

if ~correct then pause,end
