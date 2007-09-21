// Copyright  INRIA

exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_run.sci');
exec('SCI/demos/demos_lib/demo_message.sci');
exec('SCI/demos/demos_lib/demo_end.sci');
exec('SCI/demos/demos_lib/demo_choose.sci');

demo_begin;

exec('SCI/demos/control/flat/car.sci');
exec('SCI/demos/control/flat/truck.sci');

demo_run('SCI/demos/control/flat/flat_1.sce');

demo_end();
