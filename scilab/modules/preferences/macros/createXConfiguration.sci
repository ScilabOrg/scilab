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
// - doc (optional): the prefs xml document where to set the values
//                   (take care: in this case xmlWrite is not called)
//
function createXConfiguration()
    path = SCIHOME + "/XConfiguration.xml";
    if fileinfo(path) ~= [] then
        return
    end;

    doc = xmlRead(SCI + "/modules/preferences/etc/XConfiguration.xml");
    modules = ls(SCI + "/modules/*");
    xmls = [];
    for m = modules'
        xmls = [xmls ; ls(m + "/etc/XConfiguration-*.xml")];
    end

    children = doc.root.children;

    meta = "ctrl";
    if getos() == "Darwin" then
        meta = "meta";
    end

    for xml = xmls'
        xdoc = xmlRead(xml);
        if basename(xml) == "XConfiguration-general" then
            xp = xmlXPath(xdoc, "//shortcuts/body/actions/action-folder/action[contains(@key,''OSSCKEY'')]");
            for i = 1:xp.size
                e = xp(i);
                attr = e.attributes;
                attr.key = strsubst(attr.key, "OSSCKEY", meta);
            end
        end
        pos = xmlXPath(doc, "count(//" + xdoc.root.name + "/preceding-sibling::*)+1");
        if children(pos).name == xdoc.root.name then
            children(pos) = xdoc.root;
        end
        xmlDelete(xdoc);
    end

    xmlWrite(doc, path, %T);
    xmlDelete(doc);
endfunction

