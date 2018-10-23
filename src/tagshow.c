
/*
	tagshow - display a tag file - selected fields.

Note that this is ALPHA software, developed piecemeal while I
was testing various ideas.


Released under the GNU Copyright.

        Author: John Lines <john@paladin.demon.co.uk>





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
int numtaglabels=0;
int numtagheaders=0;
char taglabels[MAXTAGLABELS][MAXLABELLEN];
double totals[MAXTAGLABELS];
int digits[MAXTAGLABELS];	/* number of digits of precision to print */



void parseargs(int argc,char *argv[],struct tagfile *infile, FILE * outfile);

void showrecord(FILE *f, struct tagrecord *rec);

int main(int argc, char *argv[]) {

 struct tagrecord *record;
struct tagfile *infile;
FILE *outfile=stdout;

infile=opentagfilestdin();

parseargs(argc,argv,infile,outfile);



if (doheader) {
    record=gettagrec(infile);
    checktagresult("tagshow",record,infile);
    /* we dont do anything with the header - just throw it away */
    freetagrec(record);
    } 


while (!feof(infile->f)) {
  record=gettagrec(infile);
    checktagresult("tagshow",record,infile);
  showrecord(outfile,record);
  freetagrec(record);
 }


fprintf(outfile,"%.*f\n",digits[0],totals[0]);

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
case 't':
  arg++;
  while ((arg<argc) && (argv[arg][0]!='-')) {
  strncpy(taglabels[numtaglabels],argv[arg],MAXLABELLEN); numtaglabels++;
  if (argv[arg][0]!='-') arg++;
  }
  break;
default:
   fprintf(stderr,"tagshow - unknown switch %c\n",argv[arg][1]);
}
arg++;
}
/* after the switches we should have an input file and an output file, if suppli
ed */

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



}


void showrecord(FILE *f, struct tagrecord *rec)
{
 struct tagrecord *current;
 double value;
 int i;
 int j;
char *point;

current=rec;
if (current==0) return;

/* work our way through the taglabels looking for ones which match */
for (i=0; i<numtaglabels ; i++) {
current=rec;   /* start at the beginning again */
while (current != 0) {
 if (strcmp(current->tagname,taglabels[i])==0) {
/* we will only deal with the totals case at the moment */
/*   fprintf(f,"<td>%s</td>",current->value); */
  value=atof(current->value);
/* how many decimal places here ? */
  point=current->value; j=0;
  while (point[0]) {
    if (point[0]=='.') { j=strlen(point)-1; break; }
    point++;
    }
    if (j>digits[i]) digits[i]=j;
/* 	fprintf(stderr,"value is %s, j is %d\n",current->value,j); */
  totals[i]=totals[i]+value;
   }
current=current->next;
}
}


}


