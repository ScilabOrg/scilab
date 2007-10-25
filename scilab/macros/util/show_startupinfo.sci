function show_startupinfo()
  //used to inform users 
  
  // In in No Window mode then returns...
  if ~isempty(grep(sciargs(),["-nw","-nwni"])) then
    return
  end
  
  settings_file=pathconvert(SCIHOME+'/.scilab_settings',%f,%t)
  w=fileinfo(settings_file);
  global LANGUAGE
  if  LANGUAGE=='fr' then
    DialogNewGraphic=["Attention:"
              " "
              "This software is being provided ""as is"", without any express"
              "or implied warranty. This is a development and not at all a"
              "final or stable version. In no event will the authors be held"
              "liable for any damages arising from, out of or in connection"
              "with the software or the use or other dealings in the software."
              " "
              "Les versions Scilab 4.x seront les dernières à fonctionner"
		      "avec l''ancien mode graphique."
		      " "
		      "Veuillez lire le fichier release_notes pour plus de détails."
		      " "
		      "Cliquez ""Oui"" si vous ne désirez plus voir cet avertissement"]
    Buttons=['Oui','Non']
  else
    DialogNewGraphic=["Warning:"
              " "
              "This software is being provided ""as is"", without any express"
              "or implied warranty. This is a development and not at all a"
              "final or stable version. In no event will the authors be held"
              "liable for any damages arising from, out of or in connection"
              "with the software or the use or other dealings in the software."
              " "
		      "Scilab 4.x versions family will be the last versions working"
		      "with the old graphics mode."
		      " "
		      "Please read the release_notes file for more details."
		      " "
		      "Click ""Yes"" if you don''t wish anymore to see this warning."]
    Buttons=['Yes','No']
  end
  if ~MSDOS  then
    show=%t
    if w <> [] then
      show=grep(mgetl(SCIHOME+'/.scilab_settings'),'displayDialogNewGraphic=no')==[]
    end
    if show
      if x_message(DialogNewGraphic, Buttons) == 1 then
	mputl('displayDialogNewGraphic=no',SCIHOME+'/.scilab_settings')
      end
    end
  end
endfunction
