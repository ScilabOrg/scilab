model PerteDP 
  parameter Real L "Longueur du tube";
  parameter Real D "Diametre interne du tube";
  parameter Real lambda "Coefficient de perte de charge-frottement (s.u.)";
  parameter Real z1 "Altitude entree tuyauterie";
  parameter Real z2 "Altitude sortie tuyauterie";
  parameter Real p_rho "Si > 0, masse volumique impos�e du fluide";
//  parameter Integer mode "R�gion IF97. 1:liquide - 2:vapeur - 4:saturation - 0:calcul automatique";
  
  parameter Real g = 9.80665 "Acc�l�ration de la pesanteur";
  parameter Real pi = 3.1415927;
  parameter Real eps = 1.e-0 "Limite inf. de la fonction carr�e";
  Real khi "Coefficient de perte de charge hydraulique";
  Real deltaPf "Perte de charge par frottement";
  Real deltaP "Perte de charge totale";
  Real Q (start=500) "D�bit massique";
  Real rho (start=998) "Masse volumique";
  Real Tm (start=290) "Temp�rature moyenne";
  Real Pm (start=1.e5) "Pression moyenne";
  Real Hm (start=100000) "Enthalpie sp�cifique moyenne";
  Real region (start=1) "Num�ro de r�gion IF97";
  Real ThermoCar;

public 
  PortPHQ1 C1 ;
  PortPHQ2 C2 ;

equation

  C1.P - C2.P = deltaP;
  C2.Q = C1.Q;
  C2.H = C1.H;
  
  Q = C1.Q;

  /* Inversions de d�bit */
  0 = if (Q > 0.0) then (C1.H - C1.Hm) else (C2.H - C2.Hm);
  
  /* Calcul des pertes de charges */
  deltaP = deltaPf + rho*g*(z2 - z1);

  ThermoCar = if (abs(Q) > eps) then Q*abs(Q) else Q*eps;
  deltaPf = 8*khi*ThermoCar/(pi^2.0*D^4.0*rho);
  
  
    /* Formule de Darcy-Weisbach (Idel'cik p. 55). On suppose qu'on est en r�gime quadratique. */
  khi = lambda*L/D;
  
  /* Calcul des propri�t�s thermodynamiques */
  Pm = (C1.P + C2.P)/2;
  Hm = (C1.H + C2.H)/2;
  

  Tm = Hm/4187 + 273.15;
  region = 1;
  
  rho = 998;

end PerteDP;
