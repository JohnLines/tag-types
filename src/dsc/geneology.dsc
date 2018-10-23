Tag-dsc-version: 1.0
Introduction:: END_INTRO
This data type is intended to express relationships between people, for
exchanging data for family trees etc.
END_INTRO
End:

Tagname: Id
Description: END_ID
The Id may be computer generated, or it may be the name of the person, ie
it is allowed to contain spaces and mixed case.

If an Id which is used in another record in the same file then it should
be preceded by a dot, which indicates to processing programs that it is
viable to attempt to process this Id.

For example
Id: John Smith I
End: John Smith I

Id: John Smith II
Father: .John Smith I
End: John Smith II

Note that programs which process references which are not internal to
the file SHOULD NOT assume that (for example Henry VIII) as a reference
in the incoming file is the same as an internal Henry VIII which they
may already have in their database - although programs may link in new
entries after some sensibility checks, and possibly a human verification.

END_ID
End: Id

Tagname: Father
Description: Gives the father of the current person
End: Father

Tagname: Mother
End: Mother

Tagname: Born
Description:: END_BORN
This is a date, which may not be complete (i.e. it may not have the day
and month)
It may need some way to indicate that a date is approximate.

Programs which output such dates MUST NOT output information which they
do not hold.

Programs processing input which indicates that everyone was born on the
1st of January of the appropriate year SHOULD be very suspicious of the
input data
END_BORN
End: Born

Tagname: Married-to
Description:: END_M
This can also be Married-to-1 etc to allow for people who are married more
than once. Married-to is the same as Married-to-1. It is a reference to
the person they marry.
END_M
End: Married-to

Tagname: Sex
Description:: END_S
This should be either M or F (in upper or lower case)
END_S
End: Sex

Tagname: Child
Description: Reference to a child, there may be more than one of these in a record
End: Child

