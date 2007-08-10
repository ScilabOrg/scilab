getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('pendule_gateway.sce');

getf(path+'simulation.sci')
getf(path+'graphics.sci')
//
demo_run(path+'pendule_1.sce');

demo_end();
