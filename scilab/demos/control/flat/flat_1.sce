// Copyright  INRIA

mode(-1);

while %t then

  exec('SCI/demos/control/flat/flat_ch.sce');
  [num]=demo_choose('SCI/demos/control/flat/flat_ch.bin');

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
