
getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_file_choice.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

demo_begin;

demo_message('SCI/demos/simulation/dae/dae1/dae1_1.sce');

demo_run('SCI/demos/simulation/dae/dae1/dae1_2.sce');

demolist=["Index 2" , "pend3d2_gateway.sce",
          "Index 1" , "pend3d1_gateway.sce",
          "Index 1 with stabilization" , "pend3d1s_gateway.sce"]
          
demo_file_choice('SCI/demos/simulation/dae/dae1/','no');

demo_end();
