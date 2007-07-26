function [k,wh]=getobj(scs_m,pt)
//** 24/07/07: Al@n's patch for rotation of blocks
//** 26/07/07: Al@n : case for text
// Copyright INRIA

//** get the number of obj in scs_m
n=lstsize(scs_m.objs)

//** init variable with defult value
wh=[]
data=[]
k=[]
eps=6

//** x,y are the mouse coord.
x=pt(1);
y=pt(2);

//** loop on objects
for i=n:-1:1
  o=scs_m.objs(i)

  select typeof(o)

    case 'Block' then

      [orig,sz]=(o.graphics.orig,o.graphics.sz)

      orig = orig-eps;
      sz   = sz+2*eps;

      if %pt<>[] then
        xxx=rotate([pt(1);pt(2)],...
                   -o.graphics.theta*%pi/180,...
                   [orig(1)+sz(1)/2;orig(2)+sz(2)/2])
        x=xxx(1)
        y=xxx(2)
      end

      data=[(orig(1)-x)*(orig(1)+sz(1)-x),..
            (orig(2)-y)*(orig(2)+sz(2)-y)]

      if data(1)<0&data(2)<0 then
        k=i
        break
      end

  case 'Text' then
      [orig,sz]=(o.graphics.orig,o.graphics.sz)

      orig=orig-eps;
      sz=sz+2*eps;

      if %pt<>[] then
        xxx=rotate([pt(1);pt(2)],...
                   -o.graphics.theta*%pi/180,...
                   [orig(1)+sz(1)/2;orig(2)+sz(2)/2]);
        x=xxx(1);
        y=xxx(2);
      end

      data=[(orig(1)-x)*(orig(1)+sz(1)-x),..
            (orig(2)-y)*(orig(2)+sz(2)-y)]

      if data(1)<0&data(2)<0 then
        k=i
        break
      end

  case 'Link' then
    xx=o.xx;
    yy=o.yy;
    [d,ptp,ind]=dist2polyline(xx,yy,pt)
    if d<eps/2 then
      k=i
      wh=ind
      break
    end
  end
end
endfunction
