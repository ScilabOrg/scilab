
getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('anim4_gateway.sce');

demo_message('SCI/demos/graphics/anim/anim4/anim4_1.sce');

demo_run('SCI/demos/graphics/anim/anim4/anim4_1.sce');

demo_end();


