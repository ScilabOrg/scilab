// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

loadXcosLibs();

p = funcprot();
funcprot(0);
// Overload scicos_getvalue to customize parameters
function [ok,sgn,exprs] = scicos_getvalue(title, field, type, exprs)
    ok = %t;
    sgn = [-1;-1];
    exprs = sci2exp(sgn);
endfunction
funcprot(p);

// Allocate an Annotation
o = BIGSOM_f("define")
o.graphics
o.model
o.gui
o.doc

// Modify it, o.graphics.exprs was set to "[-1;-1]" in scicos_getvalue
o = BIGSOM_f("set", o)
o.graphics
o.model
o.gui
o.doc
