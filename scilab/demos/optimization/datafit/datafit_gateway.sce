getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');


[path,oldln]=demo_begin('datafit_gateway.sce');

demo_message('SCI/demos/optimization/datafit/demo_datafit.sci');

getf('SCI/demos/optimization/datafit/demo_datafit.sci')
demo_datafit()

demo_end();
