// Copyright  INRIA

mode(-1);

while %t then
  num=tk_choose(demolist(:,1),'Choose a vehicle');
  if num==0 then break,end
  select num
    case 1 then
    demo_message('SCI/demos/control/flat/car.sci');
    demo_car()
    
    case 2 then 
    demo_message('SCI/demos/control/flat/truck.sci');
    demo_truck()
  end;
end,
