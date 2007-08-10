
getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_file_choice.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('dae1_gateway.sce');

demo_message(path+'dae1_1.sce');

demo_run(path+'dae1_2.sce');

//la variable demolist dans demo_run est perdue après execution
//c'est la variable de simulation.dem qui est récupérée par demo_file_choice

demolist=["Index 2" , "pend3d2_gateway.sce",
          "Index 1" , "pend3d1_gateway.sce",
          "Index 1 with stabilization" , "pend3d1s_gateway.sce"]
          
demo_file_choice(path,'no');

demo_end();
