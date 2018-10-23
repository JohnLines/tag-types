VERSION_MAJOR=0
VERSION_MINOR=0
VERSION_PATCH=9
VERSION=$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

CC=gcc
CDEBUG= -g -O -Wall
#  the -s is the GNU strip binaries option
INSTALL=install -s
INSTALL_MAN=install -m 644
INSTALL_DSC=install -m 644
PREFIX=/usr/local
INSTDIR=${PREFIX}/bin
MANDIR=${PREFIX}/man
DSCDIR=${PREFIX}/lib/dsc

all : tagextract tag2html tag2tab tab2tag tagshow tagcount tag2qif qif2tag tagcheck tagdiff tagsort

tagextract : tagextract.c taglib.o
	$(CC) ${CDEBUG} -o tagextract tagextract.c taglib.o

tag2html : tag2html.c taglib.o
	$(CC) ${CDEBUG} -o tag2html tag2html.c taglib.o

event2html : event2html.c taglib.o
	$(CC) ${CDEBUG} -o event2html event2html.c taglib.o

tag2tab : tag2tab.c taglib.o
	$(CC) ${CDEBUG} -o tag2tab tag2tab.c taglib.o

tab2tag : tab2tag.c taglib.o
	$(CC) ${CDEBUG} -o tab2tag tab2tag.c taglib.o

tagreplace : tagreplace.c taglib.o
	$(CC) ${CDEBUG} -o tagreplace tagreplace.c taglib.o

tagshow : tagshow.c taglib.o
	$(CC) ${CDEBUG} -o tagshow tagshow.c taglib.o

tagcount : tagcount.c taglib.o
	$(CC) ${CDEBUG} -o tagcount tagcount.c taglib.o

tag2qif : tag2qif.c taglib.o
	$(CC) ${CDEBUG} -o tag2qif tag2qif.c taglib.o

qif2tag : qif2tag.c taglib.o
	$(CC) ${CDEBUG} -o qif2tag qif2tag.c taglib.o

tagcheck : tagcheck.c taglib.o
	$(CC) ${CDEBUG} -o tagcheck tagcheck.c taglib.o

tagdiff : tagdiff.c taglib.o
	$(CC) ${CDEBUG} -o tagdiff tagdiff.c taglib.o

tagsort : tagsort.c taglib.o
	$(CC) ${CDEBUG} -o tagsort tagsort.c taglib.o

taglib.o : taglib.c tagtypes.h
	$(CC) ${CDEBUG} -c taglib.c

install : tagextract tag2html tag2tab tab2tag tagshow tagcount tag2qif qif2tag tagcheck tagdiff tagsort
	$(INSTALL) tagextract ${INSTDIR}
	$(INSTALL) tag2html ${INSTDIR}
#	$(INSTALL) event2html ${INSTDIR}
	$(INSTALL) tag2tab ${INSTDIR}
	$(INSTALL) tab2tag ${INSTDIR}
#	$(INSTALL) tagreplace ${INSTDIR}
	$(INSTALL) tagshow ${INSTDIR}
	$(INSTALL) tagcount ${INSTDIR}
	$(INSTALL) tag2qif ${INSTDIR}
	$(INSTALL) qif2tag ${INSTDIR}
	$(INSTALL) tagcheck ${INSTDIR}
	$(INSTALL) tagdiff ${INSTDIR}
	$(INSTALL_MAN) tag.5 ${MANDIR}/man5
	$(INSTALL_MAN) tagprogs.1 ${MANDIR}/man1
	$(INSTALL_MAN) tagextract.1 ${MANDIR}/man1
	$(INSTALL_MAN) tab2tag.1 ${MANDIR}/man1
	$(INSTALL_MAN) tag2tab.1 ${MANDIR}/man1
	$(INSTALL_MAN) tag2html.1 ${MANDIR}/man1
	$(INSTALL_MAN) tagcount.1 ${MANDIR}/man1
	$(INSTALL_MAN) qif2tag.1 ${MANDIR}/man1
	$(INSTALL_MAN) tag2qif.1 ${MANDIR}/man1
	$(INSTALL_MAN) tagshow.1 ${MANDIR}/man1
	$(INSTALL_MAN) tagcheck.1 ${MANDIR}/man1
	$(INSTALL_MAN) tagdiff.1 ${MANDIR}/man1
	$(INSTALL_MAN) tagsort.1 ${MANDIR}/man1
	$(INSTALL_DSC) dsc/action.dsc ${DSCDIR}
	$(INSTALL_DSC) dsc/biblio.dsc ${DSCDIR}
	$(INSTALL_DSC) dsc/bicycle.dsc ${DSCDIR}
	$(INSTALL_DSC) dsc/dsc.dsc ${DSCDIR}
	$(INSTALL_DSC) dsc/event.dsc ${DSCDIR}
	$(INSTALL_DSC) dsc/fintrans.dsc ${DSCDIR}
	$(INSTALL_DSC) dsc/geneology.dsc ${DSCDIR}
	$(INSTALL_DSC) dsc/inventory.dsc ${DSCDIR}
	$(INSTALL_DSC) dsc/isp.dsc ${DSCDIR}
	$(INSTALL_DSC) dsc/magindex.dsc ${DSCDIR}
	$(INSTALL_DSC) dsc/mimetypes.dsc ${DSCDIR}
	$(INSTALL_DSC) dsc/package.dsc ${DSCDIR}
	$(INSTALL_DSC) dsc/rfc.dsc ${DSCDIR}


clean:
	rm -f `find . \( -name "*.o" -o -name "*[~%]" \) -print` core

distclean: clean
	rm -f tagextract tag2html event2html tag2tab tab2tag tagreplace tagshow tagcount tag2qif qif2tag tagcheck tagdiff tagsort

maketar: distclean
	tar -czv -f ../tag-types-${VERSION}.tar.gz -C .. tag-types-${VERSION}

