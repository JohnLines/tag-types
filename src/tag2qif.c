
/*
	tag2qif - Convert a tag file to a Quicken Interchange File

Note that this is ALPHA software, developed piecemeal while I
was testing various ideas.


Released under the GNU Copyright.

        Author: John Lines <john@paladin.demon.co.uk>




Things to do

        Should allow arguments to select tags which will not be displayed,
	and much more


*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


#define MAXTAGLABELS 20
#define MAXLABELLEN 127
#define MAXLITLEN 255	/* Max length of a literal field */
#define TABLEOPTIONSLEN 80
#include "tagtypes.h"

char version[6]="0.01";

/* Global flags */
int doheader=1;
int numtaglabels=0;
int rownumber=0;
char taglabels[MAXTAGLABELS][MAXLABELLEN];
struct tagfile *infile;
FILE *outfile;



int parseargs(int argc,char *argv[], struct tagfile *infile, FILE *outfile);

void putqifrec(FILE *f, struct tagrecord *rec);
void putqifheader(FILE *f);

int main(int argc, char *argv[]) {

 struct tagrecord *record;

if (parseargs(argc,argv,infile,outfile)) return 1;



if (doheader) {
    record=gettagrec(infile);
    checktagresult("tag2qif",record,infile);
    freetagrec(record);
    /* we dont do anything with the header - just throw it away */
    } 

putqifheader(outfile);

while (!feof(infile->f)) {
  record=gettagrec(infile);
  checktagresult("tag2qif",record,infile);
  rownumber++;
  putqifrec(outfile,record);

freetagrec(record);
 }

return 0;
}

int parseargs(int argc,char *argv[],struct tagfile *infile, FILE *outfile)
{

int arg=1;


/* Look for flags */

while ((arg<argc) && (argv[arg][0]=='-')) {
 switch (argv[arg][1]) {
case 'h':
  doheader=0; arg++; break;
default:
   fprintf(stderr,"Tag2qif - unknown switch %c\n",argv[arg][1]);
   arg++;
}
}
/* after the switches we should have an input file and an output file, if suppli
ed */
infile=opentagfilestdin();
outfile=stdout;

if (arg<argc) {
  if ((infile=opentagfile(argv[arg],"r"))==NULL) {
       fprintf(stderr,"Error opening %s for input\n",argv[arg]);
       exit(0);
       }

  arg++;
  }

if (arg<argc) {
  if ((outfile=fopen(argv[arg],"w"))==NULL) {
        fprintf(stderr,"Error opening %s for output\n",argv[arg]);
        exit(0);
        }
  arg++;
  }


return 0;
}

void putqifheader(FILE *f)
{

fprintf(f,"!Type: Bank\n");
}


void putqifrec(FILE *f, struct tagrecord *rec)
/* put out a tag record as an HTML definition list */
{
 struct tagrecord *current;

current=rec;
if (current==0) return;

while (current!=0) {
 if (strcmp(current->tagname,"End")!=0) {
 if (current->delimiter!=0) {
    /* We have a delimited set of text - QIF cant handle it */
   fprintf(stderr,"Qif cant handle multiline text - ignored \n");
   fprintf(stderr,"%s\n",current->value);
   fprintf(f,"\n");
   } else {
 if (strcmp(current->tagname,"Id") ==0) {
      /* we dont do anything - just throw it away QIF doesnt have Ids */ 
 } else if (strcmp(current->tagname,"Date") ==0) {
    /* mangle the date */
   if (strlen(current->value)==10) {
     fprintf(f,"D%c%c/%c%c/%c%c\n",current->value[5],current->value[6],
	current->value[8],current->value[9],current->value[2],
	current->value[3]);
	}

 } else if (strcmp(current->tagname,"Amount") ==0) {
    fprintf(f,"T%s\n",current->value);
 } else if (strcmp(current->tagname,"Reference") ==0) {
    fprintf(f,"N%s\n",current->value);
 } else if (strcmp(current->tagname,"Catagory")==0) {
    fprintf(f,"L%s\n",current->value);
 } else if (strcmp(current->tagname,"Payee")==0) {
    fprintf(f,"P%s\n",current->value);
 } else if (strcmp(current->tagname,"Description")==0) {
    fprintf(f,"M%s\n",current->value);
 } else if (strcmp(current->tagname,"Cleared-Date")==0) {
    fprintf(f,"CX\n");
    /* it may be a multiple valued tag - convert to QIF split */
 } else if (strstr(current->tagname,"Catagory.") != NULL) {
    fprintf(f,"S%s\n",current->value);
 } else if (strstr(current->tagname,"Description.") != NULL) {
    fprintf(f,"E%s\n",current->value);
 } else if (strstr(current->tagname,"Subamount.") != NULL) {
    fprintf(f,"$%s\n",current->value);
 } else {
   fprintf(stderr,"Cant convert tag %s with value %s\n",current->tagname,
	current->value);
   }
  }
 }
  current=current->next;
}
fprintf(f,"^\n");
}


