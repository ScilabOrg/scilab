exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_message.sci');
exec('SCI/demos/demos_lib/demo_end.sci');


demo_begin;

demo_message(SCI+'/demos/optimization/multiflow/demo_multiflow.sci');

exec(SCI+'/demos/optimization/multiflow/demo_multiflow.sci')
demo_multiflow(SCI+'/demos/optimization/multiflow/ex3.graph')

demo_end();
