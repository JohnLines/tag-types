/*
  qif2tag.c - Convert a Quicken Interchange file to tag format

Released under the GNU Copyright

Author: John Lines <john@paladin.demon.co.uk>


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tagtypes.h"

#define MAXQBUFLEN 80
#define ISODATELEN 12

int recbufsize=10000;
int parseargs(int argc,char *argv[], FILE *infile, FILE *outfile);

void qifdate2isodate(char* qdate, char* isodate);

int main(int argc, char *argv[]) {

FILE *infile=stdin;
FILE *outfile=stdout;

/*  struct tagrecord *record; */
 char *fieldbuffer;
 char qbuf[MAXQBUFLEN];
 int qline=0;  /* line number in quicken file */
 int splitnum=1;
 int needId=0;
 int ignoring=0;
 int sequence=1;
 char idbase[24]="qif2tag";
 char isodate[ISODATELEN]="";
 char *p;
              
if (parseargs(argc,argv,infile,outfile)) return 1;

fieldbuffer=malloc(recbufsize);


/* Quicken headers always look like
!Type:Bank
!Type:Cash
etc. At present we can only deal with Bank files (and possibly others which
look very like them
*/

while (! feof(infile)) {
fgets(qbuf,MAXQBUFLEN,infile);
/* throw away a final carriage return if there is one */
 if ((p=strchr(qbuf,'\r'))) *p=0;

qline++;
if ((!ignoring)||(qbuf[0]=='!')) {   
if (needId) {
  fprintf(outfile,"Id: %s%d\n",idbase,sequence);
  sequence++;
  needId=0;
  }
switch (qbuf[0]){
case 'D':  /* Date */
qifdate2isodate(&qbuf[1],isodate);
fprintf(outfile,"Date: %s\n",isodate);
break;
case 'T':  /* Amount */
fprintf(outfile,"Amount: %s\n",&qbuf[1]);
break;
case 'C':  /* Cleared Status */
fprintf(outfile,"Cleared-Date: %s\n",isodate);
if (isodate[0]==0) fprintf(stderr,"Warning - Cleared date not known\n");
break;
case 'N':  /* Cheque or reference number */
fprintf(outfile,"Reference: %s\n",&qbuf[1]);
break;
case 'P':  /* Payee */
fprintf(outfile,"Payee: %s\n",&qbuf[1]);
break;
case 'M':  /* Memo */
fprintf(outfile,"Description: %s\n",&qbuf[1]);
break;
case 'L':  /* Catagory */
fprintf(outfile,"Catagory: %s\n",&qbuf[1]);
break;
case 'S':  /* Catagory in split */
fprintf(outfile,"Catagory.%d: %s\n",splitnum,&qbuf[1]);
break;
case 'E':  /* Memo in split  */
fprintf(outfile,"Description.%d: %s\n",splitnum,&qbuf[1]);
break;
case '$':   /* Amount in split  */
fprintf(outfile,"Subamount.%d: %s\n",splitnum,&qbuf[1]);
 splitnum++;      /* assume that $ will be the last item in the split */
break;
case 'K':  /* Memorized transaction type */

break;
case '!':   /* Options or similar record */
 if (strcmp(qbuf,"!Type:Cat")==0)  ignoring=1; 
 else if (strcmp(qbuf,"!Type:Memorized")==0) ignoring=1;
 else if (strcmp(qbuf,"!Type:Memorised")==0) ignoring=1;
 else if (strcmp(qbuf,"!Type:CCard")==0) { ignoring=0; needId=1; }
 else if (strcmp(qbuf,"!Type:Bank")==0) { ignoring=0; needId=1; }
 else if (strcmp(qbuf,"!Type:Bank ")==0) { ignoring=0; needId=1; }
 else if (strcmp(qbuf,"!Type:Cash")==0) { ignoring=0; needId=1; }
 else if (strcmp(qbuf,"!Type:Cash ")==0) { ignoring=0; needId=1; }


 else fprintf(stderr,"Unknown options record %s\n",qbuf);
break;
case '^':  /* End of record */
fprintf(outfile,"End:\n");
splitnum=1;
needId=1;
isodate[0]=0;
break;

default:
 fprintf(stderr,"Unknown entry in Quicken file line %d: %s\n",qline,qbuf); 
}

}
}
return 0;
}



int parseargs(int argc,char *argv[],FILE *infile,FILE *outfile)
{


return 0;
}

void qifdate2isodate(char* qdate, char* isodate){
int month,day,year=0;
/* simple version -assume American date format i.e. month/day/year (2 digit) */
sscanf(qdate,"%d/%d/%d",&month,&day,&year);

if (year<50) year=year+2000;
else if (year<1900) year=year+1900;

snprintf(isodate,ISODATELEN,"%.4d-%.2d-%.2d",year,month,day);


}

