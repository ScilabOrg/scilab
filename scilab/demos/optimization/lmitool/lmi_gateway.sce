
// Copyright INRIA

exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_run.sci');
exec('SCI/demos/demos_lib/demo_message.sci');
exec('SCI/demos/demos_lib/demo_end.sci');
exec('SCI/demos/demos_lib/demo_choose.sci');

demo_begin;

exec('SCI/demos/optimization/lmitool/lmidem.sci');

demo_message('SCI/demos/optimization/lmitool/lmi_1.sce');

demo_run('SCI/demos/optimization/lmitool/lmi_2.sce');

demo_end();
