#ifndef MP3VIEW_H
#define MP3VIEW_H

#include<stdio.h>
#include "types.h"


typedef struct _ViewInfo
{
    char *mp3_fname;
    FILE *fptr_mp3;

}ViewInfo;

/* operation type */
OperationType check_operation_type(char *argv);

/* Validation */
Status read_and_validate_view(int argc,char *argv[],ViewInfo *viewInfo);

/* open file */
Status open_mp3_file(ViewInfo *viewInfo);

/* perform the viewing */
Status do_view(ViewInfo *viewInfo);

/* Read tags */
Status read_tags(ViewInfo *viewInfo);

#endif