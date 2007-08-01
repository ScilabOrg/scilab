getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_choose.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('lqg_gateway.sce');

demo_run('SCI/demos/control/lqg/lqg_1.sce');

demo_message('SCI/demos/control/lqg/lqg_2.sce');

exec('SCI/demos/control/lqg/lqg_ch.sce');
demo_choose('SCI/demos/control/lqg/lqg_ch.bin');

demo_run('SCI/demos/control/lqg/lqg_3.sce');

demo_end();


