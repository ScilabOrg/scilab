#ifndef __SCICOS_BLOCK_H__
#define __SCICOS_BLOCK_H__

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

typedef void (*voidg)();

#if WIN32
#define NULL    0
#endif


/* scicos_block structure definition */
typedef struct {
  int nevprt;
  voidg funpt ;
  int type;
  int scsptr;
  int nz;
  double *z;
  int nx;
  double *x;
  double *xd;
  double *res;
  int nin;
  int *insz;
  void **inptr;
  int nout;
  int *outsz;
  void **outptr;
  int nevout;
  double *evout;
  int nrpar;
  double *rpar;
  int nipar;
  int *ipar;
  int ng;
  double *g;
  int ztyp;
  int *jroot;
  char *label;
  void **work;
  int nmode;
  int *mode;
} scicos_block;

/* utility function for block declaration */
void do_cold_restart();
int get_phase_simulation();
double get_scicos_time();
int get_block_number();
void set_block_error(int);
void set_pointer_xproperty(int* pointer);
void * scicos_malloc(size_t );
void scicos_free(void *p);
double Get_Jacobian_parameter(void);
double Get_Scicos_SQUR(void);
void Set_Jacobian_flag(int flag);

#ifndef WIN32
	#define max(a,b) ((a) >= (b) ? (a) : (b))
	#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif


extern int s_copy();
extern int s_cmp();

 /* scicos_block macros definition :
  *
  * 1  - GetNin(blk)
  * 2  - GetInPortPtrs(blk,x)
  * 3  - GetNout(blk)
  * 4  - GetOutPortPtrs(blk,x)
  * 5  - GetInPortRows(blk,x)
  * 6  - GetInPortCols(blk,x)
  * 7  - GetInPortSize(blk,x,y)
  * 8  - GetInType(blk,x)
  * 9  - GetOutPortRows(blk,x)
  * 10 - GetOutPortCols(blk,x)
  * 11 - GetOutPortSize(blk,x,y)
  * 12 - GetOutType(blk,x)
  * 13 - GetRealInPortPtrs(blk,x)
  * 14 - GetImagInPortPtrs(blk,x)
  * 15 - GetRealOutPortPtrs(blk,x)
  * 16 - GetImagOutPortPtrs(blk,x)
  * 17 - Getint8InPortPtrs(blk,x)
  * 18 - Getint16InPortPtrs(blk,x)
  * 19 - Getint32InPortPtrs(blk,x)
  * 20 - Getuint8InPortPtrs(blk,x)
  * 21 - Getuint16InPortPtrs(blk,x)
  * 22 - Getuint32InPortPtrs(blk,x)
  * 23 - Getint8OutPortPtrs(blk,x)
  * 24 - Getint16OutPortPtrs(blk,x)
  * 25 - Getint32OutPortPtrs(blk,x)
  * 26 - Getuint8OutPortPtrs(blk,x)
  * 27 - Getuint16OutPortPtrs(blk,x)
  * 28 - Getuint32OutPortPtrs(blk,x)
  * 29 - free
  *
  * 30 - GetNipar(blk)
  * 31 - GetIparPtrs(blk)
  * 32 - GetNrpar(blk)
  * 33 - GetRparPtrs(blk)
  *
  * 34 - GetWorkPtrs(blk)
  * 35 - GetNdstate(blk)
  * 36 - GetDsatePtrs(blk)
  *
  */

/* Define integer type and integer pointers */
#define int8  char
#define int16 short
#define int32 long
#define uint8  unsigned char
#define uint16 unsigned short
#define uint32 unsigned long
#define int8Ptrs  char *
#define int16Ptrs short *
#define int32Ptrs long *
#define uint8Ptrs  unsigned char *
#define uint16Ptrs unsigned short *
#define uint32Ptrs unsigned long *

/**
   \brief Get Number of Regular Input Port
*/
#define GetNin(blk) (blk->nin)

/**
   \brief Get Regular Input Port Pointer of port number x
*/
#define GetInPortPtrs(blk,x) ((((x)>0)&((x)<=(blk->nin))) ? (blk->inptr[x-1]) : NULL)

/**
   \brief Get Number of Regular Output Port
*/
#define GetNout(blk) (blk->nout)

/**
   \brief Get Regular Output Port Pointer of port number x
*/
#define GetOutPortPtrs(blk,x) ((((x)>0)&((x)<=(blk->nout))) ? (blk->outptr[x-1]) : NULL)

/**
   \brief Get number of Rows (first dimension) of Regular Input Port number x
*/
#define GetInPortRows(blk,x) ((((x)>0)&((x)<=(blk->nin))) ? (blk->insz[x-1]) : 0)

/**
   \brief Get number of Columns (second dimension) of  Regular Input Port number x
*/
#define GetInPortCols(blk,x) ((((x)>0)&((x)<=(blk->nin))) ? (blk->insz[blk->nin+(x-1)]) : 0)

/**
   \brief Get Regular Input Port Size
*/
/*  usage :
 *   GetInPortSize(blk,x,1) : get first dimension of input port number x
 *   GetInPortSize(blk,x,2) : get second dimension of input port number x
 */
#define GetInPortSize(blk,x,y) ((((x)>0)&((x)<=(blk->nin))) ? \
              ((((y)>0)&((y)<=2)) ? (blk->insz[(y-1)*blk->nin+(x-1)]) : 0) : 0)

/**
   \brief Get Type of Input Port
*/
#define GetInType(blk,x) ((((x)>0)&((x)<=(blk->nin))) ? (blk->insz[2*(blk->nin)+(x-1)]) : 0)

/**
   \brief Get number of Rows (first dimension) of Regular Output Port number x
*/
#define GetOutPortRows(blk,x) ((((x)>0)&((x)<=(blk->nout))) ? (blk->outsz[x-1]) : 0)

/**
   \brief Get number of Columns (second dimension) of Regular Output Port number x
*/
#define GetOutPortCols(blk,x) ((((x)>0)&((x)<=(blk->nout))) ? (blk->outsz[blk->nout+(x-1)]) : 0)

/**
   \brief Get Regular Output Port Size
*/
/*  usage :
 *   GetOutPortSize(blk,x,1) : get first dimension of output port number x
 *   GetOutPortSize(blk,x,2) : get second dimension of output port number x
 */
#define GetOutPortSize(blk,x,y) ((((x)>0)&((x)<=(blk->nout))) ? \
              ((((y)>0)&((y)<=2)) ? (blk->outsz[(y-1)*blk->nout+(x-1)]) : 0) : 0)

/**
   \brief Get Type of Output Port
*/
#define GetOutType(blk,x) ((((x)>0)&((x)<=(blk->nout))) ? (blk->outsz[2*(blk->nout)+(x-1)]) : 0)

/**
   \brief Get Pointer of Real Part of Regular Input Port
*/
#define GetRealInPortPtrs(blk,x) (double *) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of Imaginary Part of Regular Input Port
*/
#define GetImagInPortPtrs(blk,x) (((x)>0)&((x)<=(blk->nin)) ? \
              (double *) ((double *)blk->inptr[x-1]+((blk->insz[(x-1)])*(blk->insz[blk->nin+(x-1)]))) : NULL)

/**
   \brief Get Pointer of Real Part of Regular Output Port
*/
#define GetRealOutPortPtrs(blk,x) (double *) GetOutPortPtrs(blk,x)

/**
   \brief Get Pointer of Imaginary Part of Regular Output Port
*/
#define GetImagOutPortPtrs(blk,x) (((x)>0)&((x)<=(blk->nout)) ? \
              (double *) ((double *)blk->outptr[x-1]+((blk->outsz[(x-1)])*(blk->outsz[blk->nout+(x-1)]))) : NULL)

/**
   \brief Get Pointer of int8 typed Regular Input Port
*/
#define Getint8InPortPtrs(blk,x) (int8Ptrs) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of int16 typed Regular Input Port
*/
#define Getint16InPortPtrs(blk,x) (int16Ptrs) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of int32 typed Regular Input Port
*/
#define Getint32InPortPtrs(blk,x) (int32Ptrs) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of uint8 typed Regular Input Port
*/
#define Getuint8InPortPtrs(blk,x) (uint8Ptrs) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of uint16 typed Regular Input Port
*/
#define Getuint16InPortPtrs(blk,x) (uint16Ptrs) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of uint32 typed Regular Input Port
*/
#define Getuint32InPortPtrs(blk,x) (uint32Ptrs) GetInPortPtrs(blk,x)

/**
   \brief Get Pointer of int8 typed Regular Output Port
*/
#define Getint8OutPortPtrs(blk,x) (int8Ptrs) GetOutPortPtrs(blk,x)

/**
   \brief Get Pointer of int16 typed Regular Output Port
*/
#define Getint16OutPortPtrs(blk,x) (int16Ptrs) GetOutPortPtrs(blk,x)

/**
   \brief Get Pointer of int32 typed Regular Output Port
*/
#define Getint32OutPortPtrs(blk,x) (int32Ptrs) GetOutPortPtrs(blk,x)

/**
   \brief Get Pointer of uint8 typed Regular Output Port
*/
#define Getuint8OutPortPtrs(blk,x) (uint8Ptrs) GetOutPortPtrs(blk,x)

/**
   \brief Get Pointer of uint16 typed Regular Output Port
*/
#define Getuint16OutPortPtrs(blk,x) (uint16Ptrs) GetOutPortPtrs(blk,x)

/**
   \brief Get Pointer of uint32 typed Regular Output Port
*/
#define Getuint32OutPortPtrs(blk,x) (uint32Ptrs) GetOutPortPtrs(blk,x)

/**
   \brief Get Number of Integer Parameters
*/
#define GetNipar(blk) (blk->nipar)

/**
   \brief Get Pointer of Integer Parameters
*/
#define GetIparPtrs(blk) (blk->ipar)

/**
   \brief Get Number Real Parameters
*/
#define GetNrpar(blk) (blk->nrpar)

/**
   \brief Get Pointer of Real Parameters
*/
#define GetRparPtrs(blk) (blk->rpar)

/**
   \brief Get Pointer of Work
*/
#define GetWorkPtrs(blk) (*(blk->work))

/**
   \brief Get Number of Discrete state
*/
#define GetNdstate(blk) (blk->nz)

/**
   \brief Get Pointer of Discrete state
*/
#define GetDstate(blk) (blk->z)

/**
   \brief Get Nevprt of the block
*/
#define GetNev(blk) (blk->nevprt)

#endif /* __SCICOS_BLOCK_H__ */
