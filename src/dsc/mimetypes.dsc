Tag-dsc-version: 1.0
Describes: mimetype
Status: Draft
Introduction:: END_I

The mimetype tagged type provides a formal description of the ietf
MIME media types registration template described in the IETF MIME
Registrations draft RFC.

Note that the sections of the template have been given shorter names to
fit the tag file style.

Specific differences between this and the registration template are described
in the descriptions below. 

This is largely provided as an example, for discussion on the ietf-types
list.
END_I
End:

Tagname: Mimetype
Type: Primary
Description:: END_D

The MIME type and subtype which are to be registered, in the format
type/subtype.

Note that this has compressed 2 fields from the registration template into
one field - this has been done so that we can use this as a unique
descriptor for this record.

END_D
Example: Mimetype: Application/tag
End:

Tagname: Required-parameters
End:

Tagname: Optional-parameters
End:

Tagname: Encoding
End:

Tagname: Security
Type: Mandatory
Description:: END_D
The security implications of the MIME type/subtype should be described.
'None' is rarely sufficient
END_D
End:

Tagname: Interoperability
End:

Tagname: Specification
End:

Tagname: Applications
End:

Tagname: Magic-number
Type: Optional
End:

Tagname: File-extension
End:

Tagname: Macintosh-file-type-code
End:

Tagname: Info-contact
End:

Tagname: Usage
Description: One of Common, Limited Use or Obsolete
End:

Tagname: Change-controller
Description: Email address of the author or change controller
End:

Tagname: Description
Description: Any additional free text.
End:


