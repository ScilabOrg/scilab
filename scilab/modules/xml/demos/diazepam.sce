clf();

function coord=displayAtom(atomName) 
disp("Rendering of  "+atomName);
atoms=xmlXPath(xmlFile,"//cmldoc/molecule/atomArray/atom[@id=''"+atomName+"'']");
nbAtoms=size(atoms);
coord=[];
for i=1:nbAtoms(2)
	atomX=xmlXPath(atoms(i),"float");
	coord=xmlAsNumber(atomX);
	rect=[coord, 1, 1];
//	xrect(rect);
    xfarc(coord(1)-0.5, coord(2)+0.5, 1, 1, 0, 360*64);	
	atomName=atoms(i).attributes.id;
    elementType=xmlXPath(atoms(i),"string[@builtin=''elementType'']");
    cercle=get("hdl");       
    select elementType.content,
    case "C" then
         cercle.background=25;
    case "N" then
         cercle.background=12;
    case "Cl" then
         cercle.background=20;
    case "0" then
         cercle.background=10;
    end 
	xstring(coord(1),coord(2)-1,atomName);

end

endfunction

//bonds=xmlXPath(xmlFile,"//cmldoc/molecule/bondArray/bond[string=''"+atomName+"'']");

xmlFile=xmlRead(SCI+"/modules/xml/demos/diazepam.xml");

plot2d(0,0,-1,"010"," ",[-5,-5,5,5])
bonds=xmlXPath(xmlFile,"//cmldoc/molecule/bondArray/bond");
nbBonds=size(bonds);
for i=1:nbBonds(2)
	bondCouple=xmlXPath(bonds(i),"string[@builtin=''atomRef'']");
	disp(bondCouple(1).content + " connected to " + bondCouple(2).content);	
	coord1=displayAtom(bondCouple(1).content);
	coord2=displayAtom(bondCouple(2).content);
	xsegs([coord1(1),coord2(1)], [coord1(2),coord2(2)]);
end

