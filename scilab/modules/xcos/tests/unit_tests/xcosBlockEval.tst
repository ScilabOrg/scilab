// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Unitary test for the xcosBlockEval macro -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7129
//
// <-- Short Description -->
// The xcosBlockEval macro is used on Xcos to update SuperBlocks I/O blocks 
// from the context data.

funcprot(0); // avoid function assignation message

ioBlocks = ["IN_f", "OUT_f", "INIMPL_f", "OUTIMPL_f", "CLKIN_f", "CLKOUT_f", ..
            "CLKINV_f", "CLKOUTV_f"];

for index = 1:size(ioBlocks, '*')
    cmd = "blk = " + ioBlocks(index) + "(""define"");";
    if (execstr(cmd, "errcatch") <> 0) then pause, end;
    
    // context and block settings
    context = ["myVariable = 42;"];
    blk.graphics.exprs = "myVariable";
    
    new_blk = xcosBlockEval(evstr(ioBlocks(index)), blk, context);
    
    // check settings
    if (new_blk.graphics.exprs <> "myVariable") then pause, end;
    if (new_blk.model.ipar <> 42) then pause, end;
end

