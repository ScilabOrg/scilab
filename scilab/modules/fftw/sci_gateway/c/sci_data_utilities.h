int getArrayOfDouble(void* pvApiCtx, int *piAddr, int *ndims, int **dims, double **Ar, double **Ai);
SciErr allocArrayOfDouble(void* _pvCtx, int _iVar, int ndims, int *dims, double **Ar);
SciErr allocComplexArrayOfDouble(void* _pvCtx, int _iVar, int ndims, int *dims, double **Ar, double **Ai);
SciErr getScalarIntArg(void* _pvCtx, int _iVar, char *fname, int *value);
SciErr getVectorIntArg(void* _pvCtx, int _iVar, char *fname, int *pndims, int **pDim);
BOOL isHyperMatrixMlist(void* _pvCtx, int *piAddressVar);
