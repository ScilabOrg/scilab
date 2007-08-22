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

demo_begin;

demolist=['Finite Element display with colorbar','fec_gateway.ex4';
	  'Finite Element display with a mesh','fec_gateway.ex2';
	  'Sfgrayplot ','fec_gateway.ex3'];

demo_file_choice('SCI/demos/graphics/fec/','no');

demo_end();
