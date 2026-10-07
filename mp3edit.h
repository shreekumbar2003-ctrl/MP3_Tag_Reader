#ifndef EDIT_H
#define EDIT_H

#include<stdio.h>
#include "types.h"

typedef struct _EditInfo
{
    char *mp3_fname;
    char *edit_option;
    char *new_data;

    FILE *fptr_src;
    FILE *fptr_dest;
}EditInfo;

/* Validate edit arguments */
Status read_and_validate_edit(int argc,char *argv[],EditInfo *editInfo);

/* Open source and destination files */
Status open_edit_files(EditInfo *editInfo);

/* perform editing */
Status do_edit(EditInfo *editInfo);

#endif
