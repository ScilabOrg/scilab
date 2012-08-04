// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012-2012 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

txt = "Angel: Raphaël Archange";
current_field_length           = regexp(txt,"/:\s/","o");
current_field                  = part(txt,1:current_field_length-1);

assert_checkequal(current_field, "Angel");

disp(txt);