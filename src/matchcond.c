
#include <sys/types.h>


/*
	tagtypes.h

	structures for the internal format of a tagged file

*/


/* A tag record is a linked list of tag items */
struct tagrecord {
  char *tagname;
  char *value;
  char *delimiter;
  struct tagrecord *next;		/* pointer to the next item */
};

struct tagrecord *gettagrec(FILE *f,char *fieldbuffer);
void puttagrec(FILE *f, struct tagrecord *rec);

int taggetvalue(char *value, const struct tagrecord *rec, const char *fieldname, const int maxlen);

int opentagdefaults();
int gettagdefval(const char *name, char *value);
int closetagdefaults();



#define MAXCONDS 5		/* Maximum number of conditions */

struct condition {
  char fieldname[40];
  char value[40];
  char op;
}; 


int matchcond(struct tagrecord *record,struct condition conditions[])
{
struct tagrecord *current;
int fieldval,condval;


current=record;
while(current!=0) {

/*
if (debug) {
 fprintf(stderr,"Comparing field name %s value %s with %s value %s\n",
    current->tagname,current->value,conditions[0].fieldname,
    conditions[0].value);
    }
*/

 condval=strtol(conditions[0].value,NULL,10);
 fieldval=strtol(current->value,NULL,10);
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

