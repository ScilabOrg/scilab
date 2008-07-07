// Selection de la position d'un toolbox dans la liste des toolboxes disponibles.
// avril 2008 by Delphine

function position = atomsSelectPosition(listDesc, nomToolbox, versionToolbox, checkVersionScilab)
  [n, m] = size(listDesc("Toolbox"))
  if versionToolbox <> ""
    [signeToolbox, versionToolbox] = atomsSeparateSignVersion(versionToolbox)
    versionToolbox = atomsDecoupVersion(versionToolbox)
  end
  for i=1:n
    if (atomsVerifVersionScilab(listDesc("ScilabVersion")(i)) & checkVersionScilab) | ~checkVersionScilab
      if versionToolbox == "" & listDesc("Toolbox")(i) == nomToolbox
        position = i
        // Recherche de version plus récente
        for j=i+1:n
          if (atomsVerifVersionScilab(listDesc("ScilabVersion")(j)) & checkVersionScilab) | ~checkVersionScilab
            if listDesc("Toolbox")(j) == nomToolbox
              vi = atomsDecoupVersion(listDesc("Version")(position))
              vj = atomsDecoupVersion(listDesc("Version")(j))
		      if atomsCompareVersion(vj, vi) == 1
		        position = j
		      end
            end
          end      
        end
        return position
      elseif listDesc("Toolbox")(i) == nomToolbox
        cmp = atomsCompareVersion(atomsDecoupVersion(listDesc("Version")(i)), versionToolbox);
        if (signeToolbox == "<=" & cmp <= 0) | (signeToolbox == ">=" & cmp >= 0) | (signeToolbox == "=" & cmp == 0)
          position = i
          // Recherche de version plus récente
          for j=i+1:n
            if (atomsVerifVersionScilab(listDesc("ScilabVersion")(j)) & checkVersionScilab) | ~checkVersionScilab
              cmp = atomsCompareVersion(atomsDecoupVersion(listDesc("Version")(j)), versionToolbox);
              if listDesc("Toolbox")(j) == nomToolbox & ((signeToolbox == "<=" & cmp <= 0) | (signeToolbox == ">=" & cmp >= 0) | (signeToolbox == "=" & cmp == 0))
                vi = atomsDecoupVersion(listDesc("Version")(position))
                vj = atomsDecoupVersion(listDesc("Version")(j))
	  	        if atomsCompareVersion(vj, vi) >= 0
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
