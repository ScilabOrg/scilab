S="// Copyright (C) 2011 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - INRIA - Allan CORNET
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
"

for f in *; do
#    cat ../../orig.txt $f > $f.tmp
#    mv $f.tmp $f
    
#    sed  -e "s|$S||g" $f
    echo "$S"
    sed -i -e "s|// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab|// =============================================================================\n// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab|" $f
    sed -i -e "s|Scilab ( http://www.scilab.org/ ) - This file is part of Scilab|Scilab ( http://www.scilab.org/ ) - This file is part of Scilab|g" $f

done

