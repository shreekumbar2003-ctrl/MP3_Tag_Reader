#include<stdio.h>
#include<string.h>
#include "mp3view.h"
#include "mp3edit.h"

int main(int argc,char *argv[])
{
    ViewInfo viewInfo;
    EditInfo editInfo;

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
    else if(check_operation_type(argv[1])==e_edit)
    {
        if(read_and_validate_edit(argc,argv,&editInfo)==e_success)
        {
            if(do_edit(&editInfo)==e_success)
            {
                printf("Success\n");
            }
            else
            {
                printf("Failure\n");
            }
        }
    }
    else if(check_operation_type(argv[1])==e_help)
    {
        printf("1. -v->to view mp3 file contents\n");
        printf("2. -e->to edit mp3 file contents\n");
        printf("    2.1. -t->to edit song title\n");
        printf("    2.2. -a->to edit artist name\n");
        printf("    2.3. -A->to edit album name\n");
        printf("    2.4. -y->to edit year\n");
        printf("    2.5. -m->to edit content\n");
        printf("    2.6. -c->to edit comment\n");
    }
    else
    {
        printf("Error:Unsupported operation\n");
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
    else if(strcmp(argv,"--help")==0)
    {
        return e_help;
    }
    {
        return e_unsupported;
    }
}