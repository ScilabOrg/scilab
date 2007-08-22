// Copyright INRIA

getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_file_choice.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

demo_begin;

demo_message('SCI/demos/simulation/dae/dae2/dae2_1.sce');

demo_run('SCI/demos/simulation/dae/dae2/dae2_2.sce');

demolist=["parabola" , "pendg1_gateway.sce" ;
          "Upside down camel" , "pendg2_gateway.sce"];
		  
demo_file_choice('SCI/demos/simulation/dae/dae2/','no');

demo_end();

