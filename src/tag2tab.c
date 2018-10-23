
/*
	tag2tab - Convert a tag file to tag seperated


Released under the GNU Copyright.

        Author: John Lines <john@paladin.demon.co.uk>

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXTAGLABELS 80
#define MAXLABELLEN 127
#include "tagtypes.h"

/* Global flags */
int doheader=1;
int mimeheader=0;
int readdescriptor=0;
int numtaglabels=0;
char taglabels[MAXTAGLABELS][MAXLABELLEN];


void parseargs(int argc,char *argv[]);

void puttabrec(FILE *f, struct tagrecord *rec);

int main(int argc, char *argv[]) {

 struct tagrecord *record, *header;
 struct tagfile *infile;

infile=opentagfilestdin();

parseargs(argc,argv);



if (doheader) {
    header=gettagrec(infile);
    checktagresult("tag2tab",header,infile);
 if (readdescriptor) {
   struct tagfile *descfile;
   descfile=opentagdesc(header);
   if (descfile) {
	struct tagrecord *dscrecord;
	char label[MAXLABELLEN];
	while (!feof(descfile->f)) {
	dscrecord=gettagrec(descfile);	
        checktagresult("tag2tab",dscrecord,descfile);
	if (taggetvalue(label,dscrecord,"Tagname",MAXLABELLEN)) {
		strncpy(taglabels[numtaglabels],label,MAXLABELLEN); numtaglabels++;
		}
	freetagrec(dscrecord);
	}
	closetagfile(descfile);
	}

   } 
    freetagrec(header);
    } 

if (mimeheader) {
 int i; 
fprintf(stdout,"%s",taglabels[0]);
for (i=1; i<numtaglabels ; i++) {
 fprintf(stdout,"\t%s",taglabels[i]);
 }
fprintf(stdout,"\n");
}

while (!feof(infile->f)) {
  record=gettagrec(infile);
  checktagresult("tag2tab",record,infile);
  puttabrec(stdout,record);
  freetagrec(record);
 }


return 0;
}

void parseargs(int argc,char *argv[])
{

int arg=1;


/* Look for flags */

while ((arg<argc) && (argv[arg][0]=='-')) {
 switch (argv[arg][1]) {
case 'h':
  doheader=0; break;
case 'm':
  mimeheader=1; break;
case 'D':
 readdescriptor=1; break;
case 't':
  arg++;
  while ((arg<argc) && (argv[arg][0]!='-')) {
  strncpy(taglabels[numtaglabels],argv[arg],MAXLABELLEN); numtaglabels++;
  arg++;
  }
  break;
default:
   fprintf(stderr,"Tag2tab - unknown switch %c\n",argv[arg][1]);
}
arg++;
}


}


void puttabrec(FILE *f, struct tagrecord *rec)
/* put out a tag record as a set of TAB seperated fields */
{
 struct tagrecord *current;
 int i;
 int first=1;

current=rec;
if (current==0) return;
/*
*/

/* work our way through the taglabels looking for ones which match */
for (i=0; i<numtaglabels ; i++) {
if ( ! first ) fprintf(f,"\t");
first=0;
current=rec;   /* start at the beginning again */
while (current != 0) {
 if (strcmp(current->tagname,taglabels[i])==0) {
/* If it is a multiline tag then we will replace newlines with \n */
/* we will also replace tab characters with \t */
   char *val=current->value;
   int nextchar;
   char ch;
   while (val) {
     nextchar=strcspn(val,"\t\n");
     ch=val[nextchar]; val[nextchar]='\0'; 
   fprintf(f,"%s",val);
     val=val+nextchar+1;
     if (ch=='\t') fprintf(f,"\\t");
     if (ch=='\n') fprintf(f,"\\n");
     if (ch=='\0') val=0;
   }
   }
current=current->next;
}
}
fprintf(f,"\n");


}



