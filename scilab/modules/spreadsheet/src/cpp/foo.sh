S="// =============================================================================
// Copyright (C) 2011 - INRIA - Allan CORNET
// Copyright (C) 2012 - Allan Cornet - allan.cornet.scilab@gmail.com
// GPL v3 http://www.gnu.org/licenses/gpl.txt
// ============================================================================="

for f in *.tst; do
    cat ../../orig.txt $f > $f.tmp
#    mv $f.tmp $f
    
#    sed  -e "s|$S||g" $f
    perl  -pe 's/$F//g' $f

done

