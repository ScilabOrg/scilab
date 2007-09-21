// This is just a graphic demo of the bike
// Copyright INRIA

exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_message.sci');
exec('SCI/demos/demos_lib/demo_run.sci');
exec('SCI/demos/demos_lib/demo_end.sci');
exec('SCI/demos/demos_lib/demo_choose.sci');

demo_begin;

exec('SCI/demos/simulation/bike/macros/'+'velod.sci');
exec('SCI/demos/simulation/bike/macros/'+'velodp.sci');
exec('SCI/demos/simulation/bike/macros/'+'show.sci');


demo_message('SCI/demos/simulation/bike/bike.sce');

demo_run('SCI/demos/simulation/bike/bike.sce');

demo_end();
