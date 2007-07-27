function disablemenus(men)
// Copyright INRIA

//** Disable the dynamic menu to avoid unintentional command generation during
//** interactive operations (Move, OpenSet, Link creation, etc.)

[lhs,rhs] = argn()

xinfo("Please be patient...") ;

//curwin=xget('window') //** Please left this instruction commented 

if rhs<1 then
  men = menus(1)
end

for k=1:size(men,'*')
  unsetmenu(curwin,men(k)); //** 
end

endfunction
