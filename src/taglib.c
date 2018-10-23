/*
	Tag library routines

Note that these routines are ALPHA software, developed piecemeal while I
was testing various ideas.


They are released under the GNU Library Copyright.

	Author: John Lines <john@paladin.demon.co.uk>

*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tagtypes.h"


struct tagrecord *gettagrec(struct tagfile *f);
struct tagrecord *gettagrechdr(struct tagfile *f, char *Endlabel);
void puttagrec(struct tagfile *f, struct tagrecord *rec);

struct tagrecord *gettagrec(struct tagfile *f)
{
 return gettagrechdr(f,"End");
}



struct tagrecord *gettagrechdr(struct tagfile *f,char *Endlabel)
{
/* enum states= {tag,colon,colonspace,svalue,eoln,doublecolon};  */
#define initial 0
#define tag 1
#define colon 2
#define colonspace 3
#define svalue 4
#define eoln 5
#define doublecolon 6
#define dcspace 7
#define inendlabel 8
#define mlvalue 9		/* multiline value */
#define endlabeleoln 10
#define mleoln 11
#define mlmatch 12

 char ch;
 char tagname[128];
 char endlabel[128];
 int tagptr=0;
 int labelptr=0;
 int bufptr=0;
/* enum states*/   int state=initial;    /* start off as if we had just been at an end of line */
 struct tagrecord *current=0;
 struct tagrecord *previous=0;
 struct tagrecord *first=0;
 

/* check to see if we have a fieldbuffer allocated already */
if (f->fieldbuffer==NULL) {
 f->fieldbuffer=malloc(f->defaultfieldbuffersize);
 if (f->fieldbuffer == NULL) { f->error=3; return 0; }
 f->currentfieldbuffersize=f->defaultfieldbuffersize;
}

 /* get tag record items until we hit one where the tagname is End */

while (!feof(f->f)) {
  /* get items - starts to get tricky because we want to be able to get
   items with large blocks of text, so how do I allocate the memory ??
    - ah - I get it allocated by someone else
*/
 ch=fgetc(f->f);
 switch(state) {
case initial:
/* we are in this state when we have not yet read any input, or only blank lines */
   if (ch=='\n') { f->currentline++; }
   else {
     if (feof(f->f)) break;
     state=tag;
     tagname[0]=ch;
     tagptr=1;
     }
  break;
case tag:
       if (ch==':') {
           state=colon;
           tagname[tagptr]=0;
           bufptr=0;
         }
      else if (ch=='\n') {
/*         fprintf(stderr,"Taglib error - End of line in tag\n"); */
	f->error=1; 
         return 0;
      } else {
             tagname[tagptr]=ch;
   }
   if (tagptr >= 127) {
       /* We should never have a tag longer than 127 characters */
       /* return a null record */
/*       fprintf(stderr,"Taglib error - tagptr > 127\n"); */
       f->error=2;
       return 0;
       }

   tagptr++;
   break;
case colon:
   if (ch==':') { state=doublecolon ; }
   else if (ch==' ') { }
   else if (ch=='\n') {
      f->currentline++;
/* Null value, still need to write it out */
/* this handling of null svalues needs work */
    /* printf("Null string value\n"); */
/* repeat the same stuff we get for a '\n' if we were in an svalue - this needs
 a rewrite to sort this out
*/
      state=eoln;
      f->fieldbuffer[bufptr]=0;
       /* allocate a record structure for the fields */
         current= (struct tagrecord *)malloc(sizeof(struct tagrecord));
         if (previous!=0) previous->next=current;
         previous=current;
         current->tagname=malloc(strlen(tagname)+1);
         strcpy(current->tagname,tagname);
         current->value=malloc(strlen(f->fieldbuffer)+1);
           if (current->value == NULL) {
		f->error=3; return 0; }
         strcpy(current->value,f->fieldbuffer);
         current->delimiter=0;
         current->next=0;
         if (first==0) first=current;
       /* check to see if this is the end label */
    if (strcmp(tagname,Endlabel)==0) {
         /* tidy up our current tagrecord structure */
         if (current != 0) current->next=0;
	 f->error=0;
         return first;
         }
     }
   else {
      /* first character of the value */
      state=svalue;
      f->fieldbuffer[0]=ch;
      bufptr=1;
      }
   break;
case svalue:
   if (ch=='\n') {
      f->currentline++;
      state=eoln;
      f->fieldbuffer[bufptr]=0;
       /* allocate a record structure for the fields */
         current= (struct tagrecord *)malloc(sizeof(struct tagrecord));
         if (previous!=0) previous->next=current;
         previous=current;
         current->tagname=malloc(strlen(tagname)+1);
         strcpy(current->tagname,tagname);
         current->value=malloc(strlen(f->fieldbuffer)+1);
         strcpy(current->value,f->fieldbuffer);
         current->delimiter=0;
         current->next=0;
         if (first==0) first=current;
       /* check to see if this is the end label */
    if (strcmp(tagname,Endlabel)==0) {
         /* tidy up our current tagrecord structure */
         if (current != 0) current->next=0;
	 f->error=0;
         return first;
         }
   } else {
      f->fieldbuffer[bufptr]=ch;
      bufptr++;
      if (bufptr>=f->currentfieldbuffersize) { f->error=4; return 0; };
   }
  break;
case eoln:
   if (ch=='\n') { f->currentline++; }
   else {
     state=tag;
     tagname[0]=ch;
     tagptr=1;
     }
  break;
case doublecolon:
   if (ch==' ') { /* state=dcspace */ }
   else {
      /* first character of the value */
      state=inendlabel;
      endlabel[0]=ch;
      labelptr=1;
      }
  break;
case dcspace:
/* we should never reach here */
   if (ch==' ') { }
   else {
      /* first character of the value */
      state=inendlabel;
      endlabel[0]=ch;
      labelptr=1;
      }
   break;
case inendlabel:
   if (ch=='\n') {
            f->currentline++;
            state=endlabeleoln;
            endlabel[labelptr]=0;
 }
   else {
       endlabel[labelptr]=ch;
       labelptr++;
   }
break;
case endlabeleoln:
     f->fieldbuffer[0]=ch;
     bufptr=1;
     labelptr=0;            /* We are reusing this to indicate how far we
			have got in matching the current label */
/*      fprintf(stderr,"State endlabeleoln ch=%c*\n",ch); */

     if (ch==endlabel[0]) {state=mlmatch; labelptr++;}
          else  state=mlvalue ;
  break;
case mlmatch:
/* We are in a multi line value part, and we match the end label so far */
/*      fprintf(stderr,"State mlmatch ch=%c*\n",ch); */
    if (ch=='\n')  {
       f->currentline++;
       /* End of line, did we match the end of string */
       if (endlabel[labelptr]==0) { 
           /* Yes we did */
           /* take the end label out of fieldbuffer, by putting in a null */
           f->fieldbuffer[bufptr-strlen(endlabel)]=0;
           current= (struct tagrecord *)malloc(sizeof(struct tagrecord));
         if (previous!=0) previous->next=current;
         previous=current;
         current->tagname=malloc(strlen(tagname)+1);
         strcpy(current->tagname,tagname);
         current->value=malloc(strlen(f->fieldbuffer)+1);
         strcpy(current->value,f->fieldbuffer);
         current->delimiter=malloc(strlen(endlabel)+1);
         strcpy(current->delimiter,endlabel);
         current->next=0;
         if (first==0) first=current;
	state=eoln;

        } else {
        /* no we didnt */
        labelptr=0;
        f->fieldbuffer[bufptr]=ch;
        bufptr++;
      if (bufptr>=f->currentfieldbuffersize) { f->error=4; return 0; };
        }
     }
     else {
    /* not end of line */
    f->fieldbuffer[bufptr]=ch;
    bufptr++;
    if (bufptr>=f->currentfieldbuffersize) { f->error=4; return 0;};
    if (ch==endlabel[labelptr]) state=mlmatch; else state=mlvalue;
    labelptr++;
    }
   break;
case mlvalue:
/* we are in a multiline value part, and we dont match the end label */
  if (ch=='\n') {
     f->currentline++;
     labelptr=0; state=mlmatch;
     }
   f->fieldbuffer[bufptr]=ch;
   bufptr++;
    if (bufptr>=f->currentfieldbuffersize) { f->error=4; return 0; };
   break;


}  /* end of the case */

}
/* if we get here then we hit an end of file */
/* if (state != initial) fprintf(stderr,"Taglib - end of file while state is %d\n",state); */
if (state != initial) f->error=10+state;
return 0;
}

void freetagrec(struct tagrecord *rec)
/* Note that this should be called pointing at the first tag record in a list,
and it will free that record and all the ones which it points to.
*/
{
struct tagrecord *current;

current=rec;
if (current ==0 ) return;

/* are we the last tagrecord in a chain */
if (current->next == 0) {
  free(current);
}
 else
  {
   freetagrec(current->next);
  }

}

void puttagrec(struct tagfile *f, struct tagrecord *rec)
{
 struct tagrecord *current;
 int endseen=0;

current=rec;

while (current!=0) {
  if (strcmp(current->tagname,"End")==0) endseen=1;
  fprintf(f->f,"%s:",current->tagname);
  if (current->delimiter!=0) {
          fprintf(f->f,": %s\n",current->delimiter);
          }
     else
         {
          fprintf(f->f," ");
         };
  if (current->value!=0) fprintf(f->f,"%s",current->value);
  if (current->delimiter==0) fprintf(f->f,"\n"); 
  if (current->delimiter!=0) fprintf(f->f,"%s\n",current->delimiter);
  current=current->next;
  }
if ((!endseen)&& (current!=0)) fprintf(f->f,"End:\n");
}


struct tagfile *opentagfile(const char *path, const char *mode)
{
 struct tagfile *t;

 t= (struct tagfile *)malloc(sizeof(struct tagfile));

t->f=fopen(path,mode);
if ( t->f == NULL ) { free(t) ; return NULL; }
t->currentline=0;
t->error=0;
t->maxfieldsize=10000;
t->defaultfieldbuffersize=t->maxfieldsize;
t->currentfieldbuffersize=0;
t->fieldbuffer=NULL;
 return t;

}

struct tagfile *opentagfilestdin()
{
 struct tagfile *t;

 t= (struct tagfile *)malloc(sizeof(struct tagfile));

t->f=stdin;
t->currentline=0;
t->error=0;
t->maxfieldsize=10000;
t->defaultfieldbuffersize=t->maxfieldsize;
t->currentfieldbuffersize=0;
t->fieldbuffer=NULL;
return t;
}

struct tagfile *opentagfilestdout()
{
 struct tagfile *t;

 t= (struct tagfile *)malloc(sizeof(struct tagfile));

t->f=stdout;
t->currentline=0;
t->error=0;
return t;

}


int closetagfile (struct tagfile *stream)
{
int i;
free(stream->fieldbuffer);
i= fclose(stream->f);
free(stream);
return i;
}



int taggetvalue(char *value, const struct tagrecord *rec, const char *fieldname, const int maxlen)

{
 struct tagrecord *current;
value[0]=0;
current=(struct tagrecord *)rec;
while (current !=0) {
  if (strcmp(current->tagname,fieldname)==0) {
     strncpy(value,current->value,maxlen); return 1;
     }
current=current->next;
}
return 0;
}

char * tagerr2str(int error)
{
switch (error) {
 case 0: return "";
 case 1: return "end of line in tag (no colon where tag label expected";
 case 2: return "tag label longer than 127 characters";
 case 3: return "malloc failed - out of memory";
 case 4: return "field data larger than currentfieldbuffersize";

/* End of file errors */
 case 11: return "end of file in state 'tag'";
 case 12: return "end of file in state 'colon'";
 case 13: return "end of file in state 'colonspace'";
 case 14: return "end of file in state 'svalue'";
 case 15: return "end of file in state 'eoln'";
 case 16: return "end of file in state 'doublecolon'";
 case 17: return "end of file in state 'dcspace'";
 case 18: return "end of file in state 'inendlabel'";
 case 19: return "end of file in state 'mlvalue'";
 case 20: return "end of file in state 'endlabeleoln'";
 case 21: return "end of file in state 'mleoln'";
 case 22: return "end of file in state 'mlmatch'";

 default:
   return "unknown error returned by taglib";
}

return "should not be reached";

}

void checktagresult (char *progname, struct tagrecord *rec, struct tagfile *f)
{
 if ((rec==0) && (f->error !=0)) {
   fprintf(stderr,"%s - taglib error %d (%s) at line %d \n",progname,f->error,tagerr2str(f->error),f->currentline);
/* avoid reporting the same error twice */
   f->error=0;
 }

}


/* Opentagdefaults opens a tag defaults file, looking in a series of places
for it. Programs which use these taglib routines can use the tagdefaults
files to get various parameters.

The sequence is
	opentagdefaults
	gettagdefval(name, returned-value)
	...
	closetagdefaults

	Do the rest of the program

The tag defaults file is structured as a single tag record, with the field
names being the parameters we want to set.

*/

struct tagrecord *tagdefaultsrec=NULL;

int opentagdefaults ()
{
struct tagfile *tagdefaultsfile;

tagdefaultsfile=opentagfile(strcat(getenv("HOME"),".tagdefaults"),"r");
if (tagdefaultsfile==NULL) {
    tagdefaultsfile = opentagfile("/usr/lib/tag/tagdefaults","r");
   /* other possible values here */
    }
if (tagdefaultsfile==NULL) {
    return 0;
}

tagdefaultsrec=gettagrec(tagdefaultsfile);

 
closetagfile(tagdefaultsfile);
return 1;

}

int gettagdefval(const char *name, char *value)
{
struct tagrecord *current;

current=tagdefaultsrec;
while (current!=0) {
  if ( strcmp(current->tagname,name)) {
       strcpy(value,current->value);
       return 1;
       }
  current=current->next;
}
/* we didnt find anything - return a failure flag */
return 0;
}


int closetagdefaults ()
{

return 1;
}

struct tagfile *opentagdesc(const struct tagrecord *headerrec)
{
/* Open the tag descriptor file associated with the header record, or return
NIL if it can not be opened.
*/

char tagtype[255];
char filename[255];
char *versionindex=0;
struct tagfile *f;

if (headerrec==NULL) return NULL;
/* The first tagname in the header record should be of the form Tag-xxx-version
 if it is then we extract the xxx part, otherwise we give up */

/* fprintf(stderr,"tagname is %s\n", headerrec->tagname); */
/* we are interested in the last occurence of the version part */
/* for now we will just look for -version, i.e. if you have a tag type with
 -version in its name then this will not work - serves you right */

if ((strlen(headerrec->tagname)<4) || (strlen(headerrec->tagname)>250)) return NULL;

strncpy(tagtype,(headerrec->tagname)+4,255);
versionindex=strstr(tagtype,"-version");
versionindex[0]=0;


/* want a function to open a file found on a path - I am sure there must
be one - for now I will hard wire the directories to search */


strcpy(filename,"/usr/local/lib/tag/dsc/");
strncat(filename,tagtype,251-strlen(filename));
strcat(filename,".dsc");
if ( (f=opentagfile(filename,"r"))) return f;

strcpy(filename,"/usr/lib/tag/dsc/");
strncat(filename,tagtype,251-strlen(filename));
strcat(filename,".dsc");
if (( f=opentagfile(filename,"r"))) return f;

strcpy(filename,"./");
strncat(filename,tagtype,251-strlen(filename));
strcat(filename,".dsc");
if (( f=opentagfile(filename,"r"))) return f;

/* we return silently if we cant open the file
if ( f=opentagfile(filename,"r")) return f;
if (f==NULL) {
   fprintf(stderr,"unable to open %s\n",filename);
}
*/



return f;
}


