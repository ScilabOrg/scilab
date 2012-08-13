#!/bin/sh
FILES="modules/graphics/sci_gateway/c/sci_move.c"
FILES="modules/gui/sci_gateway/c/sci_x_choice.c"

echo "Converting $FILES"
astyle --pad-header -n --pad-oper --indent-col1-comments --indent-switches --style=linux --indent=spaces=4 -A1  $FILES &> /dev/null
sed -i -e "s|CheckRhs(\(.*\), \(.*\))|nbInputArgument(pvApiCtx, \1, \2)|g" $FILES

sed -i -e "s|CheckLhs(\(.*\), \(.*\))|nbOutputArgument(pvApiCtx, \1, \2)|g" $FILES


sed -i -e "s|PutLhsVar()|ReturnArguments(pvApiCtx)|g" $FILES


#IN="GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);"

getProfileDouble() {
POSITION="$1"
NBROW="$2"
NBCOL="$3"
VAR="$4"
OUT="   //get variable address of the input argument\n    sciErr = getVarAddressFromPosition(pvApiCtx, $POSITION, \&piAddr);\n    if (sciErr.iErr)\n    {\n        printError(\&sciErr, 0);\n        return 0;\n    }\n    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, \&$NBROW, \&$NBCOL, \&$VAR);\n    if (sciErr.iErr)\n    {\n        printError(\&sciErr, 0);\n        return 0;\n    }"

}

createProfileDouble() {
POSITION="$1"
NBROW="$2"
NBCOL="$3"
VAR="$4"
OUT="/* Create the matrix as return of the function */\nsciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + $POSITION, $NBROW, $NBCOL, $VAR);\nfree($VAR); // Data have been copied into Scilab memory\nif (sciErr.iErr)\n{\n    free($VAR); // Make sure everything is cleanup in case of error\n    printError(\&sciErr, 0);\n    return 0;"
}

##########################################

getProfileString() {
POSITION="$1"
NBROW="$2"
NBCOL="$3"
VAR="$4"
SOURCE="stringtemplate.txt"

sed -e "s|@POSITION@|$POSITION|g" -e "s|@NBROW@|$NBROW|g"  -e "s|@NBCOL@|$NBCOL|g" -e "s|@VAR@|$VAR|g" $SOURCE > foo.txt
sed -i -n 'H;${g;s/\n/\\n/g;p}' foo.txt

OUT=$(cat foo.txt)

}

##########################################

INT="GetRhsVar"
RHS=$(grep $INT $FILES|grep MATRIX_OF_DOUBLE_DATATYPE)
if test "$RHS" != ""; then
# Split the values
ARGS=$(echo $RHS|sed -e "s|.*(\(.*\), .*, &\(.*\), &\(.*\), &\(.*\));|\1 \2 \3 \4|")
getProfileDouble $ARGS
sed -i -e "s|$RHS|$OUT|g" $FILES
fi

##########################################

CREATEINT="CreateVarFromPtr"
LHS=$(grep $CREATEINT $FILES|grep MATRIX_OF_DOUBLE_DATATYPE)
if test "$LHS" != ""; then
# Split the values
ARGS=$(echo $LHS|sed -e "s|.*(.* + \(.*\), .*, &\(.*\), &\(.*\), &\(.*\));|\1 \2 \3 \4|")
createProfileDouble $ARGS
sed -i -e "s|$LHS|$OUT|g" $FILES
fi

##########################################

qCREATEINT="GetRhsVar"
LHS="$(grep $CREATEINT $FILES|grep MATRIX_OF_STRING_DATATYPE|head -1)"

if test "$LHS" != ""; then

# Split the values
    ARGS=$(echo $LHS|sed -e "s|.*(\(.*\), .*, &\(.*\), &\(.*\), &\(.*\));|\1 \2 \3 \4|")
getProfileString $ARGS

sed -i -e "s|$LHS|$OUT|g" $FILES

fi


##########################################


sed -i -e "s|LhsVar(\(.*\))|AssignOutputVariable(pvApiCtx, \1)|g" $FILES
sed -i -e "s| Rhs | nbInputArgument(pvApiCtx) |g" $FILES
sed -i -e "s| Lhs | nbOutputArgument(pvApiCtx) |g" $FILES
sed -i -e "s|(VarType(\(.*\)).*==.*sci_matrix)|(isDoubleType(pvApiCtx,TODO:ADDRESS_OF_\1)|g" $FILES
sed -i  -e "s|(VarType(\(.*\)).*==.*sci_strings)|(isStringType(pvApiCtx,TODO:ADDRESS_OF_\1)|g" $FILES


##########################################


astyle --pad-header -n --pad-oper --indent-col1-comments --indent-switches --style=linux --indent=spaces=4 -A1  $FILES &> /dev/null

#echo "$OUT"
#perl -i -pe "s/$RHS/$OUT/g" $FILES
#OUT="foo\naze"
#OUT="   //get variable address of the input argument\n"
#echo "$OUT"
#echo "$RHS"
#echo sed -e "s|GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);|$OUT|g" $FILES
