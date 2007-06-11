model Puits
  parameter Real P0=100000 "Pression du puits";
  parameter Real T0=290"Temp�rature du puits";
  parameter Real H0=100000 "Enthalpie sp�cifique du puits";
  parameter Real option_temperature=1 "1:temp�rature fix�e - 2:enthalpie fix�e";

  Real Pm "Pression moyenne";
  Real Q "D�bit";
  Real Tm "Temp�rature moyenne";
  Real Hm "Enthalpie sp�cifique moyenne";

public 
  PortPHQ1 C ;
equation 
  
  C.P = Pm;
  C.Q = Q;
  C.Hm = Hm;

  Pm = P0;
  Hm= H0;

  
  Tm = if (option_temperature <= 1) then T0 else Hm/4187 + 273.15;

end Puits;
