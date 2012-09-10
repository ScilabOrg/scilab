S="// Copyright (C) 2011 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - INRIA - Allan CORNET
// GPL v3 http://www.gnu.org/licenses/gpl.txt
"

for f in *; do
#    cat ../../orig.txt $f > $f.tmp
#    mv $f.tmp $f
    
#    sed  -e "s|$S||g" $f
    if test "$f" != "foo.sh"; then 
fi
done

