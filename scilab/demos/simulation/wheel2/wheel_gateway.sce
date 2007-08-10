 
getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_compiler.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_choose.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('wheel_gateway.sce');

getf(path+'show.sci');

demo_message(path+'wheel_1.sce');

exec(path+'wheel2_ch.sce');
demo_choose(path+'wheel2_ch.bin');

demo_run(path+'wheel_1.sce');

demo_end();
