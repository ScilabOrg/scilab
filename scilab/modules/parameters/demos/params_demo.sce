// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

lines(0);

printf("\nCreate a plist ''test_list'' containing two fields ''min'' = [0 0 0] and ''max'' = [1 1 1].\n\n");

test_list = init_param();

test_list = add_param(test_list, "min");
test_list = add_param(test_list, "max");

printf("Check fields: \n");
printf("is_param(test_list,''min'') = %f\n",1.0*is_param(test_list, "min"));
printf("is_param(test_list,''max'') = %f\n",1.0*is_param(test_list, "max"));
printf("is_param(test_list,''mex'') = %f\n\n",1.0*is_param(test_list, "mex"));

printf("Set ''min'' = [0 0 0] and ''max'' = [1 1 1].\n");
printf("set_param(test_list, ''min'', [0 0 0]);\n");
printf("set_param(test_list, ''max'', [1 1 1]);\n\n");
test_list = set_param(test_list, "min", [0 0 0]);
test_list = set_param(test_list, "max", [1 1 1]);

printf("Check values: \n");
printf("get_param(test_list, ''min'') = "); disp(get_param(test_list, "min"));
printf("get_param(test_list, ''max'') = "); disp(get_param(test_list, "max"));
printf("get_param(test_list, ''mex'') = "); disp(get_param(test_list, "mex"));

printf("\nRemove the parameter ''min'' from test_list.\n");
printf("\nBefore remove_param: \n");
disp(list_param(test_list));

printf("\ntest_list = remove_param(test_list, ''min'');\n")
test_list = remove_param(test_list, "min");

printf("\nAfter remove_param: \n");
disp(list_param(test_list));

printf("\nCheck removal: \n");
printf("get_param(test_list, ''min'') = ");
disp(get_param(test_list, "min"));

printf("\nCheck type: \n");
printf("typeof(test_list) = %s\n\n", typeof(test_list));

A = []

printf("\nAdd a parameter to the non plist ''A''.\n");
printf("\nBefore add_param: \n");
disp(A);

printf("\ntry [A,err] = add_param(A, ''min''); catch err=%%t; end;\n");
try
    [A,err] = add_param(A, "min");
catch
    err=%t;
end

printf("\nAfter add_param: \n");
disp(A);
printf("\nError = %f. If error <> 0, then the error has been caught.\n", 1*err);

