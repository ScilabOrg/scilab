// Selection des toolboxes correspondant à la bonne version de scilab
// juin 2008 by Delphine

function result = verifVersionScilab(scilabVersion)
  // On récupère la version du logiciel
  currentVersion = getversion()
  index = strindex(currentVersion, "-")
  currentVersion = strsplit(currentVersion,index)
  [a, b]=size(index)
  // S'il y a plus d'un "-" c'est soit trunk, soit une alpha/beta/rc
  if b == 3 
    modifCurrentVersion = currentVersion(3) + currentVersion(4) 
  end
  currentVersion = currentVersion(2)
  currentVersion = strsubst(currentVersion, "-", "")
  // On met en forme la version de scilab de la toolbox
  scilabVersion = strsubst(scilabVersion, " ", "")
  [signeScilab, versionScilab] = separateSignVersion(scilabVersion)
  // On regarde si la aussi on n'a pas affaire a une alpha/beta/rc
  index = strindex(versionScilab, "-")
  if index <> []
    versionScilab = strsplit(versionScilab,index)
    modifVersionScilab = versionScilab(2) + versionScilab(3)
    versionScilab = versionScilab(1)
    versionScilab = strsubst(versionScilab, "-", "")
  end
  // On regarde les différents cas pour savoir si les versions concordent
  if currentVersion == "trunk"
    if signeScilab == ">="
      result = %T
    else
      result = %F
    end
  else
    if compareVersion(decoupVersion(versionScilab), decoupVersion(currentVersion)) == 0
      // On regarde si on a des modificateurs de version
      if isdef("modifCurrentVersion") & isdef("modifVersionScilab")
        if verifVersionModif(modifCurrentVersion, modifVersionScilab) == 0
          result = %T
        elseif signeScilab == "<=" & verifVersionModif(modifCurrentVersion, modifVersionScilab) == -1
          result = %T
        elseif signeScilab == ">=" & verifVersionModif(modifCurrentVersion, modifVersionScilab) == 1
          result = %T
        else
          result = %F
        end
      elseif isdef("modifCurrentVersion") & ~isdef("modifVersionScilab") & signeScilab == "<="
        result = %T
      elseif ~isdef("modifCurrentVersion") & isdef("modifVersionScilab") & signeScilab == ">="
        result = %T
      else
        result = %F
      end
    elseif signeScilab == "<=" & compareVersion(decoupVersion(versionScilab), decoupVersion(currentVersion)) == 1
      result = %T
    elseif signeScilab == ">=" & compareVersion(decoupVersion(versionScilab), decoupVersion(currentVersion)) == -1
      result = %T
    else
      result = %F
    end
  end
endfunction

function result = verifVersionModif(vScilab, vTool)
  index = strindex(vScilab, "-")
  vScilab = strsplit(vScilab,index)
  index = strindex(vTool, "-")
  vTool = strsplit(vTool,index)
  // On les compare : 1 si vScilab > vTool; 0 si vScilab = vTool; -1 si vScilab < vTool
  // alpha < beta < rc
  if length(vScilab(1)) > length(vTool(1))
    return -1
  elseif length(vScilab(1)) < length(vTool(1))
    return 1
  else
    if strtod(vScilab(2)) > strtod(vTool(2))
      return 1
    elseif strtod(vScilab(2)) < strtod(vTool(2))
      return -1
    else
      return 0
    end
  end
endfunction


