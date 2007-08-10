getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');


[path,oldln]=demo_begin('datafit_gateway.sce');

demo_message(path+'demo_datafit.sci');

getf(path+'demo_datafit.sci')
demo_datafit()

demo_end();
