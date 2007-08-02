getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

getf(SCI+'/demos/metanet/metro.sci','c');

[path,oldln]=demo_begin('meta_gateway.sce');

demo_run('SCI/demos/metanet/meta_1.sce');

demo_end();
