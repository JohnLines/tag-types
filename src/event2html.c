
/*
	event2html - Convert an event file to html

Note that this is ALPHA software, developed piecemeal while I
was testing various ideas.


Released under the GNU Copyright.

        Author: John Lines <john@paladin.demon.co.uk>




Things to do

        Does not handle header tags (They should be displayed differently at the
        start of the html output - we probably dont want version tags etc)
        Should allow arguments to select tags which will not be displayed,
        and much more




*/

#include <stdio.h>
#include <stdlib.h>

#include "tagtypes.h"


void parseargs(int argc,char *argv[]);

void puthtmlrec(FILE *f, struct tagrecord *rec);

int main(int argc, char *argv[]) {

 int recbufsize=10000;        /* Size of a record buffer */
 struct tagrecord *record;
 struct tagrecord *header;
 char *fieldbuffer;
 char EventsDir[40];



parseargs(argc,argv);

opentagdefaults();
gettagdefval("Events-dir",EventsDir);


closetagdefaults();


fieldbuffer=malloc(recbufsize);

header=gettagrec(stdin, fieldbuffer);
freetagrec(header);  /* we dont use it at all at the moment */

fprintf(stdout,"<dl>\n");
while (!feof(stdin)) {
  record=gettagrec(stdin,fieldbuffer);
  puthtmlrec(stdout,record);
  freetagrec(record);
 }
fprintf(stdout,"</dl>\n");

return 0;
}

void parseargs(int argc,char *argv[])
{


}


void puthtmlrec(FILE *f, struct tagrecord *rec)
{
 struct tagrecord *current;

current=rec;
if (current==0) return;
/*
  here we could write out a title taken from the command line, and maybe
other bits of info

If we get really flash then we could read the input twice (save to /tmp
if it was being read from stdin) and then generate an index.
- alternatively we could read the whole file into memory before we start
to generate output - it is probably best to leave all such fiddles as
options
*/

while (current!=0) {
 if (strcmp(current->tagname,"Date")==0) {
   fprintf(f,"<dt>%s<dd>",current->value);
   }

 if (strcmp(current->tagname,"Title") ==0) {
   fprintf(f,"%s\n", current->value);
   }
  current=current->next;
 }
}

