function texte = standard_document(objet, k)
//**
// Copyright INRIA
//**   
// standard_document - Scicos block documentation 
//
// Copyright INRIA

  type_objet = typeof(objet)
  //
  select type_objet

   case "Text" then
    texte = 'This is a TEXT block'
    
    case "Block" then
    //- Initialisations 
    modele = objet.model
    graphique = objet.graphics
    macro = objet.gui
    //
    fonction = modele.sim
    if prod(size(fonction)) > 1 then
      if fonction(2) == 0 then
	language = '0 (Scilab function type Scicos 2.2)'
      elseif fonction(2) == 1  then
	language = '1 (Fortran or C code)'
      elseif fonction(2) == 2 then
	language = '2 (C code)'
      elseif fonction(2) == 3 then
	language = '3 (Scilab function)'
      elseif fonction(2) == 4 then
	language = '4 (C code)'
      elseif fonction(2) == 5 then
	language = '5 (Scilab function)'
      elseif fonction(2) == 99 then
	language = '99 (Scicos debug block)'
       elseif fonction(2) < 0  then
	language = '<0 (synchro block)'
      elseif fonction(2) <10000 then
	language = string(fonction(2))+'  (dynamic link or...)'
      else
	language = string(fonction(2))+'  (internally implicit bloc)'
      end
    else
      if modele.equations==list() then
	language = '0 (Scilab function type Scicos 2.2)'
      else
	language= 'implicit Modelica block'
      end
    end
    //

    if modele.blocktype == 'l' then
      typ= 'synchro'
    elseif modele.blocktype == 'x'
      typ= 'x-type'
    elseif modele.blocktype == 'm'
      typ = 'memory'
    else
      typ = 'regular'
    end
    //
    if modele.dep_ut(1) then
      dependance_u = 'yes'
    else
      dependance_u = 'no'

    end

    if modele.dep_ut(2) then
      dependance_t = 'yes'
    else
      dependance_t = 'no'
    end
    //
    identification = graphique.id

    //- Informations generales 

    if modele.sim=='super'|modele.sim=='csuper' then
      texte = ['General Information'; 
	       '--------------------';' '
	       'object type                : Super Block';   ..
	       'Identification             : '+identification; ..
	       'Object number in diagram   : '+string(k); ' ';  ..
	       'Drawing function           : '+macro;' ']
    else
      texte = ['General Information'; 
	       '--------------------';' '
	       'object type                : bloc standard';   ..
	       'Identification             : '+identification; ..
	       'Object number in diagram   : '+string(k); ' ';  ..
	       'Drawing function           : '+macro;           ..
	       'Simulation function        : '+fonction(1);     ..
	       'Simulation Function type   : '+language;' ';    ..
	       'Bloc type                  : '+typ;             ..
	       'Direct feed through        : '+dependance_u;    ..
	       'Permanently active         : '+dependance_t]
      if %cpr<>list()&needcompile<>4 then
	cor = %cpr.cor
	corinv = %cpr.corinv
	path=list()
	for kp=1:size(super_path,'*'),path(kp)=super_path(kp);end
	path($+1)=k
	ind=cor(path)

	if ind>0&ind<=size(corinv) then
	  txt = ['Compiled structure Index   : '+string(cor(path)); ' ']
	else
	  txt = ['Compiled structure Index   : suppressed'; ' ']
	end
      else
	txt = ['Compiled structure Index   : Not available';' ']
      end
      texte=[texte;txt]
    end
    //- Entrees / sorties 

    //tableau = ['Port type', 'Number', 'Size', 'Link'; '-', '-', '-', '-']
    tableau = ['Port type', 'Number', 'I/E', 'Size', 'Data type', 'Link';
               '-', '-', '-', '-', '-', '-']
    //- Entrees standard
    for i = 1 : min(size(modele.in,'*'),size(graphique.pin,'*'))
      // General information
      txt = ['Regular input', string(i)]
      // Info. for implicit/explicit port
      ierr=execstr('tt=string(graphique.in_implicit(i))','errcatch')
      if ierr<>0 then
        txt = [txt, '?'] //not specified
      else
        if tt==[] then 
          txt = [txt, '?'] //not specified
        else 
          txt = [txt, tt]
        end
      end
      // Info. for size of port
      txt = [txt ,'['];
      tt=string(modele.in(i)) //first dim.
      if tt==[] then 
        txt($) = txt($)+'?' //not specified
      else 
        txt($) = txt($)+tt
      end
      txt($)=txt($)+';'
      ierr=execstr('tt=string(modele.in2(i))','errcatch')//sec. dim.
      if ierr<>0 then 
        txt($) = txt($)+'?' //not specified
      else
        if tt==[] then 
          txt($) = txt($)+'?' //not specified
        else 
          txt($) = txt($)+tt
        end
      end
      txt($)=txt($)+']'
      // Info for data type
      ierr=execstr('ttyp=modele.intyp(i)','errcatch')
      if ierr<> 0 then
        tt='?' //not specified
      else
        if ttyp==-1 then tt='-1'
        elseif ttyp==1 tt='double'
        elseif ttyp==2 tt='complex'
        elseif ttyp==3 tt='int32'
        elseif ttyp==4 tt='int16'
        elseif ttyp==5 tt='int8'
        elseif ttyp==6 tt='uint32'
        elseif ttyp==7 tt='uint16'
        elseif ttyp==8 tt='uint8'
        else tt='?' //Unknown
        end
      end
      txt = [txt, tt]
      //Info for link
      txt = [txt,string(graphique.pin(i))]
      //update tableau
      tableau = [tableau; txt]
    end

    //- Sorties standard
    for i = 1 : min(size(modele.out,'*'),size(graphique.pout,'*'))
      //General informations.
      txt = ['Regular output', string(i)]
      // Info. for implicit/explicit port
      ierr=execstr('tt=string(graphique.out_implicit(i))','errcatch')
      if ierr<>0 then
          txt = [txt, '?'] //not specified
      else
        if tt==[] then 
          txt = [txt, '?'] //not specified
        else 
          txt = [txt, tt]
        end
      end
      // Info. for size of port
      txt = [txt ,'['];
      tt=string(modele.out(i)) //first dim.
      if tt==[] then 
        txt($) = txt($)+'?' //not specified
      else 
        txt($) = txt($)+tt
      end
      txt($)=txt($)+';'
      ierr=execstr('tt=string(modele.out2(i))','errcatch') //sec. dim.
      if ierr<>0 then
          txt($) = txt($)+ '?' //not specified
      else
        if tt==[] then 
          txt($) = txt($)+ '?' //not specified
        else 
          txt($) = txt($)+ tt
        end
      end
      txt($)=txt($)+']'
      // Info for data type
      ierr=execstr('ttyp=modele.outtyp(i)','errcatch')
      if ierr<>0 then
        tt='?' //not specified
      else
        if ttyp==1 then tt='double'
        elseif ttyp==2 tt='complex'
        elseif ttyp==3 tt='int32'
        elseif ttyp==4 tt='int16'
        elseif ttyp==5 tt='int8'
        elseif ttyp==6 tt='uint32'
        elseif ttyp==7 tt='uint16'
        elseif ttyp==8 tt='uint8'
        else tt='?' //Unknown
        end
      end
      txt = [txt, tt]
      //Info for link
      txt = [txt,string(graphique.pout(i))]
      //update tableau
      tableau = [tableau; txt]
    end

    //- Entrees evenements 
    for i = 1 : min(size(modele.evtin,'*'),size(graphique.pein,'*'))
      tableau = [tableau; 'Event input', string(i),'',..
		 string(modele.evtin(i)),'', string(graphique.pein(i))]
    end
    //- Sorties evenements 
    for i = 1 : min(size(modele.evtout,'*'),size(graphique.peout,'*'))
      tableau = [tableau; 'Event output', string(i),'',..
		 string(modele.evtout(i)),'', string(graphique.peout(i))]
    end
    //
    texte = [texte; 'Input / output'; 
	     '--------------';
	     ' '
	     tabule(tableau); ' ']
    // Documentation
    documentation=objet.doc
    if type(documentation)==15 then
      if size(documentation)>=2 then
	funname=documentation(1);doc=documentation(2)
	if type(funname)==10 then 
	  ierr=execstr('docfun='+funname,'errcatch')
	  if ierr<>0 then
	    x_message('function '+funname+' not found')
	    return
	  end
	else
	  docfun=funname
	end
	ierr=execstr('doc=docfun(''get'',doc)','errcatch')
	if ierr==0&doc<>[] then
	  texte = [texte; 'Documentation'; 
		   '-------------';
		   ' '
		   doc; ' ']
	end
      end
    end

    //= Liaisons 
    case 'Link' then
    //- Initialisation 
    identification = objet.id
    if objet.ct(2) == 1 then
      sous_type = 'Regular Link'
    else
      sous_type = 'Event link'
    end
    //- Informations generales 
    texte = ['General informations'; 
	     '--------------------';' '
	     'Object type                : '+sous_type;
	     'Object Identification      : '+identification'; 
	     'Object number in diagram   : '+string(k); ' ']
    from=objet.from
    if %cpr<>list() then
      if sous_type == 'Regular Link' then 
	scs_m_tmp=scs_m
	
	while %t
	  obji=scs_m_tmp.objs(from(1))
	  if obji.model.sim=='lsplit' then
	    
	  elseif obji.model.sim=='super' then
	    super_path;super_path($+1)=from(1)
	    scs_m_tmp=obji.model.rpar
	    for obji=scs_m_tmp.objs
	      if typeof(obji)=='Block' then
		if obji.model.sim=='output'&obji.model.ipar==from(2) then
		  break
		end
	      end
	    end
	  else
	    break
	  end
	  #link=obji.graphics.pin
	  from=scs_m_tmp.objs(#link).from
	end
	cor = %cpr.cor
	path=list()
	for kp=1:size(super_path,'*'),path(kp)=super_path(kp);end
	path($+1)=from(1)
	ind=cor(path)
	if type(ind)==1 then
	  kl=%cpr.sim('outlnk')(%cpr.sim('outptr')(ind)+(from(2)-1));
          kmin=[];kmax=[];count=1;
          for j=1:size(%cpr.state('outtb'))
            if j==kl then
              kmin=count;
              kmax=count+size(%cpr.state.outtb(j),'*')-1;
              break
            else
              count=count+size(%cpr.state.outtb(j),'*');
            end
          end
          txt =   ['Compiled link memory zone  : outtb('+string(kl)+')']
          txt=[txt;'                     Area  : ['+string(kmin)+':'+string(kmax)+']']
          txt=[txt;'                     Type  : '+typeof(%cpr.state.outtb(kl))']
          txt=[txt;'                     Size  : '+sci2exp(size(%cpr.state.outtb(kl)))';' ']
	else
	  txt = ['Compiled link memory zone  : Not available';' ']
	end
      end
    else
      txt = ['Compiled link memory zone  : Not available';' ']
    end
    texte=[texte;txt]   
    
    //- Connexions 

    tableau = [' ', 'Block', 'Port' ; '-', '-', '-'; 
	       'From', string(objet.from(1:2)); 
	       'to', string(objet.to(1:2))] 
    texte = [texte; 
	     'Connections'; 
	     '-----------';' ' 
	     tabule(tableau); ' ']
    //
  else
    texte=[]
  end
endfunction
