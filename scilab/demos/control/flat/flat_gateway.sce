// Copyright  INRIA

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');
getf('SCI/demos/demos_lib/demo_choose.sci');

[path,oldln]=demo_begin('flat_gateway.sce');

getf(path+'car.sci');
getf(path+'truck.sci');

demo_run(path+'flat_1.sce');

demo_end();
