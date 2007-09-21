exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_message.sci');
exec('SCI/demos/demos_lib/demo_run.sci');
exec('SCI/demos/demos_lib/demo_end.sci');

demo_begin;

demo_message('SCI/demos/simulation/ode/ode_vanderpol/ode_vanderpol_1.sce');

demo_message('SCI/demos/simulation/ode/ode_vanderpol/ode_vanderpol_2.sce');

demo_run('SCI/demos/simulation/ode/ode_vanderpol/ode_vanderpol_2.sce');

demo_end();
