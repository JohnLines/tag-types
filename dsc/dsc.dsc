Tag-dsc-version: 1.0
Describes: dsc
Status: Draft
Introduction:: END_I
The tag-type-desc tag type descriptor describes the fields in the tag-type
description system. The aim is to be able to write a program which
parses the syntactic content of any valid tagged file, with it only requiring
the appropriate descriptor file as input.

Any tagged stream of data, whether in a file, or a mail message, or returned
by an application, consists of lines of ascii text. The data is a set of
fields, rather like mail headers, designed to convey some semi-structured
data in a way which renders it capable of automatic processing, while still
being readable by a human.

Another goal was that it should be relatively easy to create a valid tag file
with a simple text editor.

The overall structure of a tagged file is a set of introductory, or header
records - for example containing version information for the file as a whole,
followed by an End tag, which indicates the end of the header tags.

These are followed by sets of tags, each describing a data object.
Each data object
description will start with a tag which names the data object, and will
end with an End tag, which SHOULD be followed by the name of the object it
is ending. (It may be left blank, in which case programs processing the file
may add the name of the object during the processing)

Tags generally consist of a single word (though hyphens and underscores are
allowed), followed by a colon, followed by the data for that tag. (very
similar to mail headers. If the data for that tag would stretch over more than
one line then the tag name will be followed by two successive colons, 
which is then followed by an end label. The data for that tag follows
on successive lines until a matching end label is encountered on a line of
its own.

The period character (.) within tag names is reserved for creating a level of
structure within the tagged data. It can be used for things like ingredients
in recipies where you could have (for example)
Ingredient.1: Sugar
Quantity.1: 2tsp
Ingredient.2: Flour
Quantity.2: 4tsp 

This document describes the syntax used to write all the other tag descriptions.

END_I
End:

Tagname: Tagname
Type: Primary
Description:The Tagname tag contains the name of the field
End: Tagname

Tagname: Description
Type: Optional
Description: This contains the description of the tag
End: Description

Tagname: Type
Type: Mandatory
Description:: END_D
The Type tag says whether the tag will always appear in a section or not.

The types are:
	Primary - Implies mandatory - this is the name for the whole bunch
		of fields
	Mandatory - must be present
	Standard - will normally be present - this is the default
	Optional - may be present - for many things this is the same as Standard
		but it may be used by editors etc to decide whether to offer it
		in a set of default tags.
	Header - present in the header rather than belonging to a particular
		item.
	Unique - the tag can only occur once in an item
	Nonunique - the tag can occur more than once, for example records
		of actions taken, or recipe ingredients
	Numbered - The tagnames will be of the form Tagname.1:, Tagname.2: etc


The Type tag may be a comma seperated list of types, for example
Header,Optional

END_D
End: Type

Tagname: ValueType
Type: Optional
Description:: END_D

The ValueType contains information which permits validitity checking of
the information found in the value part of the tag. The default is
Text, which permits any text. Other possible values are

	Integer - will always contain a decimal integer number
	Numerical - will always contain a number, which need not be an integer.
	Date - a date in ISO 8601 format (with 4 digit year), see
		http://http://www.ft.uni-erlangen.de/~mskuhn/iso-time.html
		(it is unfortunate that I cant reference the ISO standard
		directly)
	Enumeration - must be one of a given set of values
	URL - Contains a Uniform Resource Locator

END_D
End:


Tagname: Introduction
Description: Introduces the tag file description
Type: Header,Optional
End: Introduction

Tagname: Describes
Description:: END_D
Says what the descriptor file describes.
This is a one word description, which will key into a dsc file name, for
example action.dsc will contain Describes: action
and any individual action stream will start Tag-action-version: nnn
This allows a program which is receiving unknown tag information to
find the tag descriptor file, based on the incoming data, even if it does
not have access to the MIME information.
END_D
Type: Header,Mandatory
End: Describes

Tagname: Tag-dsc-version
Description:: END_D
Gives the version number of the description
Some applications require this to be the first tag in the file so that
they can recognise the file type.
END_D
Type: Header,Mandatory
End: Tag-dsc-version

Tagname: Status
Type: Header,Mandatory
Description:: END_D
Gives the status of the description.

Valid values for the status are:
    Draft - The type has been released for initial discussion - interested
       parties should start commenting on the overall scheme for addressing
       whatever data objects this type describes. Fields are very likely
       to be added, renamed, removed etc.

    Alpha - The overall structure of the type is stable, but new fields may
       be added, but interested developers could start trial implementations
       in the understanding that the feedback from themselves and other
       developers may result in changes to the specification

   Beta - In final testing - last chance for any changes

   Release - No more changes until a new version

END_D
End:


Tagname: Example
Type: Optional
Description: Provides an example of that tag being used - more than one example may be given
Example: Type: Optional
End:

Tagname: PGP-sig
Description:: END_D
Provides a PGP signature for all the fields in the current tag record, except
the PGP-sig tag record, but including the Primary tag, and the end tag,
with a value - if that is in the record as sent out in the data stream.

This provides a way of moving tag info around, and verifying the integrity
of individual records, even if they are detached from the rest of a tag
stream.

Obviously PGP can be used to sign a complete set of records externally to
the tag types system.
END_D
End:


