#include<stdio.h>
#include<string.h>
#include "mp3view.h"

int main(int argc,char *argv[])
{
    ViewInfo viewInfo;

    if(argc<2)
    {
        printf("Error:Invalid Arguments\n");
        printf("USAGE:\n");
        printf("To view: Please pass like: ./a.out -v mp3filename\n");
        printf("To edit: Please pass like: ./a.out -e -t/-a/-A/-m/-y/-c mp3filename\n");

        return 1;
    }

    if(check_operation_type(argv[1])==e_view)
    {
        if(read_and_validate_view(argc,argv,&viewInfo)==e_success)
        {
            if(do_view(&viewInfo)==e_success)
            {
                printf("Success\n");
            }
            else
            {
                printf("Failure\n");
            }
        }
    }
}
    
OperationType check_operation_type(char *argv)
{
    if(strcmp(argv,"-v")==0)
    {
        return e_view;
    }
    else if(strcmp(argv,"-e")==0)
    {
        return e_edit;
    }
    else if(strcmp(argv,"-h")==0)
    {
        return e_help;
    }
    {
        return e_unsupported;
    }
    return 0;
}