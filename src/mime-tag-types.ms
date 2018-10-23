.pl 10.0i
.po 0
.ll 7.2i
.lt 7.2i
.nr LL 7.2i
.nr LT 7.2i
.ds LF Lines
.ds RF FORMFEED[Page %]
.ds CF
.ds LH RFC xxxx
.ds RH 1 March 1996
.ds CH The Tag MIME type
.hy 0
.ad l
.in 0
Network Working Group                                           J. Lines
Request for Comments: xxxx                                              
                                                            1 March 1996


.ce
A MIME content type for 'tagged' partially structured data

.ti 0
Status of this Memo

.fi
.in 3
This memo describes an experimental protocol for the Internet community.
Discussion and suggestions for improvement are welcomed.
Distribution of this memo is unlimited.

.ti 0
Overview and Rationale

The existing MIME content types provide for the transfer of information
intended to be read by a human being, or viewed as an image, or played
as a sound, and while information targeted at a specific application
is conveyed by the application types.

This document describes a system for organising information as a series
of groups of records where each record consists of an identifying tag,
followed by the data for that record. This document does not describe
the valid tag values, but provides an overall framework for creation of
different tag types.

There are many pieces of information which are carried around the
Internet at present which are often manually entered into applications
due to the absence of standard ways of exchanging such information in
an application-neutral way.

For example if I am part of a group of Sun users who wish to be informed
that a meeting will take place at some particular time then a specially
formatted mail message will enable recipients of the message to put
the information directly into Sun calendar manager.  Many LAN based mail
systems have similar facilities, provided everyone is using the same
set of products.

At present information about events is available across the Internet in
mailing lists, News groups and Web pages but has to be transposed by
hand into electronic diaries.

Similarly a MIME type to describe the actions put on the attendees of a
meeting which could be attached to the minutes and read into their
todo lists would save much unnecessary retyping.

Events, actions and much more can be exchanged using the tag type system
described below.

.ti 0
Design considerations

In order to be useful across the wide range of systems used on the Internet
the information within a tagged type should be comprehensible to someone
with no specialised reader software, in much the same way as a MIME
text/plain message does not require a specialised mail reader.

It should be possible to create the information with a normal text editor
so that specialist software is not required for creating information.

There should be few restrictions on the actual data which may be transfered.
In particular it is desireable to be able to preserve the formatting of
a block of text, rather than have it treated as a string of words.



.ti 0
Tag stream format

A tag stream looks like a series of items of the form
.nf
Tagname: value
or
Tagname:: End_label
several lines
of text
End_label
.f

In the first format the data associated with the tag is the contents of
the line following the colon, except that a single leading space is
not considered part of the data. (For particular applications leading
spaces may not be important in any case).

In the second format the data is all the lines down to, but not including
the End_label. Any non-whitespace printable ASCII string may be used for
the End_label, but the use of a single period is not recommended.

The first tag in the file will be of the form
.nf
Tag-tagtype-version: n.nn



.ti 0
Tag type registration

The formal description of a particular tag type is contained in a file
which is itself in tag format. The MIME type tag/dsc is used to exchange
tag file descriptors. New tag descriptions or updated versions should be
registered according to the procedures described in RFC1590.


.ti 0
Discussion

The machine readable definition of tag types enables suites of programs
to be created which can process tag streams without needing to understand
the specific application they are used for. They can be used to filter
attachments before they are fed into an application, or presented to the
reader.

.ti 0
Security Considerations

.in 3
The ability to exchange data between applications through an open specification
should improve security overall. 

Where any tag type contains data which may carry its own security risks
that should be discussed in the information which relates to the particular
tag type.


.ti 0
Author's Address

.nf
John Lines
52, North Hinksey Lane,
Oxford,
OX2 0LY

Phone: +44 1865 721294

Email: john+tag-mime-type@paladyn.org
