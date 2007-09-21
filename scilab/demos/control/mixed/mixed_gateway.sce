exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_run.sci');
exec('SCI/demos/demos_lib/demo_message.sci');
exec('SCI/demos/demos_lib/demo_end.sci');
exec('SCI/demos/demos_lib/demo_mdialog.sci');

demo_begin;

demo_message('SCI/demos/control/mixed/mixed_3.sce');

demo_run('SCI/demos/control/mixed/mixed_1.sce');

demo_message('SCI/demos/control/mixed/mixed_2.sce');

demo_run('SCI/demos/control/mixed/mixed_3.sce');

demo_end();
