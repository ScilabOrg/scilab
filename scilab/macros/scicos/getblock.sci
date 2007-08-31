function k = getblock(scs_m, pt)
//** Copyright INRIA
//**
//** 24/07/07: Al@n's patch for rotation of blocks
//**
//** 31/08/07: Simone code optimizion and varius comments 

//** Magic Numbers 
xf = 60; //** default block dimension 
yf = 40;

dx = xf/7 ; //** default tollerance band around 
dy = yf/7 ; //** the block 
//**--------------------------------------------

x = pt(1); //** "click" position 
y = pt(2);

data = []; //** init 
k    = []; 

n = lstsize(scs_m.objs); //** numbers of objects in the diagram 

for i=n:-1:1 //** scan all the scs_m objects 
  k = i
  o = scs_m.objs(i)
  
  if typeof(o)=="Block" then

    [orig, sz] = (o.graphics.orig,o.graphics.sz);

    if %pt<>[] then
      xxx = rotate([pt(1);pt(2)],...
                   -o.graphics.theta * %pi/180,...
                   [orig(1) + sz(1)/2; orig(2) + sz(2)/2]);
      x = xxx(1);
      y = xxx(2);
    end

    data = [(orig(1)-dx-x)*(orig(1)+sz(1)+dx-x),...
            (orig(2)-dy-y)*(orig(2)+sz(2)+dy-y) ]

    if data(1)<0 & data(2)<0 then
      return ; //** EXIT on the first object found
               //** with [k] index  
    end

  end //** Blocks
  
end //**... for loop 

k = [] ; //** not object was found  

endfunction

