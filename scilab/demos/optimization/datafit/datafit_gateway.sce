exec('SCI/demos/demos_lib/demo_begin.sci');
exec('SCI/demos/demos_lib/demo_message.sci');
exec('SCI/demos/demos_lib/demo_end.sci');


demo_begin;

demo_message('SCI/demos/optimization/datafit/demo_datafit.sci');

exec('SCI/demos/optimization/datafit/demo_datafit.sci')
demo_datafit()

demo_end();
