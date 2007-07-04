function []=demo_run(info,expr)  // display and execute //
  x_message([info;expr]);
  execstr(expr);
endfunction
