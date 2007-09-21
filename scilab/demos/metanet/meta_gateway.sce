exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_run.sci');
exec('SCI/demos/demos_lib/demo_message.sci');
exec('SCI/demos/demos_lib/demo_choose.sci');
exec('SCI/demos/demos_lib/demo_end.sci');

demo_begin;

exec('SCI/demos/metanet/metro.sci','c');

demo_run('SCI/demos/metanet/meta_1.sce');

demo_end();
