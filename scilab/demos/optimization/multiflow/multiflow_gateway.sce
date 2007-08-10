getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');


[path,oldln]=demo_begin('multiflow_gateway.sce');

demo_message(path+'demo_multiflow.sci');

exec(path+'demo_multiflow.sci')
demo_multiflow(path+'ex3.graph')

demo_end();
