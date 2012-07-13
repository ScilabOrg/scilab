// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// unit tests strsplit
//===============================
STR_SPLITED = ['strsplit splits';'a string';'into';'a vector of strings'];
STR = 'strsplit splits a string into a vector of strings';
INDICES = [15 25 30];
R = stripblanks(strsplit(STR,INDICES));
assert_checkequal(R, STR_SPLITED);
//===============================
assert_checkequal(strsplit([],[1 1 1]), []);
//===============================
// Scilab 4.x 
//if execstr('strsplit([],[3 2 1])','errcatch') <> 99  then pause,end
// Scilab 5.x
assert_checkequal(strsplit([],[3 2 1]), []);
//===============================
assert_checkequal(execstr('strsplit('',[0 1])','errcatch'), 31);
// Scilab 4.x 
//if execstr('strsplit([])','errcatch') <> 39  then pause,end
// Scilab 5.1
//if execstr('strsplit([])','errcatch') <> 77  then pause,end
// Scilab 5.2
assert_checkequal(execstr('strsplit([])','errcatch'), 0);
assert_checkequal(strsplit([]), []);
//===============================
ref_1 = ["toto"];
ref_2 = [];
[r_1, r_2] = strsplit("toto","a");
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
//===============================
// request 663
ref_1 = ["a";"b";"c";"d";"e";"f"];
ref_2 = ["";"";"";"";"";""];
[r_1, r_2] = strsplit("abcdef");
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
assert_checkequal(strcat(r_1, ""), "abcdef");
//===============================
[r_1, r_2] = strsplit("abcdef","");
[r_3, r_4] = strsplit("abcdef");
assert_checkequal(r_1, r_3);
assert_checkequal(r_2, r_4);
//===============================
ref_1 = ["a";"bcdef"];
ref_2 = [""];
[r_1, r_2] = strsplit("abcdef","",1);
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
assert_checkequal(strcat(r_1,""), "abcdef");
//===============================
ref_1 = ["abcdef";"ghijkl";"mnopqr";"stuvw";"xyz"];
ref_2 = [",";",";",";","];
[r_1, r_2] = strsplit("abcdef,ghijkl,mnopqr,stuvw,xyz",",");
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
assert_checkequal(strcat(r_1,","), "abcdef,ghijkl,mnopqr,stuvw,xyz");
//===============================
ref_1 = ["abc";"";"";"def";"";"ghijkl";"";"mno"];
ref_2 = [":";":";":";":";":";":";":"];
[r_1, r_2] = strsplit("abc:::def::ghijkl::mno",":");
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
assert_checkequal(strcat(r_1,":"), "abc:::def::ghijkl::mno");
//===============================
ref_1  = ["abcdef";"ghijkl";"mnopqr";"stuvw";"xyz"];
ref_2  = ["~~~";"~~~";"~~~";"~~~"] ;
[r_1, r_2] = strsplit("abcdef~~~ghijkl~~~mnopqr~~~stuvw~~~xyz","~~~");
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
assert_checkequal(strcat(r_1,"~~~"), "abcdef~~~ghijkl~~~mnopqr~~~stuvw~~~xyz");
//===============================
ref_1 = ["abcdef";"ghijkl";"mnopqr";"stuvw";"xyz"];
ref_2 = ["2";"3";"6";"7"];
[r_1, r_2] = strsplit("abcdef2ghijkl3mnopqr6stuvw7xyz","/\d+/");
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
//===============================
ref_2 = ["scilab";"scilab"];
ref_1 = ["";" a numerical tools ";"oraty"];
[r_1, r_2] = strsplit("scilab a numerical tools scilaboraty","/scilab/");
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
//===============================
ref_1  = ["abcdef";"ghijkl";"mnopqr";"stuvw";"xyz"];
ref_2  = [ascii(9);ascii(9);ascii(9);ascii(9)];
[r_1, r_2] = strsplit("abcdef"+ascii(9)+"ghijkl" + ascii(9)+"mnopqr"+ascii(9)+"stuvw" + ascii(9)+"xyz","/\t/") ;
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
assert_checkequal(strcat(r_1,ascii(9)), "abcdef" + ascii(9) + "ghijkl" + ascii(9) + "mnopqr" + ascii(9) + "stuvw" + ascii(9) + "xyz");
//===============================
ref_1  = ["server.name";"scilab.org"];
ref_2  = "       = ";
linestr = "server.name       = scilab.org";
[r_1, r_2] = strsplit(linestr, "/\s*=\s*/");
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
assert_checkequal(strcat(r_1,r_2), "server.name       = scilab.org");
//===============================
ierr = execstr("strsplit(''root:x:0:0:root:/root:/bin/bash'','':'',0)","errcatch");
assert_checkequal(ierr, 999);
//===============================
ref_1 = ["root";"x:0:0:root:/root:/bin/bash"];
ref_2 = ":";
[r_1, r_2] = strsplit("root:x:0:0:root:/root:/bin/bash",":",1);
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
//===============================
ref_1 = ["root";"x";"0";"0";"root";"/root:/bin/bash"];
ref_2 = [":";":";":";":";":"];
[r_1, r_2] = strsplit("root:x:0:0:root:/root:/bin/bash",":",5);
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
//===============================
[r_1, r_2] = strsplit("root:x:0:0:root:/root:/bin/bash",":",7);
[r_3, r_4] = strsplit("root:x:0:0:root:/root:/bin/bash",":",50);
assert_checkequal(r_1, r_3);
assert_checkequal(r_2, r_4);
//===============================
ref_1 = ["abc";""];
ref_2 = ',';
[r_1, r_2] = strsplit("abc,",",");
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
//===============================
ref_1 = ["abc";""];
ref_2 = ',';
[r_1, r_2] = strsplit("abc,",",",1);
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
//===============================
[r_1, r_2] = strsplit("abc,",",",1);
[r_3, r_4] = strsplit("abc,",",",10);
assert_checkequal(r_1, r_3);
assert_checkequal(r_2, r_4);
//===============================
ref_1 = ["abc";"def";"ijk";"";"lmo"];
ref_2 = [",";":";",";":"];
[r_1, r_2] = strsplit("abc,def:ijk,:lmo","/:|,/");
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
//===============================
ref_1 = ["abc";"def";"ijk";"";"lmo"];
ref_2 = [",";":";",";":"];
[r_1, r_2] = strsplit("abc,def:ijk,:lmo",[":";","]);
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
//===============================
ref_1 = ["abc";"def";"ijk,:lmo"];
ref_2 = [",";":"];
[r_1, r_2] = strsplit("abc,def:ijk,:lmo",[":";","],2);
assert_checkequal(ref_1, r_1);
assert_checkequal(ref_2, r_2);
//===============================
v = "世界您好";
c = "您";
[r_1,r_2] = strsplit(v,c);
assert_checkequal(strcat(r_1,r_2), v);
//===============================
v = "азеаея";
c = "з";
[r_1,r_2] = strsplit(v,c);
assert_checkequal(strcat(r_1,r_2), v);
//===============================
v = "ድቅስድስግ";
c = "ቅ";
[r_1,r_2] = strsplit(v,c);
assert_checkequal(strcat(r_1,r_2), v);
//===============================
v = "ハロー・ワールド";
c = "ド";
[r_1,r_2] = strsplit(v,c);
assert_checkequal(strcat(r_1,r_2), v);
//===============================
v = "תוכנית";
c = "י";
[r_1,r_2] = strsplit(v,c);
assert_checkequal(strcat(r_1,r_2), v);
//===============================
