// Selection de la position d'un toolbox dans la liste des toolboxes disponibles.
// avril 2008 by Delphine

function position = selectPosition(listDesc, nomToolbox, versionToolbox)
  [n, m] = size(listDesc("Toolbox"))
  if versionToolbox <> ""
    [signeToolbox, versionToolbox] = separateSignVersion(versionToolbox)
    versionToolbox = decoupVersion(versionToolbox)
  end
  for i=1:n
    if (verifVersionScilab(listDesc("ScilabVersion")(i)) & checkVersionScilab == "oui") | checkVersionScilab == "non"
      if versionToolbox == "" & listDesc("Toolbox")(i) == nomToolbox
        position = i
        // Recherche de version plus récente
        for j=i+1:n
          if (verifVersionScilab(listDesc("ScilabVersion")(j)) & checkVersionScilab == "oui") | checkVersionScilab == "non"
            if listDesc("Toolbox")(j) == nomToolbox
              vi = decoupVersion(listDesc("Version")(position))
              vj = decoupVersion(listDesc("Version")(j))
		      if compareVersion(vj, vi) == 1
		        position = j
		      end
            end
          end      
        end
      return position
      elseif listDesc("Toolbox")(i) == nomToolbox
        if (signeToolbox == "<=" | signeToolbox == "=") & (compareVersion(decoupVersion(listDesc("Version")(i)), versionToolbox) == -1) | ((signeToolbox == ">=" | signeToolbox == "=") & compareVersion(decoupVersion(listDesc("Version")(i)), versionToolbox) == 1)
        position = i
          // Recherche de version plus récente
          for j=i+1:n
            if (verifVersionScilab(listDesc("ScilabVersion")(j)) & checkVersionScilab == "oui") | checkVersionScilab == "non"
              if listDesc("Toolbox")(j) == nomToolbox & ((signeToolbox == "<=" | signeToolbox == "=") & (compareVersion(decoupVersion(listDesc("Version")(j)), versionToolbox) == -1) | ((signeToolbox == ">=" | signeToolbox == "=") & compareVersion(decoupVersion(listDesc("Version")(j)), versionToolbox) == 1))
                vi = decoupVersion(listDesc("Version")(position))
                vj = decoupVersion(listDesc("Version")(j))
	  	        if compareVersion(vj, vi) == 0 | compareVersion(vj, vi) == 1
		          position = j
		        end
              end
            end
          end
          return position
        end
      end
    end
  end
  // Toolbox non trouvée
  // Cas ou on a regardé en local
  if n == 1
    position = 0.1
  // Cas ou on a regardé sur les repository
  else
    position = 0
  end
endfunction
