
/*
	tagdiff - Report on the difference between two tagged files


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
struct tagrecord* header2;
char fn[255];		/* file name of current file with appended colon */
struct tagfile *tagfile1, *tagfile2;
struct checkrec {
	struct tagrecord *rec;
	int matched;
	struct checkrec *next;
};
struct checkrec *file2;


struct checkrec *gettagfile(struct tagfile *f);
int difftagrec(struct tagfile *f, struct tagrecord *rec);
void parseargs(int argc,char *argv[]);
void print_unmatched_recs(struct checkrec *file2);
void printtagrec(struct tagrecord *rec, char ch);
int printtagdiff(struct tagrecord* r1, struct tagrecord* r2);

int main(int argc, char *argv[]) {

 struct tagrecord *record, *header;

 struct tagfile* outfile;

outfile=opentagfilestdout();

parseargs(argc,argv);


file2=gettagfile(tagfile2);


if (doheader) {
    header=gettagrec(tagfile1);
    checktagresult("tagdiff",header,tagfile1);

/*  ought to check the file1 header against the file 2 header - if they
are different then we should give up now */

    freetagrec(header);
    } 


while (!feof(tagfile1->f)) {
  int i;
  record=gettagrec(tagfile1);
  checktagresult("tagdiff",record,tagfile1);
  i=difftagrec(outfile,record);
  freetagrec(record);
 }
closetagfile(tagfile1);

/* now print all the records in file2 which do not have the matched flag */
 print_unmatched_recs(file2);

return 0;
}

void parseargs(int argc,char *argv[])
{
int arg=1;	/* pointer to current active file in argv */

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

/* the rest of the arguments are assumed to be files  */

if ((tagfile1=opentagfile(argv[arg],"r"))==NULL) {
   fprintf(stderr,"Tagdiff could not open %s/n",argv[arg]);
   exit;
   }
arg++;
if ((tagfile2=opentagfile(argv[arg],"r"))==NULL) {
   fprintf(stderr,"Tagdiff could not open %s/n",argv[arg]);
   exit;

}
}

struct checkrec *gettagfile(struct tagfile *f) {

struct checkrec *current=0;
struct checkrec *first=0;
struct checkrec *previous=0;

/* read the header record into header2 */
if (doheader) {
   header2=gettagrec(f);
   checktagresult("tagdiff",header2,f);
   }



while (!feof(f->f)) {
 current=(struct checkrec *)malloc(sizeof(struct checkrec));
 if (previous!=0) previous->next=current;                               
     previous=current; 
     current->rec=gettagrec(f);
               checktagresult("tagdiff",current->rec,f);
     current->matched=0;
     current->next=0;
     if (first==0) first=current;
 
}

return first;
}

int difftagrec(struct tagfile *f, struct tagrecord *rec) {
 struct checkrec *rec2; 
 int status;

if (rec==0) return 0;

/*
fprintf(stderr,"Differences - primary tags %s:%s - %s:%s\n",
	rec->tagname,rec->value,file2->rec->tagname,file2->rec->value);
*/

/* called for every record in file1 - try to find equivalent record in file2 */
/* At the moment I will assume that the first tag is the primary tag
   in every case
*/
rec2=file2;
while ((rec2 !=0) && (rec2->rec !=0)) {
/* fprintf(stderr,"Comparing - primary tags %s:%s - %s:%s\n",
	rec->tagname,rec->value,rec2->rec->tagname,rec2->rec->value); */
  if ((strcmp(rec2->rec->tagname,rec->tagname)==0) && (strcmp(rec2->rec->value,rec->value)==0)) {
    /* we have found a matching tag name and value - check the rest of the
	tags */
/*     fprintf(stderr,"found matching tag\n"); */
/* need a similar strategy to doing the whole thing - we need to pair off
 tags in both records, and display any differences */

    status=printtagdiff(rec2->rec, rec); 

    rec2->matched=1;
    return status;

     }
    else rec2=rec2->next;
}

/* If we get here then we didnt find a matching tag */
/* fprintf(stderr,"Failed to find matching tag for %s:%s\n",rec->tagname,rec->value);
 	*/
  printtagrec(rec,'-');
return 0;
}

int printtagdiff(struct tagrecord* r1, struct tagrecord* r2) {
char taglabels[MAXTAGLABELS][MAXLABELLEN];
int matched[MAXTAGLABELS];
int numr2labels=0;
int i;
int primarytagprinted=0;
int thismatched;
struct tagrecord* p1;
struct tagrecord* p2;
int founddifference=0;

p2=r2;
while (p2 !=0) {
 strncpy(taglabels[numr2labels],p2->tagname,MAXLABELLEN); 
 matched[numr2labels]=0;
 numr2labels++;
 p2=p2->next;
 }

/* go through the tags in r1, and check that they match a record in tag2*/
p1=r1;
while (p1 !=0) {
  thismatched=0;
  for (i=0; i<numr2labels ; i++) {
      if (strcmp(p1->tagname,taglabels[i])==0) {
	/* found matching label */
	matched[i]=1;
	/* are the contents the same ? */
	p2=r2;
	while (p2 !=0) {
		if (strcmp(p1->tagname,p2->tagname)==0) {
		/* found the record (again) */
		thismatched=1;
			if (strcmp(p1->value,p2->value)!=0) {
			/* found a difference - print the records */
		if (! primarytagprinted) { printf("%s: %s\n",r1->tagname,r1->value) ; primarytagprinted=1; }
		printf("-%s: %s\n",p1->tagname,p1->value);
		printf("+%s: %s\n",p2->tagname,p2->value);
		founddifference=1;
			}
		}
		p2=p2->next;
	}
  }

 }
/* if we didnt find a label which matched then this  */

 if (!thismatched) {
	if (! primarytagprinted) { printf("%s: %s\n",r1->tagname,r1->value) ; primarytagprinted=1; }
	printf("+%s: %s\n",p1->tagname,p1->value);
	founddifference=1;
	}

 p1=p1->next;
}

/* check that there were no tags in r2 which we missed */

for (i=0; i<numr2labels ; i++) {
 if (!matched[i]) {
   /* find its value */
 p2=r2;
 while (p2 !=0) {
    if (strcmp(p2->tagname,taglabels[i])==0) {
        if (p2->delimiter==0) 
        printf("-%s: %s\n",p2->tagname,p2->value);
          else printf("-%s:: %s\n%s%s\n",p2->tagname, p2->delimiter, p2->value, p2->delimiter);
        } 

    p2=p2->next;
    }  

	}
}

return founddifference;
}

void print_unmatched_recs(struct checkrec *file2) {

struct checkrec *rec2;

rec2=file2;
while (rec2 !=0) {
 if (rec2->matched==0) {
    printtagrec(rec2->rec,'+');
      }
   rec2=rec2->next;
}
}

void printtagrec(struct tagrecord *rec, char ch) {
/* used to print complete tag records which are present in one file or the
 other - essentially a clone of puttagrec in taglib.c
*/
      struct tagrecord *current;
      int endseen=0;
       current=rec;
       while (current!=0) {
        if (strcmp(current->tagname,"End")==0) endseen=1; else printf("%c",ch); 
        printf("%s:",current->tagname);
        if (current->delimiter!=0) {
		printf(": %s\n",current->delimiter);
		}
	else
		printf(" ");

	if (current->value!=0) printf("%s",current->value);
	if (current->delimiter==0) printf("\n");
	if (current->delimiter!=0) printf("%s\n",current->delimiter);
	current=current->next;
        }
     if ((!endseen) && (current!=0)) printf("End:\n");
       
}
