
#ifndef __SCIMPI
#define __SCIMPI
#include <stdlib.h>
#include <mpi.h>

/* Create some static datastructure to store all the Request references */
MPI_Request *request;
static int REQUEST_MAXSIZE = 10000;


#define debuglevel 10

#endif /* __SCIMPI */
