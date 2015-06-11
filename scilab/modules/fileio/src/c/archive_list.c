#include<archive.h>
#include<stdio.h>
#include<stdlib.h>
#include<archive_entry.h>
#include<string.h>

char* archive_list(char *filename)
{
struct archive *a;
struct archive_entry *entry;
int r;
char delimiter[100];
strcpy(delimiter," ;");
a = archive_read_new();
archive_read_support_filter_all(a);
archive_read_support_format_all(a);
r = archive_read_open_filename(a, filename, 10240); 
if (r != ARCHIVE_OK)
{
    char error_temp[100];
    strcpy(error_temp, "ERROR;1");
    return error_temp;
}
char *temp;
temp = (char*)malloc(1);
temp[0] = 0;
while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
  char *temp2;
/*  temp2 = (char*)malloc(sizeof(temp)+200);
  strcpy(temp2,temp);
  strcat(temp2,delimiter);
  strcat(temp2,archive_entry_pathname(entry));
  temp = temp2;*/
  temp2 = temp;
  temp = (char *)malloc(sizeof(temp) + 200);
  temp[0] = 0;
  sprintf(temp, "%s;%s", temp2, archive_entry_pathname(entry));
  archive_read_data_skip(a);  
}
r = archive_read_free(a);  
if (r != ARCHIVE_OK)
{
  char error_temp[100];
  strcpy(error_temp, "ERROR;2");
  return error_temp;
}
return temp;
}
