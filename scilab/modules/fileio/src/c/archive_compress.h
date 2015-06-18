#ifndef __ARCHIVE_COMPRESS_H__
#define __ARCHIVE_COMPRESS_H__

#include<sys/types.h>
#include<sys/stat.h>
#include<archive.h>
#include<archive_entry.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int archive_compress(char *archive_name,char *file_list, char *arg_list);

#endif
