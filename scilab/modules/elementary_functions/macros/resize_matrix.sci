// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 2007 - Jean-Baptiste SILVY
// Copyright (C) DIGITEO - 2011-2012 - Allan CORNET
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier: added hypermatrix support
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function resMat = resize_matrix(mat, nbRow, nbCol, nbCol2, typeOfMat)

    // Author Jean-Baptiste Silvy
    // Create a matrix of size nbRow x nbCol x nbCol2 whose elements (i,j,k) are mat(i,j,k)
    // if (i,j,k) is in the range of the matrix. Otherwise elements (i,j,k) are 0 for real
    // or integer matrix, %f for boolean matrix and an empty string for string matrix.
    // The type of the matrix may be modified by specifying the typeOfMat variable

    [lhs, rhs] = argn(0);
    if rhs <> 3 & rhs <> 4 & rhs <> 5 then
        error(msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"),"resize_matrix", 3, 4));
    end

    if type(mat) <> 1 & type(mat) <> 4 & type(mat) <> 8 & type(mat) <> 10 & type(mat) <> 17 then
        msg = _("%s: Wrong type for input argument #%d: %s, %s, %s, %s, %s or %s matrix expected.\n");
        error(msprintf(msg, "resize_matrix", 1, "Real", "Complex", "Integer", "Boolean", "String", "Hypermatrix"));
    end

    if rhs < 4 then
        nbCol2 = 1;
        minCol2 = 1;
    elseif rhs == 4 & typeof(nbCol2) == "string" then
        typeOfMat = nbCol2;
        nbCol2 = 1;
        minCol2 = 1;
        rhs = 5;
    end

    if ((nbRow <= 0) | (nbCol <= 0) | (nbCol2 <= 0)) then
        resMat = [];
        return
    end

    matSize = size(mat);
    minRow = min(nbRow, matSize(1));
    minCol = min(nbCol, matSize(2));
    if size(matSize, "c") > 3 then
        error(msprintf(_("%s: Wrong size for input argument #%d: Matrix or 3D hypermatrix expected.\n"),"resize_matrix", 1));
    elseif size(matSize, "c") == 3 then
        minCol2 = min(nbCol2, matSize(3)); // Retrieve third dimension if input is a hypermatrix.
    else
        minCol2 = 1;
    end

    if rhs < 5 then
        typeOfMat = typeof(mat);
    end

    z = zeros(nbRow, nbCol, nbCol2);

    select typeOfMat
    case "constant" then
        // real matrix
        resMat = z;
    case "boolean" then
        // boolean matrix
        // intialized it to false
        resMat = (z == 1);
        if typeof(mat) == "boolean" then
            resMat(1:minRow, 1:minCol, 1:minCol2) = (mat(1:minRow, 1:minCol, 1:minCol2) == %t);
        else
            resMat(1:minRow, 1:minCol, 1:minCol2) = (mat(1:minRow, 1:minCol, 1:minCol2) == 1);
        end
        return
    case "int8" then
        mat = int8(mat);
        resMat = int8(z);
    case "int16" then
        mat = int16(mat);
        resMat = int16(z);
    case "int32" then
        mat = int32(mat);
        resMat = int32(z);
    case "uint8" then
        mat = uint8(mat);
        resMat = uint8(z);
    case "uint16" then
        mat = uint16(mat);
        resMat = uint16(z);
    case "uint32" then
        mat = uint32(mat);
        resMat = uint32(z);
    case "string" then
        // string matrix
        mat = string(mat);
        resMatTemp = emptystr(nbRow, nbCol);
        for i=1:nbCol2
            resMat(:, :, i) = resMatTemp; // emptystr() does not handle hypermatrices yet
        end
    case "hypermat" then
        typeOfMat = typeof(mat(1, 1)); // Retrieve the type of the hypermatrix elements.
        select typeOfMat
        case "constant" then
            // real matrix
            resMat = z;
        case "boolean" then
            // boolean matrix
            // intialized it to false
            resMat = (z == 1);
            if typeof(mat) == "boolean" then
                resMat(1:minRow, 1:minCol, 1:minCol2) = (mat(1:minRow, 1:minCol, 1:minCol2) == %t);
            else
                resMat(1:minRow, 1:minCol, 1:minCol2) = (mat(1:minRow, 1:minCol, 1:minCol2) == 1);
            end
            return
        case "int8" then
            mat = int8(mat);
            resMat = int8(z);
        case "int16" then
            mat = int16(mat);
            resMat = int16(z);
        case "int32" then
            mat = int32(mat);
            resMat = int32(z);
        case "uint8" then
            mat = uint8(mat);
            resMat = uint8(z);
        case "uint16" then
            mat = uint16(mat);
            resMat = uint16(z);
        case "uint32" then
            mat = uint32(mat);
            resMat = uint32(z);
        case "string" then
            // string matrix
            mat = string(mat);
            resMatTemp = emptystr(nbRow, nbCol);
            for i=1:nbCol2
                resMat(:, :, i) = resMatTemp; // emptystr() does not handle hypermatrices yet
            end
        end
    end

    resMat(1:minRow, 1:minCol, 1:minCol2) = mat(1:minRow, 1:minCol, 1:minCol2);

endfunction
