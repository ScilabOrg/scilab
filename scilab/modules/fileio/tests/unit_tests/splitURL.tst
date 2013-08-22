// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrigh (C) 2011 - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//splitURL

[a,b,c,d]=splitURL("http://www.scilab.org");
assert_checkequal(a, "http");
assert_checkequal(b, "www.scilab.org");
assert_checkequal(c, "");
assert_checkequal(d, "");

[a,b,c,d]=splitURL("http://www.scilab.org/");
assert_checkequal(a, "http");
assert_checkequal(b, "www.scilab.org");
assert_checkequal(c, "/");
assert_checkequal(d, "");

[a,b,c,d]=splitURL("http://www.scilab.org/products/scilab/environment");
assert_checkequal(a, "http");
assert_checkequal(b, "www.scilab.org");
assert_checkequal(c, "/products/scilab/environment");
assert_checkequal(d, "");

[a,b,c,d]=splitURL("http://www.scilab.org/content/search?SearchText=plot");
assert_checkequal(a, "http");
assert_checkequal(b, "www.scilab.org");
assert_checkequal(c, "/content/search");
assert_checkequal(d, "SearchText=plot");

[a,b,c,d]=splitURL("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README");
assert_checkequal(a, "ftp");
assert_checkequal(b, "ftp.free.fr");
assert_checkequal(c, "/pub/Distributions_Linux/debian/README");
assert_checkequal(d, "");

[a,b,c,d]=splitURL("https://encrypted.google.com");
assert_checkequal(a, "https");
assert_checkequal(b, "encrypted.google.com");
assert_checkequal(c, "");
assert_checkequal(d, "");

[a,b,c,d,e,f,g]=splitURL("https://plop:ae@encrypted.google.com:443/full/path?query=true#myFragment");
assert_checkequal(a, "https");
assert_checkequal(b, "encrypted.google.com");
assert_checkequal(c, "/full/path");
assert_checkequal(d, "query=true");
assert_checkequal(e, "plop:ae");
assert_checkequal(f, int32(443)); // port
assert_checkequal(g, "myFragment"); // fragment

// Badly formated URL
assert_checkerror("splitURL(''http://plop@ae:sylvestre.ledru.info:80'');", [], 999);
