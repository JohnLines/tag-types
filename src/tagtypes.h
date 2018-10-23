/*
	tagtypes.h

	structures for the internal format of a tagged file

*/

#include <stdio.h>

/* A tag record is a linked list of tag items */
struct tagrecord {
  char *tagname;
  char *value;
  char *delimiter;
  struct tagrecord *next;		/* pointer to the next item */
};

struct tagfile {
	FILE *f;
        int currentline;
	int error;
	char *fieldbuffer;
	int maxfieldsize;
	/* this is the maximum size we want fieldbuffer to be */
	int defaultfieldbuffersize;  /* default field buffer size */
	int currentfieldbuffersize; /* internally used */
	/* want something to hold index entries when I know what they
		should be like
	*/
};

/* taglib error returns in the error field are:
	0 - no error
	1 - End of line in tag (i.e. no colon in what should be a tag label)
	2 - Tag label longer than 127 characters
	3 - Malloc failed (out of memory)
	4 - Field data larger than maxfieldsize

*/

/* convert a tag error into a message */
char * tagerr2str(int error);

/* checktagresult (could be a macro ???) (or inline function ??) */
void checktagresult (char *progname, struct tagrecord *rec, struct tagfile *f);

struct tagrecord *gettagrec(struct tagfile *f);
void freetagrec(struct tagrecord *rec);
void puttagrec(struct tagfile *f, struct tagrecord *rec);

struct tagfile *opentagfile(const char *path, const char *mode);
struct tagfile *opentagfilestdin();
struct tagfile *opentagfilestdout();


int closetagfile (struct tagfile *stream);


int taggetvalue(char *value, const struct tagrecord *rec, const char *fieldname, const int maxlen);

int opentagdefaults();
int gettagdefval(const char *name, char *value);
int closetagdefaults();

struct tagfile *opentagdesc(const struct tagrecord* headerrec);
/* Open the tag descriptor file associated with the header record, or return
NIL if it can not be opened.
*/


