// Comparaison de deux versions
// juin 2008 by Delphine

function result = atomsCompareVersion(v1, v2)
  // On converti les string en double
  v1 = strtod(v1)
  v2 = strtod(v2)
  if v1(1) > v2(1)
    result = 1
  elseif v1(1) ==  v2(1)
    if v1(2) > v2(2)
      result = 1
   	elseif v1(2) == v2(2)
  	  // On regarde s'il existe un 3ème chiffre
  	  [o, m] = size(v2)
  	  [n, m] = size(v1)
  	  if n == 3 & o == 3
  	    if v1(3) > v2(3)
          result = 1
    	elseif v1(3) == v2(3)
          result = 0
  	    else
          result = -1
  	    end
      elseif n == 2 & o == 2
        result = 0
  	  else
  	    if n == 2 & v2(3) == 0 | o == 2 & v1(3) == 0
          result = 0
        elseif n == 2 & v2(3) > 0
  	      result = -1
        elseif o == 2 & v1(3) > 0
  	      result = 1
  	    end
  	  end
  	else
      result = -1
   	end
  else
    result = -1
  end
endfunction
