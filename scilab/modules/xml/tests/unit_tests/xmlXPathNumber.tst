// ===========================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================


doc = xmlReadStr("<root><a>bug</a><a>13</a><a>1.2345678</a><a>b</a><a>.23E-2</a></root>");
xp = xmlXPath(doc, "//a/text()");
assert_checkequal(xmlXPathNumber(xp), [%nan,13,1.2345678,%nan,0.0023]);

xmlFile=xmlRead(SCI+"/modules/xml/tests/unit_tests/w3cExample.xml");
plop=xmlXPath(xmlFile, "/bookstore/book/price");
assert_checkequal(xmlXPathNumber(plop), [30,29.99,49.99,39.95,0]);

