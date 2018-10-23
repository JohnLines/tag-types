
/*
	tagcount - counts things in a tag file

Note that this is ALPHA software, developed piecemeal while I
was testing various ideas.


Released under the GNU Copyright.

        Author: John Lines <john@paladin.demon.co.uk>




Things to do

	implement -v, -x and -a

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


#define MAXTAGLABELS 20
#define MAXLABELLEN 127
#include "tagtypes.h"

char version[6]="0.04";

/* Global flags */
int doheader=1;
#define list 1
#define table 2
#define form 3
int htmlmode=list;
int numtaglabels=0;
char taglabels[MAXTAGLABELS][MAXLABELLEN];
char countfield[MAXLABELLEN];
char countvalue[MAXLABELLEN];
int countvalues=0;
int numrecords=0;
int printrecords=0;
int numfields=0;
int printfields=0;
int invert=0;
int allfields=0;

struct labelrec {
 char *tagname;
 int count;
 struct labelrec *next;
};



void parseargs(int argc,char *argv[],struct tagfile* infile, FILE* outfile);

void countfields(struct tagrecord *rec);

int main(int argc, char *argv[]) {
struct tagfile *infile;
FILE *outfile=stdout;

 struct tagrecord *record;

infile=opentagfilestdin();

parseargs(argc,argv,infile,outfile);



if (doheader) {
    record=gettagrec(infile);
    checktagresult("tagcount",record,infile);
    /* we dont do anything with the header - just throw it away */
    freetagrec(record);
    } 


while (!feof(infile->f)) {
  record=gettagrec(infile);
  checktagresult("tagcount",record,infile);
  if (record) numrecords++;
  if (printfields) countfields(record);
  freetagrec(record);
 }

if (printrecords) {
 fprintf(outfile,"%d\n",numrecords);
 } 

if (printfields) {
 fprintf(outfile,"%d\n",numfields);
 } 
return 0;
}

void parseargs(int argc,char *argv[], struct tagfile *infile, FILE *outfile)
{

int arg=1;


/* Look for flags */

while ((arg<argc) && (argv[arg][0]=='-')) {
 switch (argv[arg][1]) {
case 'h':
  doheader=0; break;
case 'r':
  printrecords=1; break;
case 'f':
  arg++;
  strncpy(countfield,argv[arg],MAXLABELLEN);
  printfields=1;
  break;
case 'v':
  arg++;
  strncpy(countfield,argv[arg],MAXLABELLEN);
  printfields=1;
  arg++;
  strncpy(countvalue,argv[arg],MAXLABELLEN);
  countvalues=1;
  break;
case 'x':
  invert=1;
  break;
case 'a':
  allfields=1;
  break;
default:
   fprintf(stderr,"Tagcount - unknown switch %c\n",argv[arg][1]);
}
arg++;
}
/* after the switches we should have an input file and an output file, if suppli
ed */

if (arg<argc) {
/*  fprintf(stderr,"Opening input file %s\n",argv[arg]); */
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



}


void countfields(struct tagrecord *rec)
/* Check to see if the field we are  counting is in the current record */
{
 struct tagrecord *current;

current=rec;
if (current==0) return;

while (current != 0) {
 if (strcmp(current->tagname,countfield)==0) {
   if (countvalues) {
/*      printf("Field %s value %s compared to %s\n",current->tagname,current->value,countvalue); */
      if (strcmp(current->value,countvalue)==0) numfields++;
      } else
   numfields++;
   }
current=current->next;
}



}


