// This is just a graphic demo of the bike
// Copyright INRIA

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('bike_gateway.sce');

getf(path+'macros/'+'velod.sci');
getf(path+'macros/'+'velodp.sci');
getf(path+'macros/'+'show.sci');

demo_message('SCI/demos/simulation/bike/bike.dem');

demo_run('SCI/demos/simulation/bike/bike_1.sce');

demo_end();
