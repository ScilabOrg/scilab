
// Copyright INRIA

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');
getf('SCI/demos/demos_lib/demo_choose.sci');

demo_begin;

getf('SCI/demos/optimization/lmitool/lmidem.sci');

demo_message('SCI/demos/optimization/lmitool/lmi_1.sce');

demo_run('SCI/demos/optimization/lmitool/lmi_2.sce');

demo_end();
