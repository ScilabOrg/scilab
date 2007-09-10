model SineVoltage "Sine voltage source" 
  parameter Real V=1 "Amplitude of sine wave";
  parameter Real ph=0 "Phase of sine wave";
  parameter Real frq=1 "Frequency of sine wave";
  parameter Real offset=0 "Offset volatge of sine wave";
  parameter Real start=0 "sine wave start time";
  parameter Real PI=3.141592653589793;
  Pin p, n;
  Real v;
  
equation

  v = offset + (if time < start then 0 else V*Modelica.Math.sin(2*PI*frq*(time - start) + ph));
  v= p.v - n.v;	
  0 = p.i + n.i;
end SineVoltage;


