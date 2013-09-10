%module mpi

%{
#include <mpi.h>
    %}

%include <typemaps.i>
%include <matrix.i>

%scilabconst(1);
%inline %{

MPI_Op MPI_Max() {
  	return MPI_MAX;
}
MPI_Op MPI_Min() {
	return MPI_MIN;
}
MPI_Op MPI_Sum() {
	return MPI_SUM;
}
MPI_Op MPI_Prod() {
	return MPI_PROD;
}
MPI_Op MPI_Land() {
	return MPI_LAND;
}
MPI_Op MPI_Band() {
	return MPI_BAND;
}
MPI_Op MPI_Lor() {
	return MPI_LOR;
}
MPI_Op MPI_Bor() {
	return MPI_BOR;
}
MPI_Op MPI_LXor() {
	return MPI_LXOR;
}
MPI_Op MPI_BXor() {
	return MPI_BXOR;
}
MPI_Op MPI_MaxLoc() {
	return MPI_MAXLOC;
}
MPI_Op MPI_MinLoc() {
	return MPI_MINLOC;
}
MPI_Op MPI_Replace() {
	return MPI_REPLACE;
}

%}
