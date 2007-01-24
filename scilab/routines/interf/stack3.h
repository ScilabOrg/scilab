#ifndef STACK3_H 
#define STACK3_H 

extern int C2F(readmat)  __PARAMS((char *name__, integer *m, integer *n, double *scimat, unsigned long name_len));
extern int C2F(creadmat)  __PARAMS((char *name__, integer *m, integer *n, double *scimat, unsigned long name_len));
extern int C2F(creadcmat)  __PARAMS((char *name__, integer *m, integer *n, double *scimat, unsigned long name_len));
extern int C2F(creadsmat)  __PARAMS((char *name__, integer *m, integer *n, double *scimat, unsigned long name_len));
extern int C2F(cwritemat)  __PARAMS((char *name__, integer *m, integer *n, double *mat, unsigned long name_len));
extern int C2F(cwritecmat)  __PARAMS((char *name__, integer *m, integer *n, double *mat, unsigned long name_len));
extern int C2F(readchain)  __PARAMS((char *name__, integer *itslen, char *chai, unsigned long name_len, unsigned long chai_len));
extern int C2F(creadchain)  __PARAMS((char *name__, integer *itslen, char *chai, unsigned long name_len, unsigned long chai_len));
extern int C2F(creadchains)  __PARAMS((char *name__, integer *ir, integer *ic, integer *itslen, char *chai, unsigned long name_len, unsigned long chai_len));
extern int C2F(cwritechain)  __PARAMS((char *name__, integer *m, char *chai, unsigned long name_len, unsigned long chai_len));
extern int C2F(matptr)  __PARAMS((char *name__, integer *m, integer *n, integer *lp, unsigned long name_len));
extern int C2F(cmatptr)  __PARAMS((char *name__, integer *m, integer *n, integer *lp, unsigned long name_len));
extern int C2F(cmatcptr)  __PARAMS((char *name__, integer *m, integer *n, integer *lp, unsigned long name_len));
extern int C2F(cmatsptr)  __PARAMS((char *name__, integer *m, integer *n, integer *ix, integer *j, integer *lp, integer *nlr, unsigned long name_len));

extern int C2F(creadbmat)(char *namex, integer *m, integer *n, int *scimat, unsigned long name_len);
extern int C2F(cwritebmat)(char *namex, integer *m, integer *n, int *mat, unsigned long name_len);
extern int C2F(cmatbptr)(char *namex, integer *m,integer *n,integer *lp, unsigned long name_len);
extern int getlengthchain(char *namex);

extern int C2F(str2name)  __PARAMS((char *name__, integer *id, unsigned long name_len));
extern int C2F(objptr)  __PARAMS((char *name__, integer *lp, integer *fin, unsigned long name_len));
extern int C2F(putvar) __PARAMS((int *number, char *namex, unsigned long name_len ));

#endif 


