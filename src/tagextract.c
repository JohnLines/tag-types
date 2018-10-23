/*
	tagextract - extract tag records which match the criteria
	given on the command line from stdin and write them to stdout.

	Note that if we cant work out what the which is the primary
	tag, and the first record does not meet the criteria then
	we may lose the headers.


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
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "tagtypes.h"

#define MAXCONDS 5		/* Maximum number of conditions */
#define CONDNAMELEN 40		/* Max length of a fieldname in a condition */
#define CONDVALLEN 40		/* Max length of a field value in a condition */

struct condition {
  char fieldname[CONDNAMELEN];
  char value[CONDVALLEN];
  char op;
}; 

/*
	Supported values of op
	=	Numerically equal
	E	String equal

*/

 int debug=0;
 int invert=0;
 int doheader=1;
 int startrecord=-1;         /* start looking at records starting here */
 int stoprecord=-1;

int parseargs(int argc,char *argv[], struct condition conditions[MAXCONDS],
	struct tagfile *infile, struct tagfile *outfile);
int matchcond(struct tagrecord *record,struct condition conditions[]);
int isodatecmp(const char* date1,const char* date2);
int isodate2tm(const char* date, struct tm* tm1);

int main(int argc, char *argv[]) {
 struct tagfile *infile;      /* was FILE *infile=stdin */
 struct tagfile *outfile;

 struct condition conditions[MAXCONDS];
 struct tagrecord *record;
 int recordnumber=0;

infile=opentagfilestdin();
outfile=opentagfilestdout();

if (parseargs(argc,argv,conditions,infile,outfile)) return 1;


if (doheader) {
    record=gettagrec(infile);
    checktagresult("tagextract",record,infile);
    puttagrec(outfile,record);
    freetagrec(record);
    } else  recordnumber++ ; 		/* first 'real' record is always 1 */


while (!feof(infile->f)) {
  record=gettagrec(infile);
  checktagresult("tagextract",record,infile);
  recordnumber++;
  if (debug) fprintf(stderr,"Testing record %d\n",recordnumber);
  if ((startrecord <0) || (recordnumber>= startrecord)) {
    if (debug) fprintf(stderr,"Record OK on start recordnumber\n");
     if ((stoprecord <0) || (recordnumber <=stoprecord))  {
    if (debug) fprintf(stderr,"Record OK on stop recordnumber\n");
  if (matchcond(record,conditions)^invert) puttagrec(outfile,record);
  }
  }
  freetagrec(record);
}
return 0;
}

int parseargs(int argc,char *argv[], struct condition conditions[MAXCONDS],struct tagfile *infile, struct tagfile *outfile)
{
 int i;
 int arg=1;
 int cond=0;
 for (i=0;i<MAXCONDS ; i++) conditions[i].op = 0;
/*  puts("condition ops set\n"); */

/* look for flags */

while ((arg<argc) && (argv[arg][0]=='-')) {
 switch (argv[arg][1]) {
case 'v':
  invert=1;
  if (debug) fprintf(stderr,"Invert set\n");
  break;
case 'h':
  doheader=0; break;
case 'd':
  debug=1; break;
case 'n':
  if (debug) fprintf(stderr,"Numeric select\n");
  arg++;
  conditions[0].op='A';
  startrecord=strtol(argv[arg],NULL,10); 
  stoprecord=startrecord;
  break;
case 'm':
  if (debug) fprintf(stderr,"Match conditions flag seen\n");
arg++;
/* while (arg<argc) { */
 strncpy(conditions[cond].fieldname,argv[arg],CONDNAMELEN); arg++;
 if (arg>=argc) {
     fprintf(stderr,"Error - operator expected\n"); return 1;
     }
 if (strcmp(argv[arg],"-eq")==0) conditions[cond].op='E';
 else if (strcmp(argv[arg],"==")==0) conditions[cond].op='=';
  else if (strcmp(argv[arg],"-gt")==0) conditions[cond].op='>';
  else if (strcmp(argv[arg],"-lt")==0) conditions[cond].op='<';
  else if (strcmp(argv[arg],"-contains")==0) conditions[cond].op='C';
  else if (strcmp(argv[arg],"-exists")==0) conditions[cond].op='X';
  else if (strcmp(argv[arg],"-before")==0) conditions[cond].op='B';
  else if (strcmp(argv[arg],"-earlier")==0) conditions[cond].op='e';
  else if (strcmp(argv[arg],"-later")==0) conditions[cond].op='l';
  else {
     printf("Unknown condition operator %s\n",argv[arg]);
     }
 if (debug) fprintf(stderr,"Conditions operator is %s\n",argv[arg]);
 if (conditions[cond].op!='X') {
 arg++;
 if (arg>=argc) {
     fprintf(stderr,"Error - value expected\n"); return 1;
     }
}
  strncpy(conditions[cond].value,argv[arg],CONDVALLEN); arg++;
 if ((conditions[cond].op=='=')||(conditions[cond].op=='>')) {
   /* The argument to these operators should be an integer */
 for (i=0 ; i<strlen(conditions[cond].value) ; i++) {
   if (!isdigit(conditions[cond].value[i])) {
      fprintf(stderr,"Error - argument must be an integer\n"); return 1;
      }
   } 


  }

/* } */
break;
default:
  fprintf(stderr,"Tagextract - unknown switch %c\n",argv[arg][1]);
}
arg++;
}
/* after the switches we should have an input file and an output file, if supplied */

if (arg<argc) {
  if (debug) fprintf(stderr,"Opening input file %s\n",argv[arg]);
  if ((infile=opentagfile(argv[arg],"r"))==NULL) {
       fprintf(stderr,"Error opening %s for input\n",argv[arg]);
       exit(0);
       }

  arg++;
  }

if (arg<argc) {
  if (debug) fprintf(stderr,"Opening output file %s\n",argv[arg]);
  if ((outfile=opentagfile(argv[arg],"w"))==NULL) {
	fprintf(stderr,"Error opening %s for output\n",argv[arg]);
	exit(0);
	}
  arg++;
  }
return 0;
}


int matchcond(struct tagrecord *record,struct condition conditions[])
{
struct tagrecord *current;
int fieldval,condval;


current=record;
while(current!=0) {

if (debug) {
 fprintf(stderr,"Comparing field name %s value %s with %s value %s\n",
    current->tagname,current->value,conditions[0].fieldname,
    conditions[0].value);
    }

 condval=strtol(conditions[0].value,NULL,10);
 fieldval=strtol(current->value,NULL,10);
if (debug)  fprintf(stderr,"condition value %d - field value %d\n",condval,fieldval);
 switch(conditions[0].op) {
 case '\0':
      break;    /* i.e. no comparison operator */
 case 'A':
		/* always matches */
	return 1;
	break;
 case 'N':
		/* never match */
	return 0;
	break;
 case 'E':
 if ((strcmp(current->tagname,conditions[0].fieldname)==0) && 
       (strcmp(current->value,conditions[0].value)==0)) return 1; 
	break;
 case '=':
 if ((strcmp(current->tagname,conditions[0].fieldname)==0) &&
       (condval==fieldval)) return 1;
	break;
 case '>':
 if ((strcmp(current->tagname,conditions[0].fieldname)==0) &&
       (fieldval > condval)) return 1;
	break;
 case '<':
 if ((strcmp(current->tagname,conditions[0].fieldname)==0) &&
       (fieldval < condval)) return 1;
	break;
 case 'C':
 if ((strcmp(current->tagname,conditions[0].fieldname)==0) &&
	(strstr(current->value,conditions[0].value)!=0)) return 1;
	break;
 case 'B':
 if ((strcmp(current->tagname,conditions[0].fieldname)==0) && 
       (strcmp(current->value,conditions[0].value)==0)) {
	   conditions[0].op='N';
           return 1; 
           }
	break;
 case 'l':
 if ((strcmp(current->tagname,conditions[0].fieldname)==0) &&
       (isodatecmp(current->value,conditions[0].value)==1)) return 1;
        break;
 case 'e':
 if ((strcmp(current->tagname,conditions[0].fieldname)==0) &&
       (isodatecmp(current->value,conditions[0].value)==-1)) return 1;
        break;
 case 'X':
 if (strcmp(current->tagname,conditions[0].fieldname)==0) return 1;
        break;
  default:
     printf("Error - invalid comparison operator\n");
 }


  current=current->next;
 }
return 0;
}

int isodatecmp(const char* date1,const char* date2) {

struct tm tm1,tm2;
/* double timediff; */
int i;


if ((i=isodate2tm(date1,&tm1))) fprintf(stderr,"isodate2tm date1 returns %d\n",i);
if ((i=isodate2tm(date2,&tm2))) fprintf(stderr,"isodate2tm date2 returns %d\n",i);
/* there is no timediff on sunos or solaris - I will do the compare
  directly within the time structure - this is probably better anyway
timediff=difftime(mktime(&tm1),mktime(&tm2));
*/
/*
fprintf(stderr,"isodatecmp - date1 is %s\n", asctime(&tm1));
fprintf(stderr,"isodatecmp - date2 is %s\n", asctime(&tm2));
*/

if (tm1.tm_year > tm2.tm_year) return 1;
else if (tm1.tm_year < tm2.tm_year) return -1;
else if (tm1.tm_mon > tm2.tm_mon) return 1;
else if (tm1.tm_mon < tm2.tm_mon) return -1;
else if (tm1.tm_mday > tm2.tm_mday) return 1;
else if (tm1.tm_mday < tm2.tm_mday) return -1;



else return 0;

/*
fprintf(stderr,"isodatecmp - date1 is %s\n", asctime(&tm1));
fprintf(stderr,"isodatecmp - timediff is %e\n",timediff);
*/

/* if (timediff >0) return 1;
else if (timediff <0 ) return -1;
else return 0;
*/

}


int isodate2tm(const char* date, struct tm* tm1) {

int i;
char* p1;
char* p2;

p1=(char *)date;

/* initialise the return results to sensible (if not very useful) values */

tm1->tm_year=0;  /* 1900 !! */
tm1->tm_mon=0;   /* January */
tm1->tm_mday=0;  /* - this is an indicator that it has not been set up - not
		    in the range 1-31 */
tm1->tm_wday=0;  /* always Sunday !! */
tm1->tm_yday=0;  /* always Jan 1st as a day of the year */
tm1->tm_isdst=-1; /* we dont know if it it Daylight Savings Time */

tm1->tm_hour=0;
tm1->tm_min=0;
tm1->tm_sec=0;

/* get the year */
/* we now use the strptime function instead */

/* strptime(p1,"%Y-%m-%d %r",&tm1); */




i=(int)strtol(p1,&p2,10);

if (*p2=='-') {
tm1->tm_year=i-1900;
p1=p2;
p1++;
}
else return 1;


i=(int)strtol(p1,&p2,10);
if (*p2=='-') {
tm1->tm_mon=i-1;
p1=p2;
p1++;
}
else return 2;


i=(int)strtol(p1,&p2,10);
if ((*p2==' ') ||(*p2=='\0')) {
tm1->tm_mday=i;
p1=p2;
p1++;
}
else return 3;


/* not attempting to decode time yet */

return 0;

}



