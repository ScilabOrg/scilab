// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Set preferences values
// - xpath is something like "//web/body/proxy", the target must be a single node
// - kv is a matrix of strings 2xN: keys are in the first row and values in the
//   seconde one.
//
function setPrefValues(xpath, kv)
    if type(xpath) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "setPrefValues", 1));
    end

    if type(kv) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "setPrefValues", 2));
    end

    if (size(kv, "r") ~= 2) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: a 2xN maxtrix expected.\n"), "setPrefValues", 2));
    end

    doc = xmlRead(SCIHOME + "/XConfiguration.xml");

    try
        xp = xmlXPath(doc, xpath);
    catch
        xmlDelete(doc);
        error(msprintf(gettext("%s: Invalid XPath request.\n"), "setPrefValues"));
    end

    if xp.size ~= 1 then
        xmlDelete(doc);
        error(msprintf(gettext("%s: Invalid XPath request."), "setPrefValues"));
    end

    node = xp(1);
    if node.type ~= "XML_ELEMENT_NODE" then
        xmlDelete(doc);
        error(msprintf(gettext("%s: Target node is not a XML_ELEMENT_NODE."), "setPrefValues"));
    end

    attr = node.attributes;
    for i = 1:size(kv, "c")
        v = attr(kv(1, i));
        if ~isempty(v) then
            attr(kv(1, i)) = kv(2, i);
        else
            xmlDelete(doc);
            error(msprintf(gettext("%s: Invalid attribute name: %s."), "setPrefValues", kv(1, i)));
        end
    end

    xmlWrite(doc);
    xmlDelete(doc);
endfunction
