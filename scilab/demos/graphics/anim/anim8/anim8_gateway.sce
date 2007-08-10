
getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('anim8_gateway.sce');

demo_message(path+'anim8_1.sce');

demo_run(path+'anim8_1.sce');

demo_end();





