connector PortPHQ1 
//  extends ThermoLib.Connecteurs.PortPHQ;
  Real P "Pression du fluide";
  Real H "Enthalpie sp�cifique du fluide";
  Real Q "D�bit massique du fluide";
  Real Hm "Enthalpie sp�cifique de m�lange";

//  input Boolean a=true "Pseudo-variable pour la v�rification du sens des connexions";
//  output Boolean b "Pseudo-variable pour la v�rification du sens des connexions";
end PortPHQ1;
connector PortPHQ2 
//  extends ThermoLib.Connecteurs.PortPHQ;
  Real P "Pression du fluide";
  Real H "Enthalpie sp�cifique du fluide";
  Real Q "D�bit massique du fluide";
  Real Hm "Enthalpie sp�cifique de m�lange";

//  input Boolean a "Pseudo-variable pour la v�rification du sens des connexions";
//  output Boolean b=true "Pseudo-variable pour la v�rification du sens des connexions";

end PortPHQ2;
model Source
  parameter Real P0=300000 "Pression de la source";
  parameter Real T0=290 "Temp�rature de la source";
  parameter Real H0=100000 "Enthalpie sp�cifique de la source";
  parameter Real option_temperature=1 "1:temp�rature fix�e - 2:enthalpie fix�e";
   
  Real Pm "Pression moyenne";
  Real Q "D�bit";
  Real Tm "Temp�rature moyenne";
  Real Hm "Enthalpie sp�cifique moyenne";

public 
  PortPHQ2 C ;
equation 
  
  C.P = Pm;
  C.Q = Q;
  C.Hm = Hm;

  Pm = P0;

  Hm = H0;
  
  Tm = if (option_temperature <= 1) then T0 else Hm/4187 + 273.15;
  
end Source;
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
model Bache 
  parameter Real Patm "Pression dans le ciel de la bache";
  parameter Real A "Section de la bache";
  parameter Real ze1 "Altitude du piquage d'entr�e 1";
  parameter Real ze2 "Altitude du piquage d'entr�e 2";
  parameter Real zs1 "Altitude du piquage de sortie 1";
  parameter Real zs2 "Altitude du piquage de sortie 2";
  parameter Real z0 "Altitude initiale du fluide";
  parameter Real T0 "Temperature initiale du fluide";
  parameter Real p_rho     "Si > 0, masse volumique impos�e du fluide";
  //  parameter Integer mode "R�gion IF97. 1:liquide - 2:vapeur - 4:saturation - 0:calcul automatique";  
  parameter Real g=9.80665 "Acc�l�ration de la pesanteur";
  parameter Real e=0.25 "Ouverture des piquages";
  parameter Real k=1   "Coefficient de perte de charge des piquages";
  parameter Real eps=1.e-0 "Limite inf. de la fonction carr�e";
  parameter Real zmin=1.e-6     "Hauteur minimum d'eau pour l'�quation  de l'�nergie";

  Real z(start=z0) "Hauteur du fluide";
  Real Tm(start=T0) "Temp�rature moyenne";
  Real Pm(start=1.e5) "Pression moyenne";
  Real Hm(start=100000) "Enthalpie sp�cifique moyenne";
  Real rho(start=998) "Masse volumique";
  Real BQ "Bilan de masse";
  Real BH "Bilan d'�nergie";
  Real Oe1;
  Real Oe2;
  Real Os1;
  Real Os2;
  Real region(start=1) "Num�ro de r�gion IF97";
  Real Max_e1;
  Real Max_e2;
  Real Max_s1;
  Real Max_s2;
  Real ThermoCar1;
  Real ThermoCar2;

  Real yNiveau ;

public 
//  SortieReelle yNiveau ;
  PortPHQ1 Ce1 ;
  PortPHQ2 Cs2 ;
//  PortTher Cth ; /* Port non utilis� dans le mod�le d�sir�, non implant� sur le bloc SCICOS */
  PortPHQ1 Ce2 ;
  PortPHQ2 Cs1 ;

//initial equation 

//  Hm = 100000;
  
equation

  Ce2.Q = 0;
  Ce2.H = 1.e5;

  Cs1.Q = 0;
  Cs1.H = 1.e5;

  Max_e1 = if (z - ze1 < 0.0) then 0.0 else (z - ze1);
  ThermoCar1 = if (abs(Ce1.Q) > eps) then Ce1.Q*abs(Ce1.Q) else Ce1.Q*eps;
  (Patm + rho*g*Max_e1 - Ce1.P)*Oe1 = k*ThermoCar1/2/rho;

  Oe1 = if (Ce1.Q >= 0.0) then 1.0
	elseif (z > ze1 + e) then 1.0
	elseif (z < ze1) then 0.0
	else (z - ze1)/e;


   Max_e2 = if (z - ze2 < 0.0) then 0.0 else (z - ze2);

  Ce2.P = Patm + rho*g*Max_e2;
  Oe2 = 0;

  Max_s1 = if (z - zs1 < 0.0) then 0.0 else (z - zs1);
  Cs1.P = Patm + rho*g*Max_s1;
  Os1 = 0; 

  Max_s2 = if (z - zs2 < 0.0) then 0.0 else (z - zs2);
  ThermoCar2 =if (abs(Cs2.Q) > eps) then Cs2.Q*abs(Cs2.Q) else Cs2.Q*eps;
  (Patm + rho*g*Max_s2 - Cs2.P)*Os2 = k*ThermoCar2/2/rho;

  Os2 = if (Cs2.Q <= 0.0) then 1.0
    elseif (z > zs2 + e) then 1.0
    elseif (z < zs2) then 0.0
    else (z - zs2)/e;


  /* Bilan de masse */
  BQ = Ce1.Q + Ce2.Q - Cs1.Q - Cs2.Q;
  rho*A*der(z) = BQ;
  
  /* Bilan d'�nergie */
  /* le port thermique n'est pas utilis� ici  "+ Cth.W" (on a W=0)*/
  BH = Ce1.Q*(Ce1.H - Hm) + Ce2.Q*(Ce2.H - Hm) - Cs1.Q*(Cs1.H - Hm) - Cs2.Q*(Cs2.H - Hm);

  der(Hm) = if (z > zmin) then BH/(rho*A*z) else 0.0;

  Ce1.Hm = Hm;
  Ce2.Hm = Hm;
  Cs1.Hm = Hm;
  Cs2.Hm = Hm;
  
//  Cth.T = Tm; /* le port thermique n'est pas utilis� ici*/
  
  /* Capteur de niveau */
  yNiveau = z;
  
  /* Calcul des propri�t�s thermodynamiques */
  Pm = Patm + rho*g*z/2;
  
  Tm = Hm/4187 + 273.15;
  region = 1.0;
  
  rho = 998.0;

end Bache;
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

model Flowmeter "Capteur de d�bit"
  parameter Real Qini=1 ;

  Real Q (start=10) "D�bit massique";
  Real Mesure;

  PortPHQ1 C1;
  PortPHQ2 C2;

equation 
  Mesure = Q;

  C1.P = C2.P;
  C1.H = C2.H;
  C1.Q = C2.Q;

  Q = C1.Q;
  0 = if (Q > 0) then C1.H - C1.Hm else C2.H - C2.Hm;

end Flowmeter;
