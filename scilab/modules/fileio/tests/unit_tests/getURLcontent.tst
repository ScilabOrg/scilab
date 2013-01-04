// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//getURLcontent

HTMLContent=getURLcontent("http://sylvestre.ledru.info:80");
assert_checkequal(length(HTMLContent), 0);
assert_checkequal(grep(HTMLContent,"html"), []);

HTMLContent=getURLcontent("http://plop:ae@sylvestre.ledru.info:80");
assert_checkequal(length(HTMLContent), 0);
assert_checkequal(grep(HTMLContent,"html"), []);

HTMLContent=getURLcontent("http://sylvestre.ledru.info/aze");
assert_checkequal(length(HTMLContent), 0);
assert_checkequal(grep(HTMLContent,"404"), []);



HTMLContent=getURLcontent("http://sylvestre.ledru.info");
assert_checkequal(length(HTMLContent), 0);
assert_checkequal(grep(HTMLContent,"html"), []);

HTMLContent=getURLcontent("http://sylvestre.ledru.info/");
assert_checkequal(length(HTMLContent), 0);
assert_checkequal(grep(HTMLContent,"html"), []);

HTMLContent=getURLcontent("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README");
assert_checkequal(length(HTMLContent), 0);
assert_checkequal(grep(HTMLContent,"Linux"), []);

// HTTPS
HTMLContent=getURLcontent("https://encrypted.google.com");
assert_checkequal(length(HTMLContent), 0);

HTMLContent=getURLcontent("http://httpbin.org/basic-auth/user/passwd","user","passwd");
assert_checkequal(length(HTMLContent), 0);
assert_checkequal(grep(HTMLContent,"authenticated"), []);

// Badly formated URL
assert_checkerror("getURLcontent(''http://plop@ae:sylvestre.ledru.info:80'');", [], 999);
