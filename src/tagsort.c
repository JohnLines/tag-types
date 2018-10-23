
/*
	tagsort - Sort a tagged file


Released under the GNU Copyright.

        Author: John Lines <john@paladin.demon.co.uk>

	In memory version

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXTAGLABELS 80
#define MAXLABELLEN 127
#define MAXKEYNAMELEN 127
#define FORWARD 1
#define REVERSE 2
#include "tagtypes.h"

/* Global flags */
int doheader=1;
int readdescriptor=1;
int status=0;
int numdescfields=0;
char keyname[MAXKEYNAMELEN];
int direction=FORWARD;
struct tagfile *tagf;
struct sortrec {
	struct tagrecord *rec;
	struct sortrec *prev;
	struct sortrec *next;
};
struct sortrec *file;
struct sortrec *highest, *lowest;


struct sortrec *treegettagfile(struct tagfile *f);
void treeprint(struct sortrec *rec, int direction);
void parseargs(int argc,char *argv[]);

int main(int argc, char *argv[]) {

 struct tagrecord *header;

parseargs(argc,argv);



if (doheader) {
    header=gettagrec(tagf);
    checktagresult("tagsort",header,tagf);
/*     freetagrec(header); */
    } 

file=treegettagfile(tagf);

treeprint(file,direction);

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
case 'r':
  direction=REVERSE; break;
case 'k':
   arg++;
   strncpy(keyname,argv[arg],MAXKEYNAMELEN);
   break;
default:
   fprintf(stderr,"tagcheck - unknown switch %c\n",argv[arg][1]);
}
arg++;
}

/* the rest of the arguments are assumed to be files  */

if ((tagf=opentagfile(argv[arg],"r"))==NULL) {
   fprintf(stderr,"Tagdiff could not open %s/n",argv[arg]);
   exit;
   }

}

struct sortrec *treegettagfile(struct tagfile *f) {

struct sortrec *current=0;
struct sortrec *first=0;
struct sortrec *p=0;

struct tagrecord *rec;

while (!feof(f->f)) {
 rec=gettagrec(f);
 checktagresult("tagsort",rec,f);
 if (rec) {
 current=(struct sortrec *)malloc(sizeof(struct sortrec));
/* we have a sort record - work out where to put it */
 if (first ==0) {
	first=current;
	current->rec=rec;
	current->prev=0;
	current->next=0;
	}
   else
   {
	p=first;
/* work through the records and see which way to go */
/*     if (tagcmp(p)	*/
   }

 
}
}

return first;
}

void treeprint(struct sortrec *rec, int direction) {

/* The tree - starting at rec should have one link with a null next and one
 with a null prev pointer - depending on which direction we are sorting in
 we start at one, follow links and print until we come to the other
*/


}
