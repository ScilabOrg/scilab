mode(-1);
lines(0);
originaldir = pwd();
builddir = get_absolute_file_path("builder.sce");
cd(builddir);
ilib_verbose(0);
ilib_name = "mpilib";
libs = [];
ldflags = "";
cflags = ["-g -I" + builddir];
files = "src/swig/mpi_wrap.c";
table = ["SWIG_Init","SWIG_Init";"MPI_Max","_wrap_MPI_Max";"MPI_Min","_wrap_MPI_Min";"MPI_Sum","_wrap_MPI_Sum";"MPI_Prod","_wrap_MPI_Prod";"MPI_Land","_wrap_MPI_Land";"MPI_Band","_wrap_MPI_Band";"MPI_Lor","_wrap_MPI_Lor";"MPI_Bor","_wrap_MPI_Bor";];

table = [table;"MPI_LXor","_wrap_MPI_LXor";"MPI_BXor","_wrap_MPI_BXor";"MPI_MaxLoc","_wrap_MPI_MaxLoc";"MPI_MinLoc","_wrap_MPI_MinLoc";"MPI_Replace","_wrap_MPI_Replace";];
ret = 1;
if ~isempty(table) then
    ilib_build(ilib_name, table, files, libs, [], ldflags, cflags);
    libfilename = "lib" + ilib_name + getdynlibext();
    if isfile(libfilename) & isfile("loader.sce") then
        ret = 0;
    end
end
cd(originaldir);
exit(ret)