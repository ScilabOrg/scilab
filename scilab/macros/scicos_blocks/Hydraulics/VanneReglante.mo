model VanneReglante 
  parameter Real Cvmax "CV max";
  parameter Real p_rho "Si > 0, masse volumique impos�e du fluide";
//  parameter Integer mode=0 "R�gion IF97. 1:liquide - 2:vapeur - 4:saturation - 0:calcul automatique";

  parameter Real eps=1.e-0 "Limite inf. de la fonction carr�e";
  Real Cv "Cv";
  Real Q(start=500) "D�bit massique";
  Real deltaP "Perte de charge singuli�re";
  Real rho(start=998) "Masse volumique";
  Real Tm(start=290) "Temp�rature moyenne";
  Real Pm(start=1.e5) "Pression moyenne";
  Real Hm(start=100000) "Enthalpie sp�cifique moyenne";
  Real region(start=1) "Num�ro de r�gion IF97";
  Real ThermoCar;

  Real Ouv ;

public 
//  EntreeReelle Ouv ;
  PortPHQ1 C1 ;
  PortPHQ2 C2 ;

equation 
  
  C1.H = C2.H;
  C1.Q = C2.Q;
  
  Q = C1.Q;
  deltaP = C1.P - C2.P;
  
  /* Inversions de d�bit */
  0 = if (Q > 0.0) then C1.H - C1.Hm else C2.H - C2.Hm;
  
  /* Calcul de la perte de charge */

  ThermoCar = if (abs(Q) > eps) then Q*abs(Q) else Q*eps;
  deltaP*Cv*abs(Cv) = 1.733e12*ThermoCar/rho^2;
  
  /* Calcul du Cv en fonction de l'ouverture */
  Cv = Ouv*Cvmax;
  
  /* Calcul des propri�t�s thermodynamiques */
  Pm = (C1.P + C2.P)/2;
  Hm = (C1.H + C2.H)/2;
  
  Tm = Hm/4187 + 273.15;
  region = 1;
  
  rho = 998;

end VanneReglante;
