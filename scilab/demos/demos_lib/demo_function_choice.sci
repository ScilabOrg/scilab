function []=demo_function_choice()
  while %t then
    num=tk_choose(demolist(:,1),'Choose a demo');
    if num==0 then
      xdel(0);
      lines(oldln(1));
      return;
    else
    xdel(0);
	  xbasc();
      execstr(demolist(num,2));
    end,
  end
mode(0);
endfunction
