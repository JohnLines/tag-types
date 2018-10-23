Tag-dsc-version: 1.0
Describes: rfc
Introduction:: END_I
Internet Requests for Comments (RFCs) provide information and standards
to the Internet community.

The rfc tag type gives a means of communicating information about an RFC
in a standard manner, allowing interchange of data between programs which
maintain databases of RFCs.

A text/prs.jlines.tag rfc MIME attachment to an RFC announcement would allow a receiving
mail agent to check whether a new RFC had keywords which related to an
area that site was particularly interested in, such as MIME, or SNMP
or to check if it obsoleted an RFC which was already held locally on
that site.

Questions such as 'show me all the security related RFCs issued this
year' could be answered by an RFC database, which could be built using
the same tools which manipulate any tag file.

The text/prs.jlines.tag rfc type could also be used for rfc draft information, and by
organisations which use an RFC like system for internal standards.
END_I
End:

Tagname: Rfc
Description: The RFC number, in the form RFC1234, or name of the internet draft
Type: Primary
End:

Tagname: Title
Description: The official title of the RFC
End:

Tagname: Alias
Description: The rfc may also be an FYI, or STD document
Type: Optional
End:

Tagname: Authors
Description:: END_D
There may be more than one author in this tag, seperated by commas
This could be used to have a type of killfile for RFCs - this may be of
use if the tag/rfc type was used to distribute Draft RFC information
END_D
End:

Tagname: Mailbox
Description: The email address(es) of the author(s)
End:

Tagname: Status
Description:: END_D
Internet RFCs have a status, which will be one of
	Informational
	Draft_Standard
	Experimental
	Proposed_Standard
	Standard
	Historic
END_D
End:

Tagname: Working-Group
Description:: END_D
If this Rfc is the product of an official IETF Working Group then this
tag contains the name of the Working Group.
END_D
End:

Tagname: Date
Description: The date the RFC was issued
End:

Tagname: Description
Description: A short summary of the contents of the RFC
End:

Tagname: Url
Description: A Uniform Resource Locator which can be used to access the Rfc
End:

Tagname: Obsoletes
End:

Tagname: Obsoleted-by
End:

Tagname: Updates
End:

Tagname: Updated-by
End:

Tagname: See-Also
End:

Tagname: Format
End:

Tagname: Pages
Description: The number of pages in the document.
End:

Tagname: Characters
Description: The size of the RFC in characters.
End:

Tagname: Keywords
Description:: END_D
May be several keywords, seperated by commas

Note that there is no formal allocation of keywords to RFCs, so some of the
allocations of keyword to RFC are subjective. For example all RFCs should
have a security section, but the Security keyword should only be on those
where the primary topic is security.

The Keywords tag in the header record of the master RFC index tag file is used
to construct the list of all known keywords when used in the rfc cgi-bin script.
END_D
End:

Tagname: I-D-Tag
Description:: END_D
The name of the document when it was last and Internet Draft, before it became
an RFC
END_D
End:

