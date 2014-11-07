// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();

// Allocate a summation block
o = BIGSOM_f("define")
o.graphics
o.model
o.gui
o.doc

// Set the block to itself without making any modification
// (scicos_getvalue returns BIGSOM_f default parameters)
p = funcprot();
funcprot(0);
function [ok,sgn,exprs] = scicos_getvalue(title, field, Type, exprs)
    ok = %t;
    sgn = [1; 1];
    exprs = sci2exp(sgn);
endfunction
funcprot(p);

o = BIGSOM_f("set", o)
o.graphics
o.model

// Now use scicos_getvalue to modify the 'exprs' and 'rpar' fields to "[-1;-1]" and '[-1;-1]'
p = funcprot();
funcprot(0);
function [ok,sgn,exprs] = scicos_getvalue(title, field, Type, exprs)
    ok = %t;
    sgn = [-1; -1];
    exprs = sci2exp(sgn);
endfunction
funcprot(p);

o = BIGSOM_f("set", o)
o.graphics
o.model

// Now modify the block's entry port #2 to '1' and add a new one with value '1'
p = funcprot();
funcprot(0);
function [ok,sgn,exprs] = scicos_getvalue(title, field, Type, exprs)
    ok = %t;
    sgn = [-1; 1; 1];
    exprs = sci2exp(sgn);
endfunction
funcprot(p);

o = BIGSOM_f("set", o)
o.graphics
o.model

// Try with another block containing event inputs
o = CSCOPE("define")
o.graphics
o.model
o.gui
o.doc

// Set the block to itself without making any modification
// (scicos_getvalue returns CSCOPE_f default parameters)
p = funcprot();
funcprot(0);
function [ok,clrs,win,wpos,wdim,ymin,ymax,per,N,heritance,nom,exprs] = scicos_getvalue(title, fields, Type, exprs)
    ok = %t;
    clrs = model.ipar(4:11)
    win = model.ipar(1)
    wpos = model.ipar(12:13)
    wdim = model.ipar(14:15)
    ymin = model.rpar(2)
    ymax = model.rpar(3)
    per = model.rpar(4)
    N = model.ipar(3)
    heritance = 0
    nom = model.label
    exprs = graphics.exprs;
endfunction
funcprot(p);
o = CSCOPE("set", o)

// Test 'opar' and 'odstate'
o = LOGIC("define")
o.model.opar
typeof(o.model.opar(1))
o.model.opar = list([1 2 3; 4 5 6], ["Hello" "world!"; "Test" "123"], [%i 2 3*%i; 4 5*%i 6], uint32([32 32; 32 32]));
o.model.opar

o.model.odstate = list([1 2 3; 4 5 6], ["Hello" "world!"; "Test" "123"], [%i 2 3*%i; 4 5*%i 6], uint32([32 32; 32 32]));
o.model.odstate

// Test 'equations'
o = VsourceAC("define")
o.model.equations
o.model.equations = modelica();
o.model.equations

// Test 'exprs' with Modelica Block
o = MBLOCK("define")
o.graphics
o.graphics.exprs
o.model.equations


// Check that all the model items are freed
clear
