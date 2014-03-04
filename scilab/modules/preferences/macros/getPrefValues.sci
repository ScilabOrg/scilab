// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Get preferences values
// - xpath is something like "//web/body/proxy", the target must be a single node
// - attributes is a matrix of strings containing the attributes names
//
function values = getPrefValues(xpath, attributes)
    if type(xpath) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "getPrefValues", 1));
    end

    if type(attributes) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "getPrefValues", 2));
    end

    doc = xmlRead(SCIHOME + "/XConfiguration.xml");

    try
        xp = xmlXPath(doc, xpath);
    catch
        xmlDelete(doc);
        error(msprintf(gettext("%s: Invalid XPath request.\n"), "getPrefValues"));
    end

    if xp.size ~= 1 then
        xmlDelete(doc);
        error(msprintf(gettext("%s: Invalid XPath request."), "getPrefValues"));
    end

    node = xp(1);
    if node.type ~= "XML_ELEMENT_NODE" then
        xmlDelete(doc);
        error(msprintf(gettext("%s: Target node is not a XML_ELEMENT_NODE."), "getPrefValues"));
    end

    attr = node.attributes;
    values = [];
    for a = attributes(:)'
        v = attr(a);
        if ~isempty(v) then
            values = [values v];
        else
            xmlDelete(doc);
            error(msprintf(gettext("%s: Invalid attribute name: %s."), "getPrefValues", a));
        end
    end

    xmlDelete(doc);
    values = matrix(values, size(attributes));
endfunction
