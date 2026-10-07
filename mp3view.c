#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mp3view.h"
#include "types.h"


/* Check operation type and validate arguments */

Status read_and_validate_view(int argc, char *argv[], ViewInfo *viewInfo)
{
    if (argc < 3)
    {
        printf("Error: Invalid Arguments\n\n");

        printf("USAGE:\n");
        printf("To view: Please pass like: ./a.out -v mp3filename\n");
        printf("To edit: Please pass like: ./a.out -e -t/-a/-A/-m/-y/-c mp3filename\n");

        return 1;
    }

    /* Check .mp3 file */

    if (strlen(argv[2]) < 4 || strcmp(argv[2] + strlen(argv[2]) - 4, ".mp3") != 0)
    {
        printf("Error: Input file is not .mp3\n");
        return e_failure;
    }

    /* Store file name */

    viewInfo->mp3_fname = argv[2];

    return e_success;
}

Status open_mp3_file(ViewInfo *viewInfo)
{
    viewInfo->fptr_mp3 = fopen(viewInfo->mp3_fname, "r");

    if (viewInfo->fptr_mp3==NULL)
    {
        printf("ERROR: Unable to open MP3 file\n");
        return e_failure;
    }

    return e_success;
}


Status read_tags(ViewInfo *viewInfo)
{
    char tag[5];
    unsigned char size_buffer[4];

    unsigned int size;
    int i, j;

    char *data;

    char *supported_tags[] ={"TIT2","TPE1","TALB","TYER","TCON","COMM"};
    char *display_names[]={"Title","Artist","Album","Year","Content","Comment"};

    /* Move offset to 10th position */
    fseek(viewInfo->fptr_mp3, 10, SEEK_SET);

    /* Read 6 tags */
    for (i = 0; i < 6; i++)
    {
        /* Read tag - 4 bytes */
        if(fread(tag, 1, 4, viewInfo->fptr_mp3)!=4)
        {
            return e_failure;
        }
        tag[4] = '\0';

        /* Read size - 4 bytes */
        if(fread(size_buffer, 1, 4, viewInfo->fptr_mp3)!=4)
        {
            return e_failure;
        }

        /* Convert Endianess of size */
        size = ((unsigned int)size_buffer[0] << 24) |
               ((unsigned int)size_buffer[1] << 16) |
               ((unsigned int)size_buffer[2] << 8) |
               size_buffer[3];

        /* Skip 3 bytes: 2 bytes flags + 1 byte null */
        fseek(viewInfo->fptr_mp3, 3, SEEK_CUR);

        if(size==0)
        {
            return e_failure;
        }

        data= malloc(size);

        if (data== NULL)
        {
            return e_failure;
        }

        if(fread(data, 1, size - 1, viewInfo->fptr_mp3)!=size-1)
        {
            free(data);
            return e_failure;
        }

        data[size - 1] = '\0';

        /* Check whether tag is one of the 6 supported tags */


        for (j = 0; j < 6; j++)
        {
            if(strcmp(tag,supported_tags[j])==0)
            {
                printf("%d\t\t%s\t\t%s\n",i + 1, display_names[j], data);

                break;
            }
        }
        free(data);
    }
    return e_success;
}
Status do_view(ViewInfo *viewInfo)
{
    /* open MP# file */
    if (open_mp3_file(viewInfo) == e_failure)
    {
        return e_failure;
    }

    printf("\n");
    printf("\t<-------------Started view------------->\n");
    printf("\n");

    printf("------------------------------------------------------------\n");
    printf("Sl.no\t\tTAG\t\tContent\n");
    printf("------------------------------------------------------------\n");

    if (read_tags(viewInfo) == e_failure)
    {
        fclose(viewInfo->fptr_mp3);
        return e_failure;
    }

    printf("------------------------------------------------------------\n");
    printf("\n");
    printf("\t<---------------End of view-------------->\n");

    fclose(viewInfo->fptr_mp3);

    return e_success;
}