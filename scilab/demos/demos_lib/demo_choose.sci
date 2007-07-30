function [num]=demo_choose(fil)
mopen(fil);
num=tk_choose(choice,titl);
mclose(fil);
endfunction
