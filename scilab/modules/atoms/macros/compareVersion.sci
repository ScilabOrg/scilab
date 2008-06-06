// Comparaison de deux versions
// avril 2008 by Delphine

function result = compareVersion(v1, signe, v2)
  // On converti les string en double
  v1 = strtod(v1)
  v2 = strtod(v2)
  if v1(1) > v2(1)
    if signe == ">="
      result = %T
    else
      result = %F
    end
  elseif v1(1) ==  v2(1)
    if v1(2) > v2(2)
      if signe == ">="
        result = %T
      else
        result = %F
      end
   	elseif v1(2) == v2(2)
  	  // On regarde s'il existe un 3ème chiffre
  	  [o, m] = size(v2)
  	  [n, m] = size(v1)
  	  if n == 3 & o == 3
  	    if v1(3) > v2(3)
          if signe == ">="
            result = %T
          else
            result = %F
          end
    	elseif v1(3) == v2(3)
          result = %T
  	    else
  	      if signe == "<="
  	        result = %T
  	      else
  	        result = %F
  	      end
  	    end
      elseif n == 2 & o == 2
        result = %T
  	  else
  	    if n == 2 & v2(3) == 0 | o == 2 & v1(3) == 0
          result = %T
        elseif n == 2 & v2(3) > 0
  	      if signe == "<="
  	        result = %T
  	      else
  	        result = %F
  	      end
        elseif o == 2 & v1(3) > 0
          if signe == ">="
            result = %T
          else
            result = %F
          end
  	    else
  	      result = %F
  	    end
  	  end
  	else
  	  if signe == "<="
  	    result = %T
  	  else
  	    result = %F
  	  end
   	end
  else
    if signe == "<="
  	  result = %T
  	else
  	  result = %F
  	end
  end
endfunction

