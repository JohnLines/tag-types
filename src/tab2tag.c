
/*
	tab2tag - Convert a tab seperated file to a tag file

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
int recbufsize=10000;        /* Size of a record buffer */
int mimeheader=0;
int numtaglabels=0;
char taglabels[MAXTAGLABELS][MAXLABELLEN];

struct tagrecord *gettabrec(FILE *f,char *fieldbuffer);

void parseargs(int argc,char *argv[]);

void getmimeheader(FILE *f);

int main(int argc, char *argv[]) {

 struct tagrecord *record;
 char *fieldbuffer;
struct tagfile *outfile;

outfile=opentagfilestdout();

parseargs(argc,argv);


fieldbuffer=malloc(recbufsize);

if (mimeheader) getmimeheader(stdin);

while (!feof(stdin)) {
  record=gettabrec(stdin,fieldbuffer);
  puttagrec(outfile,record);
 }

return 0;
}

void parseargs(int argc,char *argv[])
{

int arg=1;

/* Look for flags */

while ((arg<argc) && (argv[arg][0]=='-')) {
 switch (argv[arg][1]) {
case 'm':
  mimeheader=1; break;
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

void getmimeheader(FILE *f)
{
 int bufptr=0;
 char ch;

while (!feof(f)) {
 ch=fgetc(f);
 if ((ch =='\t')||(ch=='\n')) {
	bufptr=0;
	numtaglabels++;
	if (ch=='\n') return;
  }
 else {
      taglabels[numtaglabels][bufptr]=ch;
      bufptr++;
  } 

}

return;
}

struct tagrecord *gettabrec(FILE *f,char *fieldbuffer)
{

/* need to deal with the cases of
       more tab fields than labels
	more labels than tab fields
*/

 char tagname[128];
 struct tagrecord *current=0;
 struct tagrecord *previous=0;
 struct tagrecord *first=0;

 int labelindex=0;
 int bufptr=0;
 char ch;
 int multiline=0;	/* flag to say we are multiline buffer */


while (!feof(f)) {
 ch=fgetc(f);
 if ((ch =='\t')||(ch=='\n')) {
      fieldbuffer[bufptr]=0;
       /* allocate a record structure for the fields */
         current= (struct tagrecord *)malloc(sizeof(struct tagrecord));
         if (previous!=0) previous->next=current;
         previous=current;
	strncpy(tagname,taglabels[labelindex],MAXLABELLEN);
	labelindex++;
         current->tagname=malloc(strlen(tagname)+1);
         strcpy(current->tagname,tagname);
         current->value=malloc(strlen(fieldbuffer)+1);
         strcpy(current->value,fieldbuffer);
	if (multiline) {
		current->delimiter=(malloc(11));
		snprintf(current->delimiter,11,"END_%06d",rand());
		}
         else current->delimiter=0;
         current->next=0;
         if (first==0) first=current;
	bufptr=0;
	multiline=0;
	if (ch=='\n') {
       /* allocate a record for an end label */
         current= (struct tagrecord *)malloc(sizeof(struct tagrecord));
         if (previous!=0) previous->next=current;
         previous=current;
        strcpy(tagname,"End");
         current->tagname=malloc(strlen(tagname)+1);
         strcpy(current->tagname,tagname);
         current->value=0;
         current->delimiter=0;
         current->next=0;
         if (first==0) first=current;

            if (current != 0) current->next=0;
         return first;
	}
  }
 else {
     /* yet another special case - deal with \n and \t in buffers */
     if (ch=='\\') {
         char nextch;
         nextch=fgetc(f);
         if (nextch=='t') ch='\t';
          else if (nextch=='n') {
		/* We are now a multiline buffer */
		ch='\n'; multiline=1;
		}
	else {
		/* Oh dear - this was a \ which was not followed by t or n
			put it in the buffer */
		fieldbuffer[bufptr]=ch; bufptr++;
		ch=nextch;
		}	
	}

      fieldbuffer[bufptr]=ch;
      bufptr++;
  } 

}

return 0;
}
