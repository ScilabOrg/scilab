// ==========================================
// Copyright INRIA
// Copyright Enpc
// last modified : 14th, December 2005
// ==========================================


getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_run.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('anim1_gateway.sce');

demo_message('SCI/demos/graphics/anim/anim1/anim1_1.sce');

demo_run('SCI/demos/graphics/anim/anim1/anim1_1.sce');

demo_end();
