// ==========================================
// Copyright INRIA
// Copyright Enpc
// last modified : 14th, December 2005
//
// ==========================================

getf('SCI/demos/demos_lib/demo_begin.sci');
getf('SCI/demos/demos_lib/demo_file_choice.sci');
getf('SCI/demos/demos_lib/demo_message.sci');
getf('SCI/demos/demos_lib/demo_end.sci');

[path,oldln]=demo_begin('fec_gateway.sce');

demolist=['Finite Element display with colorbar','fec.ex4';
	  'Finite Element display with a mesh','fec.ex2';
	  'Sfgrayplot ','fec.ex3'];

demo_file_choice(path,'no');

demo_end();
