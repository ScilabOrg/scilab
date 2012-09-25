/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution. The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#ifndef __ERROR_STACKAPI__
#define __ERROR_STACKAPI__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_error.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

 /*api list error*/

 /*common error 1 - 99*/
#define STACKAPI_ERROR_INVALID_POINTER                           1
#define STACKAPI_ERROR_INVALID_TYPE                              2
#define STACKAPI_ERROR_NOT_MATRIX_TYPE                           3
#define STACKAPI_ERROR_GET_PROCESSMODE                           6
#define STACKAPI_ERROR_INVALID_POSITION                          7
#define STACKAPI_ERROR_INVALID_COMPLEXITY                        8
#define STACKAPI_ERROR_GET_DIMFROMVAR                            9
#define STACKAPI_ERROR_CHECKRHS                                  10
#define STACKAPI_ERROR_CHECKLHS                                  11

#define STACKAPI_ERROR_INVALID_NAME                              50
#define STACKAPI_ERROR_NAMED_TYPE                                51
#define STACKAPI_ERROR_NAMED_UNDEFINED_VAR                       52
#define STACKAPI_ERROR_NAMED_VARDIM                              55
#define STACKAPI_ERROR_GET_NAMED_DIMFROMVAR                      59
#define STACKAPI_ERROR_IS_ROW_VECTOR                             60
#define STACKAPI_ERROR_IS_COLUMN_VECTOR                          61
#define STACKAPI_ERROR_IS_VECTOR                                 62
#define STACKAPI_ERROR_IS_SCALAR                                 63
#define STACKAPI_ERROR_IS_SQUARE                                 64
#define STACKAPI_ERROR_CHECK_VAR_DIMENSION                       65
#define STACKAPI_ERROR_CREATE_EMPTY_MATRIX                       66
#define STACKAPI_ERROR_CREATE_NAMED_EMPTY_MATRIX                 67
#define STACKAPI_ERROR_IS_NAMED_ROW_VECTOR                       68
#define STACKAPI_ERROR_IS_NAMED_COLUMN_VECTOR                    69
#define STACKAPI_ERROR_IS_NAMED_SCALAR                           70
#define STACKAPI_ERROR_IS_NAMED_SQUARE                           71
#define STACKAPI_ERROR_CHECK_NAMED_VAR_DIMENSION                 72

 /*double errors 101 - 199*/
#define STACKAPI_ERROR_GET_DOUBLE                                101
#define STACKAPI_ERROR_GET_ZDOUBLE                               102
#define STACKAPI_ERROR_ALLOC_DOUBLE                              103
#define STACKAPI_ERROR_ALLOC_COMPLEX_DOUBLE                      104
#define STACKAPI_ERROR_CREATE_DOUBLE                             105
#define STACKAPI_ERROR_CREATE_COMPLEX_DOUBLE                     106
#define STACKAPI_ERROR_CREATE_ZDOUBLE                            107
#define STACKAPI_ERROR_READ_NAMED_DOUBLE                         108
#define STACKAPI_ERROR_GET_SCALAR_DOUBLE                         109
#define STACKAPI_ERROR_CREATE_SCALAR_DOUBLE                      110
#define STACKAPI_ERROR_CREATE_NAMED_SCALAR                       111
#define STACKAPI_ERROR_GET_NAMED_SCALAR_DOUBLE                   112

 /*poly errors 201 - 299*/
#define STACKAPI_ERROR_GET_POLY_VARNAME                          201
#define STACKAPI_ERROR_GET_POLY                                  202
#define STACKAPI_ERROR_CREATE_POLY                               203
#define STACKAPI_ERROR_TOO_LONG_VAR                              204
#define STACKAPI_ERROR_CREATE_NAMED_POLY                         207
#define STACKAPI_ERROR_READ_NAMED_POLY                           208
#define STACKAPI_ERROR_GET_ALLOC_SINGLE_POLY                     209
#define STACKAPI_ERROR_GET_ALLOC_NAMED_SINGLE_POLY               210
#define STACKAPI_ERROR_GET_ALLOC_MATRIX_POLY                     211
#define STACKAPI_ERROR_GET_ALLOC_NAMED_MATRIX_POLY               212

 /*boolean errors 401 - 499*/
#define STACKAPI_ERROR_GET_BOOLEAN                               401
#define STACKAPI_ERROR_CREATE_BOOLEAN                            402
#define STACKAPI_ERROR_CREATE_NAMED_BOOLEAN                      403
#define STACKAPI_ERROR_READ_BOOLEAN                              404
#define STACKAPI_ERROR_CREATE_SCALAR_BOOLEAN                     405
#define STACKAPI_ERROR_CREATE_NAMED_SCALAR_BOOLEAN               406

 /*sparse errors 501 - 599*/
#define STACKAPI_ERROR_GET_SPARSE                                501
#define STACKAPI_ERROR_ALLOC_SPARSE                              502
#define STACKAPI_ERROR_CREATE_SPARSE                             504
#define STACKAPI_ERROR_CREATE_NAMED_SPARSE                       505
#define STACKAPI_ERROR_READ_NAMED_SPARSE                         506
#define STACKAPI_ERROR_GET_ALLOC_SPARSE                          507
#define STACKAPI_ERROR_GET_NAMED_ALLOC_SPARSE                    508

 /*boolean sparse errors 601 - 699*/
#define STACKAPI_ERROR_GET_BOOLEAN_SPARSE                        601
#define STACKAPI_ERROR_ALLOC_BOOLEAN_SPARSE                      602
#define STACKAPI_ERROR_FILL_BOOLEAN_SPARSE                       603
#define STACKAPI_ERROR_CREATE_BOOLEAN_SPARSE                     604
#define STACKAPI_ERROR_CREATE_NAMED_BOOLEAN_SPARSE               605
#define STACKAPI_ERROR_READ_NAMED_BOOLEAN_SPARSE                 606
#define STACKAPI_ERROR_GET_SCALAR_BOOLEAN                        607
#define STACKAPI_ERROR_GET_NAMED_SCALAR_BOOLEAN                  608
#define STACKAPI_ERROR_GET_ALLOC_BOOLEAN_SPARSE                  611
#define STACKAPI_ERROR_GET_NAMED_ALLOC_BOOLEAN_SPARSE            612

 /*matlab sparse errors 701 - 799*/

 /*ints errors 801 - 899*/
#define STACKAPI_ERROR_GET_INT                                   802
#define STACKAPI_ERROR_CREATE_INT                                803
#define STACKAPI_ERROR_ALLOC_INT                                 804
#define STACKAPI_ERROR_GET_NAMED_INT_PRECISION                   805
#define STACKAPI_ERROR_READ_NAMED_INT                            806
#define STACKAPI_ERROR_GET_SCALAR_INTEGER                        807
#define STACKAPI_ERROR_GET_NAMED_SCALAR_INTEGER                  808
#define STACKAPI_ERROR_CREATE_SCALAR_INT                         809
#define STACKAPI_ERROR_CREATE_NAMED_SCALAR_INT                   810

 /*strings errors 1001 - 1099*/
#define STACKAPI_ERROR_GET_STRING                                1001
#define STACKAPI_ERROR_INVALID_SUBSTRING_POINTER                 1002
#define STACKAPI_ERROR_CREATE_STRING                             1003
#define STACKAPI_ERROR_CREATE_NAMED_STRING                       1005
#define STACKAPI_ERROR_READ_NAMED_STRING                         1006
#define STACKAPI_ERROR_GET_WIDE_STRING                           1007
#define STACKAPI_ERROR_CREATE_WIDE_STRING                        1008
#define STACKAPI_ERROR_CREATE_NAMED_WIDE_STRING                  1009
#define STACKAPI_ERROR_READ_NAMED_WIDE_STRING                    1010
#define STACKAPI_ERROR_GET_ALLOC_SINGLE_STRING                   1011
#define STACKAPI_ERROR_GET_ALLOC_SINGLE_WIDE_STRING              1012
#define STACKAPI_ERROR_GET_ALLOC_STRING_MATRIX                   1013
#define STACKAPI_ERROR_GET_ALLOC_WIDE_STRING_MATRIX              1014
#define STACKAPI_ERROR_GET_ALLOC_NAMED_SINGLE_STRING             1015
#define STACKAPI_ERROR_GET_ALLOC_NAMED_SINGLE_WIDE_STRING        1016
#define STACKAPI_ERROR_GET_ALLOC_NAMED_STRING_MATRIX             1017
#define STACKAPI_ERROR_GET_ALLOC_NAMED_WIDE_STRING_MATRIX        1018
#define STACKAPI_ERROR_CREATE_SINGLE_STRING                      1019
#define STACKAPI_ERROR_CREATE_SINGLE_WIDE_STRING                 1020
#define STACKAPI_ERROR_CREATE_NAMED_SINGLE_STRING                1021
#define STACKAPI_ERROR_CREATE_NAMED_SINGLE_WIDE_STRING           1022
 
 /*list errors 1501 - 1599*/
#define STACKAPI_ERROR_LIST_ITEM_NUMBER                          1501
#define STACKAPI_ERROR_INVALID_LIST_TYPE                         1502
#define STACKAPI_ERROR_GET_ITEM_ADDRESS                          1503
#define STACKAPI_ERROR_CREATE_NAMED_LIST                         1504
#define STACKAPI_ERROR_CREATE_LIST                               1505
#define STACKAPI_ERROR_READ_NAMED_LIST                           1506
#define STACKAPI_ERROR_GET_LIST_IN_LIST                          1507
#define STACKAPI_ERROR_GET_LIST_IN_NAMED_LIST                    1508
#define STACKAPI_ERROR_CREATE_LIST_IN_LIST                       1509
#define STACKAPI_ERROR_CREATE_LIST_IN_NAMED_LIST                 1510
#define STACKAPI_ERROR_ITEM_LIST_NUMBER                          1511
#define STACKAPI_ERROR_NON_ORDERED_INSERTION                     1512
#define STACKAPI_ERROR_ALLOC_IN_LIST                             1513
#define STACKAPI_ERROR_CREATE_VOID_IN_LIST                       1514
 /*double in list*/
#define STACKAPI_ERROR_GET_DOUBLE_IN_LIST                        1520
#define STACKAPI_ERROR_ALLOC_DOUBLE_IN_LIST                      1521
#define STACKAPI_ERROR_CREATE_ZDOUBLE_IN_LIST                    1523
#define STACKAPI_ERROR_CREATE_DOUBLE_IN_LIST                     1524
#define STACKAPI_ERROR_CREATE_ZDOUBLE_IN_NAMED_LIST              1525
#define STACKAPI_ERROR_CREATE_DOUBLE_IN_NAMED_LIST               1526
#define STACKAPI_ERROR_READ_DOUBLE_IN_NAMED_LIST                 1527

 /*string in list*/
#define STACKAPI_ERROR_GET_STRING_IN_LIST                        1530
#define STACKAPI_ERROR_FILL_STRING_IN_LIST                       1531
#define STACKAPI_ERROR_CREATE_STRING_IN_LIST                     1532
#define STACKAPI_ERROR_CREATE_STRING_IN_NAMED_LIST               1534
#define STACKAPI_ERROR_READ_STRING_IN_NAMED_LIST                 1535

 /*boolean in list*/
#define STACKAPI_ERROR_GET_BOOLEAN_IN_LIST                       1540
#define STACKAPI_ERROR_CREATE_BOOLEAN_IN_LIST                    1541
#define STACKAPI_ERROR_ALLOC_BOOLEAN_IN_LIST                     1542
#define STACKAPI_ERROR_FILL_BOOLEAN_IN_LIST                      1543
#define STACKAPI_ERROR_CREATE_BOOLEAN_IN_NAMED_LIST              1544
#define STACKAPI_ERROR_READ_BOOLEAN_IN_NAMED_LIST                1545

 /*poly in list*/
#define STACKAPI_ERROR_GET_POLY_IN_LIST                          1550
#define STACKAPI_ERROR_CREATE_POLY_IN_LIST                       1551
#define STACKAPI_ERROR_FILL_POLY_IN_LIST                         1552
#define STACKAPI_ERROR_CREATE_POLY_IN_NAMED_LIST                 1553
#define STACKAPI_ERROR_READ_POLY_IN_NAMED_LIST                   1554

 /*int in list*/
#define STACKAPI_ERROR_FILL_INT_IN_LIST                          1560
#define STACKAPI_ERROR_ALLOC_INT_IN_LIST                         1561
#define STACKAPI_ERROR_CREATE_INT_IN_LIST                        1562
#define STACKAPI_ERROR_GET_INT_IN_LIST                           1563
#define STACKAPI_ERROR_CREATE_INT_IN_NAMED_LIST                  1564
#define STACKAPI_ERROR_READ_INT_IN_NAMED_LIST                    1565

 /*sparse in list*/
#define STACKAPI_ERROR_FILL_SPARSE_IN_LIST                       1570
#define STACKAPI_ERROR_CREATE_SPARSE_IN_LIST                     1571
#define STACKAPI_ERROR_CREATE_SPARSE_IN_NAMED_LIST               1572
#define STACKAPI_ERROR_GET_SPARSE_IN_LIST                        1573
#define STACKAPI_ERROR_READ_SPARSE_IN_NAMED_LIST                 1574

 /*boolean sparse in list*/
#define STACKAPI_ERROR_FILL_BOOLEAN_SPARSE_IN_LIST               1580
#define STACKAPI_ERROR_CREATE_BOOLEAN_SPARSE_IN_LIST             1581
#define STACKAPI_ERROR_CREATE_BOOLEAN_SPARSE_IN_NAMED_LIST       1582
#define STACKAPI_ERROR_GET_BOOLEAN_SPARSE_IN_LIST                1583
#define STACKAPI_ERROR_READ_BOOLEAN_SPARSE_IN_NAMED_LIST         1584

 /*pointer in list*/
#define STACKAPI_ERROR_GET_POINTER_IN_LIST                       1590
#define STACKAPI_ERROR_CREATE_POINTER_IN_NAMED_LIST              1591
#define STACKAPI_ERROR_READ_POINTER_IN_NAMED_LIST                1592
#define STACKAPI_ERROR_CREATE_POINTER_IN_LIST                    1593

/*pointer errors 12801 - 12899*/
#define STACKAPI_ERROR_GET_POINTER                               12801
#define STACKAPI_ERROR_ALLOC_POINTER                             12803
#define STACKAPI_ERROR_CREATE_POINTER                            12804
#define STACKAPI_ERROR_CREATE_NAMED_POINTER                      12805
#define STACKAPI_ERROR_READ_POINTER                              12806

/* error functions*/
int stackapi_addStackSizeError(SciErr* _psciErr, char* _pstCaller, int iNeeded);
int stackapi_addErrorMessage(SciErr* _psciErr, int _iErr, const char* _pstMsg, ...);
int stackapi_printError(SciErr* _psciErr, int _iLastMsg);
char* stackapi_getErrorMessage(SciErr stackapi__sciErr);

#ifdef __cplusplus
}
#endif
#endif /* __ERROR_STACKAPI__ */
