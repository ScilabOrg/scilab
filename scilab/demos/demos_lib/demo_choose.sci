function [num]=demo_choose(fil)

load(fil,'choice','titl');

num=tk_choose(choice,titl);
endfunction
