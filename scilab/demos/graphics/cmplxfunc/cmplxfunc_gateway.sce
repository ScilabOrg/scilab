// demo pour visualisation des fonctions elementaires
// complexes - Bruno Pincon

// Adapted for new graphic by Pierre MARECHAL ( 16/12/2005 )

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('cmplxfunc_gateway.sce');

exec(path+"cmplxfunc.sci");
getf(path+"MacCmplx.sci")  //  les macros utilisees ici

demo_run(path+'cmplxfunc_1.sce');

demo_end();

