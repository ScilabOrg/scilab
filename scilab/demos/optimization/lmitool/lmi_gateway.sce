
// Copyright INRIA

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');
getf('SCI/demos/demos_lib/demo_choose.sci');

[path,oldln]=demo_begin('lmi_gateway.sce');
getf(path+'lmidem.sci');

demo_message(path+'lmi_1.sce');

demo_run(path+'lmi_2.sce');

demo_end();
