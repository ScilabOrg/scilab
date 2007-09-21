exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_message.sci');
exec('SCI/demos/demos_lib/demo_choose.sci');
exec('SCI/demos/demos_lib/demo_run.sci');
exec('SCI/demos/demos_lib/demo_end.sci');
exec('SCI/demos/demos_lib/demo_mdialog.sci');

demo_begin;

demo_message('SCI/demos/control/lqg/lqg_3.sce');

demo_run('SCI/demos/control/lqg/lqg_1.sce');

demo_message('SCI/demos/control/lqg/lqg_2.sce');

demo_run('SCI/demos/control/lqg/lqg_3.sce');

demo_end();


