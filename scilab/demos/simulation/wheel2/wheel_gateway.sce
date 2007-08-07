 
getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_compiler.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_choose.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('wheel_gateway.sce');

getf('SCI/demos/simulation/wheel2/show.sci');

demo_message('SCI/demos/simulation/wheel2/wheel_1.sce');

exec('SCI/demos/simulation/wheel2/wheel2_ch.sce');
demo_choose('SCI/demos/simulation/wheel2/wheel2_ch.bin');

demo_run('SCI/demos/simulation/wheel2/wheel_1.sce');

demo_end();
