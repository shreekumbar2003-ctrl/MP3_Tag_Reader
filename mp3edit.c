#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "mp3edit.h"
#include "types.h"

Status read_and_validate_edit(int argc,char *argv[],EditInfo *editInfo)
{
    if(argc!=5)
    {
        printf("Error:Invalid Arguments\n");
        printf("USAGE:\n");
        printf("To edit title:./a.out -e -t\"TItle\"mp3file.mp3\n");
        printf("To edit artist:./a.out -e -a\"Artist\"mp3file.mp3\n");
        printf("To edit album:./a.out -e -A\"Album\"mp3file.mp3\n");
        printf("To edit year:./a.out -e -y\"Year\"mp3file.mp3\n");
        printf("To edit content:./a.out -e -m\"Content\"mp3file.mp3\n");
        printf("To edit comment:./a.out -e -c\"Comment\"mp3file.mp3\n");


        return e_failure;
    }

    if(strcmp(argv[2],"-t")!=0 &&
        strcmp(argv[2],"-a")!=0 &&
        strcmp(argv[2],"-A")!=0 &&
        strcmp(argv[2],"-y")!=0 &&
        strcmp(argv[2],"-m")!=0 &&
        strcmp(argv[2],"-c")!=0)
        {
            printf("Error:Invalid edit opetion\n");
            return e_failure;
        }

        if(strlen(argv[4])<4||strcmp(argv[4]+strlen(argv[4])-4,".mp3")!=0)
        {
            printf("Error:Input file is not .mp3\n");
            return e_failure;
        }

        editInfo->edit_option=argv[2];
        editInfo->new_data=argv[3];
        editInfo->mp3_fname=argv[4];

        return e_success;
}

Status open_edit_files(EditInfo *editInfo)
{
    editInfo->fptr_src=fopen(editInfo->mp3_fname,"r");
    if(editInfo->fptr_src==NULL)
    {
        printf("Error:Unable to open MP3 file\n");
        return e_failure;
    }
    editInfo->fptr_dest=fopen("temp.mp3","w");

    if(editInfo->fptr_dest==NULL)
    {
        printf("ERROR:Unable to create temp.mp3\n");
        fclose(editInfo->fptr_src);
        return e_failure;
    }
    return e_success;
}

Status do_edit(EditInfo *editInfo)
{
    char header[10];
    char tag[5];

    unsigned char size[4];
    unsigned char three_bytes[3];

    unsigned int old_size;
    unsigned int new_size;

    char *required_tag;
    unsigned char temp;

    char *options[]={"-t","-a","-A","-y","-m","-c"};
    char *tags[]={"TIT2","TPE1","TALB","TYER","TCON","COMM"};

    int i;

    /* open source and destination files */

    if (open_edit_files(editInfo)==e_failure)
    {
        return e_failure;
    }

    for(i=0;i<6;i++)
    {
        if(strcmp(editInfo->edit_option,options[i])==0)
        {
            required_tag=tags[i];
            break;
        }
    }
    
    /* Copy ID3 header */
    fseek(editInfo->fptr_src,0,SEEK_SET);
    fread(header,1,10,editInfo->fptr_src);
    fwrite(header,1,10,editInfo->fptr_dest);

    /* Move to first tag */
    fseek(editInfo->fptr_src,10,SEEK_SET);

    while(1)
    {
        /* Read 4 byte tag */
        if(fread(tag,1,4,editInfo->fptr_src)!=4)
        {
            break;
        }
        tag[4]='\0';

        /* read 4 byte size */
        if(fread(size,1,4,editInfo->fptr_src)!=4)
        {
            break;
        }

        temp=size[0];
        size[0]=size[3];
        size[3]=temp;

        temp=size[1];
        size[1]=size[2];
        size[2]=temp;

        old_size=((unsigned int)size[0])|
            ((unsigned int)size[1]<<8)|
            ((unsigned int)size[2]<<16)|
            ((unsigned int)size[3]<<24);

            /* read 3 bytes */
            fread(three_bytes,1,3,editInfo->fptr_src);
            
            /* Check required tag */
            if(strcmp(tag,required_tag)==0)
            {
                fwrite(tag,1,4,editInfo->fptr_dest);
                new_size=strlen(editInfo->new_data)+1;

                /*convert new size to big endian*/
                size[0]=(new_size>>24)&0xFF;
                size[1]=(new_size>>16)&0xFF;
                size[2]=(new_size>>8)&0xFF;
                size[3]=new_size&0xFF;
                
                /* cpoy new size  */
                fwrite(size,1,4,editInfo->fptr_dest);

                /* copy 3 bytes */
                fwrite(three_bytes,1,3,editInfo->fptr_dest);

                /* copy new data */
                fwrite(editInfo->new_data,1,strlen(editInfo->new_data),editInfo->fptr_dest);

                /* skip old data */
                fseek(editInfo->fptr_src,old_size-1,SEEK_CUR);
                break;
            }

            /* copy original tag */
            fwrite(tag,1,4,editInfo->fptr_dest);

            /*convert size back to Big endian */
            temp=size[0];
            size[0]=size[3];
            size[3]=temp;

            temp=size[1];
            size[1]=size[2];
            size[2]=temp;

            /* copy original size */
            fwrite(size,1,4,editInfo->fptr_dest);

            /* copy 3 bytes */
            fwrite(three_bytes,1,3,editInfo->fptr_dest);

            /* copy old data */
            for(int i=0; i<old_size-1;i++)
            {
                int ch;
                ch=fgetc(editInfo->fptr_src);
                if(ch==EOF)
                {
                    break;
                }
                fputc(ch,editInfo->fptr_dest);
            }
        }
        char buffer[1024];
        int bytes;

        while((bytes=fread(buffer,1,1024,editInfo->fptr_src))>0)
        {
            fwrite(buffer,1,bytes,editInfo->fptr_dest);
        }

        fclose(editInfo->fptr_src);
        fclose(editInfo->fptr_dest);

        /* Remove original MP# file */
        if (remove(editInfo->mp3_fname) != 0)
        {
            printf("Error: Unable to remove original file\n");
            return e_failure;
        }

        /* Rename temp.mp3 to original MP3 file */
        if (rename("temp.mp3", editInfo->mp3_fname) != 0)
        {
            printf("Error: Unable to rename temp.mp3\n");
            return e_failure;
        }


        printf("Editing completed successfully\n");
        return e_success;


    }
