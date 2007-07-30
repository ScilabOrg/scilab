// Copyright INRIA

getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_file_choice.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('dae2_gateway.sce');

demo_message('SCI/demos/simulation/dae/dae2/dae2_1.sce');

demo_run('SCI/demos/simulation/dae/dae2/dae2_2.sce');

//la variable demolist dans demo_run est perdue après execution
//c'est la variable de simulation.dem qui est récupérée par demo_file_choice
demolist=["parabola" , "pendg1_gateway.sce" ;
          "Upside down camel" , "pendg2_gateway.sce"];

demo_file_choice(path,'no');

demo_end();

