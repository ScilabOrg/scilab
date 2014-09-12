// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vladislav TRUBKIN
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//

// check function
clear "checkValue";
function checkValue(value)
    clear "name";
    name = value;
    //filename = TMPDIR + "/" + "exportImportHdf5.sod";
    filename = "d:/" + "exportImportHdf5New.sod";
    // export to hdf5
    assert_checkequal(export_to_hdf5(filename, "name"), %t);
     // reset ref value
    clear "name";
    assert_checkequal(exists("name"), 0);
    // import from hdf5
    assert_checkequal(import_from_hdf5(filename), %t);
    if type(value) <> 15 then
        assert_checkequal(name, value);
    else
        for i = 1:lstsize(value)
            assert_checkequal(name(i), value(i));
        end
    end
endfunction

//// Empty matrix
checkValue([]);

//// Double
// scalar
checkValue(77);
// vector
checkValue([1, 2, 3, 4, 5]);
checkValue([1; 2; 3; 4; 5]);
// matrix 
checkValue([1, 2, 3; 4, 5, 6]);

//// Double complex
// scalar
checkValue(1 + %i*2);
// vector
checkValue([1 + %i, 2 + %i, 3 + %i*2, 4 + %i*3, 5 + %i*4]);
checkValue([1 + %i; 2 + %i; 3 + %i*2; 4 + %i*3; 5 + %i*4]);
// matrix
checkValue([1 + %i, 2 + %i, 3 + %i*2; 4 + %i*3, 5 + %i*4, 6 + %i]);

//// String
// single
checkValue("Single String");
// vector
checkValue(["a", "b", "c"]);
checkValue(["a"; "b"; "c"]);
// matrix
checkValue(["a", "b", "c"; "d", "e", "f"]);

//// Polynomials
s = poly(0, "s");
// single
checkValue(poly([1, 2], "s", "c"));
// vector
checkValue([s, s^2, 1 + 3*s^2, 1 + 2*s + 4*s^3]);
checkValue([s; s^2; 1 + 3*s^2; 1 + 2*s + 4*s^3]);
// matrix
checkValue([s, s^2; 1 + 3*s^2, 1 + 2*s + 4*s^3]);

//// Boolean
// single
checkValue(%t);
// vector
checkValue([%t, %t, %f, %t, %t]);
checkValue([%t; %t; %f; %t; %t]);
// matrix
checkValue([%t, %t; %f, %f]);

//// Integer
clear "createIntValues";
function intValues = createIntValues(value, flag)
    select flag
        case 8 // int8
            intValues = int8(value);
            break;
        case -8 // uint8
            intValues = uint8(value);
            break;
        case 16
            intValues = int16(value);
            break;
        case -16
            intValues = uint16(value);
            break;
        case 32
            intValues = int32(value);
            break;
        case -32
            intValues = uint32(value);
            break;
    end
endfunction

clear "checkIntValue";
function checkIntValue(refValue, flag)
    for i = 1:6
        checkValue(createIntValues(refValue, flag(i)));
    end
endfunction

// all flags for createIntValues
flag = [8, -8, 16, -16, 32, -32];
// scalar
checkIntValue(5, flag);
// vector
checkIntValue([1, 2, 3, 4, 5], flag);
checkIntValue([1, 2, 3, 4, 5]', flag);
// matrix
checkIntValue([1, 2, 3; 4, 5, 6], flag);

////// Sparse
//checkValue(sparse([1, 2; 4, 5; 3, 10], [1, 2, 3]));
//checkValue(sparse([1, 2; 4, 5; 3, 10], [1 + %i, 2 + 2*%i, 3 + 3*%i]));

//// Boolean sparse
ref = [%F, %F, %T, %F, %F
       %T, %F, %F, %F, %F
       %F, %F, %F, %F, %F
       %F, %F, %F, %F, %T];
checkValue(ref);

//// List
listNew = list();
// double in list
listNew(1) = 111;
listNew(2) = [1, 2, 3];
listNew(3) = [1, 2, 3]';
// string in list
listNew(4) = ["a", "b"; "c", "d"];
// double complex in list
listNew(5) = [complex(1, 2), complex(1, 3); complex(1, 4), complex(1, 5)];
// boolean in list
listNew(6) = [%t, %t, %t; %f, %f, %f];
// integer in list 
valueRef = ones(4, 3);
for i = 7:12
    listNew(i) = createIntValues(valueRef, flag(i - 6))
end
// empty matrix in list
listNew(13) = [];
checkValue(listNew);

//// list in list
//listNew = list();
//listNew(1) = 111;
//listNew(2) = list([1, 2, 3]);
//for i = 3:8
//    listNew(i) = list(createIntValues(valueRef, flag(i - 2)));
//end
//listNew(9) = list(string([1, 2, 3; 4, 5, 6]));
//listNew(10) = list([%f, %f; %t, %t]);
//checkValue(listNew);

// TODO: tlist and mlist in the list

//// Tlist
//lstRef = tlist(["random numbers"; "Name"; "Example"], "Uniform", ones(1 , 2, 3));
//checkValue(lstRef);

//// Mlist
//lstRef = mlist(["V", "name", "value"], ["a", "b"; "c", "d"], [1, 2; 3, 4]);
//checkValue(lstRef);

// Hypermatrix
//refValue = hypermat([2, 3, 2, 2], 1:24);
//checkValue(refValue);

// TODO: undefined and void
