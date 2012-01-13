// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


ilib_verbose(0);
cd(TMPDIR);

exec(SCI+"/modules/localization/tests/unit_tests/CreateDir.sce", -1);

tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"프로그램",
"תוכנית"];

for i = 1 : size(tab_ref,'*')
	sz = "dir_" + tab_ref(i);
	cd(sz);
	
	//Example of the use of ilib_for_link with  a simple C code
    f1=['#include <math.h>'
    'void fooc(double c[],double a[], double *b,int *m,int *n)'
    '{'
    '   int i;'
    '   for ( i =0 ; i < (*m)*(*n) ; i++) '
    '     c[i] = sin(a[i]) + *b; '
    '}'];

   mputl(f1,'fooc.c');
   ilib_for_link('fooc','fooc.c',[],"c"); 
   exec loader.sce; 
   ulink();
   cd(TMPDIR);
end

