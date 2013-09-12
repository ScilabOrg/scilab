//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//

function [status, message] = xcosValidateCompareBlock(block1, block2)
    status = %t;
    message = "";

    if (typeof(block1) <> "Block")
        error(999, sprintf(_("%s: Wrong type for argument #%d: A Block expected."), "xcosValidateCompareBlock", 1))
    end
    if (typeof(block2) <> "Block")
        error(999, sprintf(_("%s: Wrong type for argument #%d: A Block expected."), "xcosValidateCompareBlock", 2))
    end

    unconsistantFields = [];

    if or(block1.gui <> block2.gui)
        status = %f;
        unconsistantFields = [ unconsistantFields ; "gui"]
    end

    if or(block1.doc <> block2.doc)
        status = %f;
        unconsistantFields = [ unconsistantFields ; "doc"]
    end

    //
    // Compare block.graphics
    //
    graphics1 = block1.graphics;
    graphics2 = block2.graphics;
    graphicFields = ["orig" ; "sz" ; "flip" ; "theta" ; "exprs" ;
    "pin" ; "pout" ; "pein" ; "peout" ; "gr_i" ;
    "id"; "in_implicit" ; "out_implicit"]

    for i=1:size(graphicFields, "*")
        if or(graphics1(graphicFields(i)) <> graphics2(graphicFields(i)))
            status = %f;
            unconsistantFields = [ unconsistantFields ; "graphics." + graphicFields(i)]
        end
    end

    //
    // Compare block.model
    //
    model1 = block1.model;
    model2 = block2.model;
    modelFields = [ "sim" ;
    "in" ; "in2" ; "intyp" ;
    "out" ; "out2" ; "outtyp" ;
    "evtin" ; "evtout" ;
    "state" ; "dstate" ; "odstate" ;
    "rpar" ; "ipar" ; "opar" ;
    "blocktype" ;
    "firing" ; "dep_ut" ;
    "label" ;
    "nzcross" ; "nmode" ;
    "equations"
    ]
    for i=1:size(modelFields, "*")
        if or(model1(modelFields(i)) <> model2(modelFields(i)))
            status = %f;
            unconsistantFields = [ unconsistantFields ; "model." + modelFields(i)]
        end
    end

    if ~isempty(unconsistantFields)
        message = [];
        for i=1:size(unconsistantFields, "*")
            msg = sprintf(_("Field %s has different values."), unconsistantFields(i));
            message = [message; msg];
        end
    end

endfunction
