
/*
	tag2html - Convert a tag file to html

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
#define CONDITIONSTRINGLEN 127
#include "tagtypes.h"

char version[6]="0.04";

/* Global flags */
int HttpHeader=0;		/* If set then we output an http header */
int doheader=1;
#define list 1
#define table 2
#define form 3
int htmlmode=list;
int numtaglabels=0;
int numtagheaders=0;
int rownumber=0;
int singlerecord=0;
char taglabels[MAXTAGLABELS][MAXLABELLEN];
char tagheaders[MAXTAGLABELS][MAXLABELLEN];
char columntype[MAXTAGLABELS]; /* t = tag value, l=literal, u=url */
char tableoptions[TABLEOPTIONSLEN];
char conditionstring[CONDITIONSTRINGLEN];



int parseargs(int argc,char *argv[], struct tagfile *infile, FILE *outfile);

void puthtmlrecl(FILE *f, struct tagrecord *rec);
void puthtmlrect(FILE *f, struct tagrecord *rec);
void puttableheader(FILE *f);
int selectrecord(struct tagrecord *record);

int main(int argc, char *argv[]) {

struct tagfile *infile;
FILE *outfile;

 struct tagrecord *record;

infile=opentagfilestdin();
outfile=stdout;

if (parseargs(argc,argv,infile,outfile)) return 1;

if (HttpHeader) {
  fprintf(outfile,"Content-type: text/html\n\n");

/* we will also put out a web page header */
 fprintf(outfile,"<HEAD>\n<META NAME=GENERATOR CONTENT=\"tag2html ");
 fprintf(outfile,"V%s\">\n</HEAD>\n",version);
}


if (doheader) {
    record=gettagrec(infile);
    checktagresult("Tag2html",record,infile);
    freetagrec(record);
    /* we dont do anything with the header - just throw it away */
    } 

if (htmlmode==table) {
  puttableheader(outfile);
}

while (!feof(infile->f)) {
  record=gettagrec(infile);
  checktagresult("Tag2html",record,infile);
  rownumber++;
  switch (htmlmode) {
case list:
 if ((!singlerecord) || (selectrecord(record))) {
  puthtmlrecl(outfile,record);

  }
  break;
case table:
  puthtmlrect(outfile,record);
  break;

default:
  fprintf(stderr,"Tag2html - unknown html mode %d\n",htmlmode);
}
freetagrec(record);
 }

if (htmlmode==table) {
 fprintf(outfile,"</table>\n");
}

return 0;
}

int parseargs(int argc,char *argv[],struct tagfile *infile, FILE *outfile)
{

int arg=1;

tableoptions[0]=0;

/* Look for flags */

while ((arg<argc) && (argv[arg][0]=='-')) {
 switch (argv[arg][1]) {
case 'h':
  doheader=0; arg++; break;
case 'H':
  HttpHeader=1; arg++; break;
case 't':
  htmlmode=table;
  arg++;
  while ((arg<argc) && (argv[arg][0]!='-')) {
  columntype[numtaglabels]='t';
  strncpy(taglabels[numtaglabels],argv[arg],MAXLABELLEN); numtaglabels++;
  strncpy(tagheaders[numtagheaders],argv[arg],MAXLABELLEN); numtagheaders++;
  if (argv[arg][0]!='-') arg++;
  }
  break;
case 'l':
  htmlmode=table;
  arg++;
  while ((arg<argc) && (argv[arg][0]!='-')) {
  columntype[numtaglabels]='l';
  strncpy(taglabels[numtaglabels],argv[arg],MAXLABELLEN); numtaglabels++;
  if (argv[arg][0]!='-') arg++;
  }
  break;
case 'u':
  arg++;
  while ((arg<argc) && (argv[arg][0]!='-')) {
  columntype[numtaglabels]='u';
  strncpy(taglabels[numtaglabels],argv[arg],MAXLABELLEN); numtaglabels++;
  if (argv[arg][0]!='-') arg++;
  }
  break;
case 's':
  arg++;
  singlerecord=1;
  strncpy(conditionstring,argv[arg],CONDITIONSTRINGLEN); arg++;

  break;
case 'T':
  arg++;
  while ((arg<argc) && (argv[arg][0]!='-')) {
  strncpy(tagheaders[numtagheaders],argv[arg],MAXLABELLEN); numtagheaders++;
  if (argv[arg][0]!='-') arg++;
  }
  break;
case 'o':
  arg++;
  strncpy(tableoptions,argv[arg],TABLEOPTIONSLEN);
/*  printf("tableoptions set to %s\n",tableoptions); */
  arg++;
  break;
default:
   fprintf(stderr,"Tag2html - unknown switch %c\n",argv[arg][1]);
   arg++;
}
/* arg++; Now each argument is responsible for incrementing the arg pointer */
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


return 0;
}

void puttableheader(FILE *f)
{
int i;

fprintf(f,"<table %s>\n",tableoptions);

for (i=0; i<numtagheaders; i++) {
  fprintf(f,"<th>%s</th>",tagheaders[i]);
}
fprintf(f,"</tr>\n");
}


void puthtmlrecl(FILE *f, struct tagrecord *rec)
/* put out a tag record as an HTML definition list */
{
 struct tagrecord *current;
 int i,fieldprinted;

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

fprintf(f,"<dl>\n");
while (current!=0) {
 if (strcmp(current->tagname,"End")!=0) {
 fprintf(f,"<dt>%s<dd>",current->tagname);
 if (current->delimiter!=0) {
    /* We have a delimited set of text - try wrapping it in <pre></pre> */
   fprintf(f,"<pre>");
   fprintf(f,"%s\n",current->value);
   fprintf(f,"</pre>");
   }
   else
   {
/* If this field was flagged as -u on the command line then we will turn
  its value into a URL
*/
   fieldprinted=0;
   for (i=0;i<=numtaglabels;i++) {
       if ((strcmp(current->tagname,taglabels[i])==0) && (columntype[i]='u')) {
         fprintf(f,"<a href=%s>%s</a>\n",current->value,current->value);
         fieldprinted=1;
         break;
         }
        }
   if (!fieldprinted) fprintf(f,"%s\n",current->value);
   }
  }
  current=current->next;
 }
fprintf(f,"</dl>\n");
fprintf(f,"<hr>\n");      /* for now put a horizontal rule between items */
}

void puthtmlrect(FILE *f, struct tagrecord *rec)
/* put out a tag record as a row in a table */
{
 int i,j;
 char tmpstr[MAXLABELLEN];

if (rec==0) return;

fprintf(f,"<tr>");
/* work our way through the taglabels looking for ones which match */
for (i=0; i<numtaglabels ; i++) {
 switch (columntype[i]) {
case 't':
 j=taggetvalue(tmpstr,rec,taglabels[i],MAXLABELLEN);
 fprintf(f,"<td>%s</td>",tmpstr);
 break;
case 'l':
  {
 char lit_line[MAXLITLEN];
 int k;
 char tmpstr2[MAXLABELLEN];
 for (k=0 ; k <=MAXLITLEN ; k++ ) lit_line[k]=0;
  k=0;
  /* check for dollar marker */
  for (j=0 ; j < strlen(taglabels[i]) ; j++) {
    if (taglabels[i][j]=='$') {
       /* it is a special character */
         j++;
         switch(taglabels[i][j]) {
           case 'n': 
                    sprintf(tmpstr,"%d",rownumber); 
                    lit_line[k]=0;
                    strcat(lit_line,tmpstr); k=strlen(lit_line);
                    break;
           case 'v': /* pick up the field name - up to : */
                   {
		    int m;
                   m=0; j++; /* skip over 'v' */
                   for ( ; j<strlen(taglabels[i]) ; j++) {
                       if (taglabels[i][j]==':') break;
                       tmpstr[m]=taglabels[i][j] ; m++; 
                       }
                      tmpstr[m]=0;
                      m=taggetvalue(tmpstr2,rec,tmpstr,MAXLABELLEN);
                           strcat(lit_line,tmpstr2); k=strlen(lit_line);
                   break;
                   }
           case '$': lit_line[k]='$'; k++; break;
           default: break;
           }
       } else {
       lit_line[k]=taglabels[i][j]; k++;
       }
     }
  lit_line[k]=0;
  fprintf(f,"<td>%s</td>",lit_line);
 break;
 }
case 'u':
 j=taggetvalue(tmpstr,rec,taglabels[i],MAXLABELLEN);
/*  fprintf(f,"<td><a href=%s>%s</a></td>",taglabels[i],taglabels[i]); */
  fprintf(f,"<td><a href=%s>%s</a></td>",tmpstr,tmpstr);
  break;

default:
  fprintf(stderr,"Tag2html - internal error columntype[%d] is %c\n",i,columntype[i]);
}
}
fprintf(f,"</tr>\n");


}


int selectrecord(struct tagrecord *record)
{

fprintf(stderr,"selectrecord - conditionstring is %s",conditionstring);


return 0;
}

