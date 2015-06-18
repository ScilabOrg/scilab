%module sci_archive_list

%include "../../src/c/archive_list.c"
%include "../../src/c/archive_extract.c"
%include "../../src/c/archive_compress.c"
%include typemaps.i

%inline %{
        extern char* archive_list(char *filename);
        extern int archive_extract(char *filename, int perm_flag);
        extern int archive_compress(char *archive_name,char *file_list, char *arg_list);
        %}
