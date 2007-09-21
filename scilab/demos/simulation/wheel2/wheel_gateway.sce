 
exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_compiler.sci');
exec('SCI/demos/demos_lib/demo_message.sci');
exec('SCI/demos/demos_lib/demo_run.sci');
exec('SCI/demos/demos_lib/demo_choose.sci');
exec('SCI/demos/demos_lib/demo_end.sci');


demo_begin;


exec('SCI/demos/simulation/wheel2/show.sci');

demo_message('SCI/demos/simulation/wheel2/wheel.sce');

exec('SCI/demos/simulation/wheel2/wheel_ch.sce');
num = demo_choose('SCI/demos/simulation/wheel2/wheel_ch.bin');

demo_run('SCI/demos/simulation/wheel2/wheel.sce');

demo_end();
