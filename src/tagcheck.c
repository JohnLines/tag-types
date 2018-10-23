
/*
	tagcheck - Check a tag file


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
int readdescriptor=1;
int numtaglabels=0;
char taglabels[MAXTAGLABELS][MAXLABELLEN];
int status=0;
struct tagrecord* descfield[255];
int numdescfields=0;
int showfilenames=0;
char fn[255];		/* file name of current file with appended colon */
int arg=1;	/* pointer to current active file in argv */
struct tagfile *tagf;


int checktagrec(FILE *f, struct tagrecord *rec);
void parseargs(int argc,char *argv[]);

int main(int argc, char *argv[]) {

 struct tagrecord *record, *header;

parseargs(argc,argv);
if (argc>arg+1) showfilenames=1;


while (arg<argc) {
 if ((tagf=opentagfile(argv[arg],"r"))!=NULL) {
  if (showfilenames) {
    strncpy(fn,argv[arg],254);
    strcat(fn,":"); 
    }
 numdescfields=0;

if (doheader) {
    header=gettagrec(tagf);
    checktagresult("tagcheck",header,tagf); 
 if (readdescriptor) {
   struct tagfile *descfile;
   descfile=opentagdesc(header);
   if (descfile) {
	/* Descriptors always start with a header, which in this case we
	probably dont want - stick it in record 0 just in case */
 	descfield[0]=gettagrec(descfile); numdescfields++;
	checktagresult("Tagcheck",descfield[0],descfile); 
	while (!feof(descfile->f)) {
	descfield[numdescfields]=gettagrec(descfile); numdescfields++;
	checktagresult("Tagcheck",descfield[numdescfields],descfile); 
	}
	closetagfile(descfile);

       }
 else {
	printf("%sWarning - could not read associated descriptor\n",fn);
	status=1;
	}
      

   } 
    freetagrec(header);
    } 


while (!feof(tagf->f)) {
  int i;
  record=gettagrec(tagf);
/*  if ((record==0) && (tagf->error!=0)) { 
	status=1;
	printf("Tagcheck - taglib error %d (%s) at line %d \n",tagf->error,tagerr2str(tagf->error),tagf->currentline);

 } */
  checktagresult("tagcheck",record,tagf); 

  i=checktagrec(stdout,record);
  if (i>status) status=i;
  freetagrec(record);
 }
closetagfile(tagf);
}
else fprintf(stderr,"Could not open %s\n",argv[arg]);
arg++;
}
return status;
}

void parseargs(int argc,char *argv[])
{

/* Look for flags */

while ((arg<argc) && (argv[arg][0]=='-')) {
 switch (argv[arg][1]) {
case 'h':
  doheader=0; break;
default:
   fprintf(stderr,"tagcheck - unknown switch %c\n",argv[arg][1]);
}
arg++;
}

/* the rest of the arguments are assumed to be files - dealt with in main */

}

int checktagrec(FILE *f, struct tagrecord *rec)
{
 struct tagrecord *current;
 int i;
 char descTagname[255];
 char primaryTagvalue[255];
 int descfound=0;
 int status=0;
 int firsttag=1;

 current=rec;
 primaryTagvalue[0]='\0';

 while (current) {
   /* is it an End tag ??? */
  if (strcmp(current->tagname,"End")==0) {
     /* If a value is supplied then check that it matches the primary tag */
     if (strlen(current->value)!=0) {
	if (strcmp(current->value,primaryTagvalue)!=0) {
		printf("%sEnd tag %s does not match primary key %s\n",
			fn,current->value,primaryTagvalue); status=3;
			}
	}

   } else {  /* not an End tag */
   descfound=0;
   /* Find the corresponding descriptor record */
	for (i=0; i<=numdescfields ; i++) {
           if (taggetvalue(descTagname,descfield[i],"Tagname",255))
		if (strcmp(current->tagname,descTagname)==0) {
			descfound=1;
	/* while we are looking at the right descriptor we will try to find
	out a bit more about what we should be looking for */
			/* get the ValueType field from the descriptor */
			

			break;
			}
	} 
       if (!descfound && (numdescfields!=0)) {
	 printf("%sDid not find tagname %s in the descriptor\n",fn,current->tagname);
	 status=2;
	 }
/*	If it is a primary tag, or there is no descriptor and it is the
	first tag then save its value */
	if ((numdescfields==0) && firsttag) {
		firsttag=0;
		strncpy(primaryTagvalue,current->value,255);
		}
	}

	current=current->next;
	}

return status;

} 


