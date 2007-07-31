getf('SCI/demos/demos_lib/demo_mdialog.sci');

demolist = [
	"log","func1()";
	"exp","func2()";
	"tan","func3()";
	"atan","func4()";
	"sin","func5()";
	"asin","func6()";
	"cos","func7()";
	"acos","func8()";
	"sinh","func9()";
	"asinh","func10()";
	"cosh","func11()";
	"acosh","func12()";
	"tanh","func13()";
	"atanh","func14()";
	"custom","func15()";]

Title = [
	"visualisation of  complex";
	"elementary  functions  on";
	"a centered disk or square";
	"";
	"    Choose a function    "];

title_custom = [
	"---BE CAREFUL NOTHING IS PROTECTED---";
	"                                     ";
	" To draw your own complex  function :";
	"                                     ";
	"1/ define the  function by a correct ";
	"   string where  the complex var must";
	"   be z ; also  as  the function will";
	"   be evaluated  on  a matrix  don''t ";
	"   forget the . to operate in element";
	"   wize meaning ; examples :         ";
	"   z.^2     (z+1).*(z-1)   (1)./(z+2)";
	"   sqrt(z)  (z+%i).*(z-%i) 1+2*z+z.^2";
	"                                     ";
	"2/ define the type of the domain     ";
	"   string Square or Disk             ";
	"                                     ";
	"3/ define the ""radius"" R of the domain";
	"                                     ";
  "4/ may be your function has a kind of";
  "   discontinuity on Ox  or  Oy => put";
  "   the string Ox or Oy or No if not  "];
title_items_custom = [
	"1/ string     ";
	"2/ Type Domain";
	"3/ R          ";
	"4/ Cut on Axe "];
rep_init = ["(1)./((z+2).*(2-z))";"Disk";"1.9";"No"];
rep = rep_init;       

while %t
	
	[num]=tk_choose(demolist(:,1),Title);
	
	if num == 0 then	
		break
	else
	  if num == 15
	     [rep]=demo_mdialog(title_custom,title_items_custom, rep);
		  if rep == [] then
		    rep = rep_init
		  else
			 select rep(4)
				  case "Ox" ; e = 0.001; TypeCut = "Ox";
				  case "Oy" ; e = 0.001; TypeCut = "Oy";
				  else e=0  ; TypeCut = "Ox" ; rep(4) = "No" ;
			 end
			 deff("Z=f(z)","Z="+rep(1))
			 theta = -110; alpha = 75;
			 R = evstr(rep(3));
			 StrFunc="f";
			 select rep(2)
				  case "Square"
					 PlotCmplxFunc(R,e,"Square",TypeCut,41,"f",theta,alpha,[0;0]);
				  case "Disk"
					 PlotCmplxFunc(R,e,"Disk",TypeCut,[40 20],"f",theta,alpha,[0;0]);
			 end
		  end
		 else
		  execstr(demolist(num,2));
		 end
	end
end
