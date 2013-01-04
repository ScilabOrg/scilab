// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//getURL

myFile=getURL("http://sylvestre.ledru.info");
assert_checkequal(myFile, "index.html")
inf=fileinfo(myFile);
assert_checkequal(inf(1)), 0);

myFile=getURL("http://sylvestre.ledru.info/");
assert_checkequal(myFile, "index.html")
inf=fileinfo(myFile);
assert_checkequal(inf(1), 0);


myFile=getURL("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README");

assert_checkequal(myFile, "README")
inf=fileinfo(myFile);
assert_checkequal(inf(1), 0);


targetFile=TMPDIR+"/README_Debian";
myFile=getURL("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README",targetFile);

assert_checkequal(myFile, targetFile)
inf=fileinfo(targetFile);
assert_checkequal(inf(1), 0)
f1 = mopen(targetFile,"r");
assert_checkequal(grep(mgetl(f1),"Linux"), []);
mclose(f1);


targetFile="README";
myFile=getURL("ftp://ftp.free.fr/pub/Distributions_Linux/debian/README",TMPDIR);

assert_checkequal(myFile, TMPDIR+"/"+targetFile)
inf=fileinfo(TMPDIR+"/"+targetFile);
assert_checkequal(inf(1), 0);
f1 = mopen(TMPDIR+"/"+targetFile,"r");
assert_checkequal(grep(mgetl(f1),"Linux"), []);
mclose(f1);

// HTTPS
myFile=getURL("https://encrypted.google.com")
assert_checkequal(myFile, "index.html");
inf=fileinfo(myFile);
assert_checkequal(inf(1), 0);

targetFile=TMPDIR+"/testauth";
myFile=getURL("http://httpbin.org/basic-auth/user/passwd",targetFile,"user","passwd");

assert_checkequal(myFile, targetFile)
inf=fileinfo(targetFile);
assert_checkequal(inf(1), 0)
f1 = mopen(targetFile,"r");
assert_checkequal(grep(mgetl(f1),"authenticated"), [])
mclose(f1);

// Badly formated URL
assert_checkerror("getURL(''http://plop@ae:sylvestre.ledru.info:80'');", [], 999);
