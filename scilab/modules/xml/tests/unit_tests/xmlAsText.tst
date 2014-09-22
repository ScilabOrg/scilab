// ===========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================

// <-- CLI SHELL MODE -->

doc = xmlReadStr("<root><a>bug</a><a>13</a><a>1.2345678</a><a>b</a><a>.23E-2</a></root>");
xp = xmlXPath(doc, "//a/text()");
assert_checkequal(xmlAsText(xp),  ["bug","13","1.2345678","b",".23E-2"]);
assert_checkequal(xmlAsText(doc.root.children),  ["bug","13","1.2345678","b",".23E-2"]);

xmlFile=xmlRead(SCI+"/modules/xml/tests/unit_tests/w3cExample.xml");
plop=xmlXPath(xmlFile, "/bookstore/book/price");
assert_checkequal(xmlAsText(plop),  ["30.00","29.99","49.99","39.95","0.0"]);
firstElement= ["Everyday Italian","Giada De Laurentiis","2005","30.00"];
assert_checkequal(xmlAsText(xmlFile.root.children(1).children), firstElement);
