// Copyright  INRIA

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');
getf('SCI/demos/demos_lib/demo_choose.sci');

demo_begin;

getf('SCI/demos/control/flat/car.sci');
getf('SCI/demos/control/flat/truck.sci');

demo_run('SCI/demos/control/flat/flat_1.sce');

demo_end();
