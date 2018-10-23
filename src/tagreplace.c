/*
	tagreplace - replace tag records which match the criteria
	given on the command line from stdin and write them to stdout.

	Note that if we cant work out what the which is the primary
	tag, and the first record does not meet the criteria then
	we may lose the headers.


Note that this is ALPHA software, developed piecemeal while I
was testing various ideas.


Released under the GNU Copyright.

        Author: John Lines <john@paladin.demon.co.uk>





*/

#include <stdio.h>
#include <stdlib.h>

#include "tagtypes.h"

#define MAXCONDS 5		/* Maximum number of conditions */

struct condition {
  char fieldname[40];
  char value[40];
  char op;
}; 

/*
	Supported values of op
	=	Numerically equal
	E	String equal

*/

 int recbufsize=10000;        /* Size of a record buffer */
 int debug=0;
 int doheader=1;

void parseargs(int argc,char *argv[], struct condition conditions[MAXCONDS]);

int main(int argc, char *argv[]) {

 struct condition conditions[MAXCONDS];
 struct tagrecord *record;
 char *fieldbuffer;

parseargs(argc,argv,conditions);

fieldbuffer=malloc(recbufsize);

if (doheader) {
    record=gettagrec(stdin,fieldbuffer);
    puttagrec(stdout,record);
    }

while (!feof(stdin)) {
  record=gettagrec(stdin,fieldbuffer);
  if (matchcond(record,conditions)) replacetagrec(stdout,record);
 }
return 0;
}

void replacetagrec(FILE *of, struct tagrecord *rec)
{



}


void parseargs(int argc,char *argv[], struct condition conditions[MAXCONDS])
{
 int i;
 int arg=1;
 int cond=0;
 for (i=0;i<=MAXCONDS ; i++) conditions[i].op = 0;
/*  puts("condition ops set\n"); */

/* look for flags */

while ((arg<argc) && (argv[arg][0]=='-')) {
 switch (argv[arg][1]) {
case 'h':
  doheader=0; break;
case 'd':
  debug=1; break;
default:
  fprintf(stderr,"Tagreplace - unknown switch %c\n",argv[arg][1]);
}
arg++;
}

while (arg<argc) {
 strcpy(conditions[cond].fieldname,argv[arg]); arg++;
 if (arg>=argc) {
     fprintf(stderr,"Error - operator expected\n"); return;
     }
 if (strcmp(argv[arg],"=")==0) conditions[cond].op='E';
 else if (strcmp(argv[arg],"-eq")==0) conditions[cond].op='=';
  else if (strcmp(argv[arg],"-gt")==0) conditions[cond].op='>';
  else {
     printf("Unknown condition operator %s\n",argv[arg]);
     }
 arg++;
 if (arg>=argc) {
     fprintf(stderr,"Error - value expected\n"); return;
     }
  strcpy(conditions[cond].value,argv[arg]); arg++;
}

}


int matchcond(struct tagrecord *record,struct condition conditions[])
{
struct tagrecord *current;
int fieldval,condval;


current=record;
while(current!=0) {

if (debug) {
 fprintf(stderr,"Comparing field name %s value %s with %s value %s\n",
    current->tagname,current->value,conditions[0].fieldname,
    conditions[0].value);
    }

 condval=strtol(conditions[0].value,NULL,10);
 fieldval=strtol(current->value,NULL,10);
/*  printf("condition value %d - field value %d\n",condval,fieldval); */
 switch(conditions[0].op) {
 case '\0':
      break;    /* i.e. no comparison operator */
 case 'E':
 if ((strcmp(current->tagname,conditions[0].fieldname)==0) && 
       (strcmp(current->value,conditions[0].value)==0)) return 1; 
	break;
 case '=':
 if ((strcmp(current->tagname,conditions[0].fieldname)==0) &&
       (condval==fieldval)) return 1;
	break;
 case '>':
 if ((strcmp(current->tagname,conditions[0].fieldname)==0) &&
       (fieldval > condval)) return 1;
	break;
  default:
     printf("Error - invalid comparison operator\n");
 }


  current=current->next;
 }
return 0;
}

