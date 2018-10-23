Tag-dsc-version: 1.0
Describes: event
Introduction:: END_I


General purpose system for describing things which happen at some time,
related to calendar manager, and all these other such things. It should be
capable of acting as an interchange format for all the other calendar
formats.
For example could have a plan2event, suncalendar2event etc.
Kinds of things which could be in event format -
Birthdays, and other annual events, Holidays, TV listings, meetings,
For meetings an event record could be sent to the people who should be
there, containing the agenda for the next meeting as the text, etc.
It could have a status, which could be used to find out if people and
meeting rooms were available.
Would like to be able to hold PHAB programmes, Church notices etc etc.
It would be nice to be able to do History type records in the same system.
(Things like RSC info too)

You could have a handler for incoming events, saying for example that
you are interested in some meetings, but not in others.

END_I
End:

Tagname: Date
Type: Mandatory
Description:  the start date (and time)
End:

Tagname: Duration
Description:  How long it goes on for (if known)
End:

Tagname: Enddate
Description:  An alternative to duration
End:


Tagname: Location
Type: Optional
Description:: END_D
Where the event is taking place - this will usually be a text description,
though it would be useful to have something which could be automatically
processed.
END_D
End:

Tagname: Coordinates
Type: Optional
Description:: END_D
The location of the event as a pair of latitude and longitude numbers
in the format proposed for DNS LOC records.
This will help a program to filter out events which occur a long way from
the person reading the file.
END_D
End:

Tagname: Title
Description: Short description of the event.
End:

Tagname: Description
Description:: END_D
 Multiple lines of text, terminated with its delimiter, can
be HTML if starts with angle bracket.
END_D
End:

Tagname: Required-People
Type: Optional
Description: list of people who must be there, possibly as email addresses
End:

Tagname: Optional-People
Description: People who may be there - sort of like a cc: list.
End:

Tagname: Channel
Description:  for TV programs
End:

Tagname: Status
Description:: END_D
 some form of status code, e.g. Unconfirmed-meeting, Confirmed-meeting,
 Birthday, Anniversary, etc.
END_D
End:

Tagname: Project
Description:: END_D
 (again in dotted decimal form - e.g. phab.oxford, phab.oxon-pac, etc.)
 - theatre.rsc.stratford, theatre.apollo
END_D
End:

Tagname: Repeat
Description:: END_D
 How often the event is repeated, for those such as birthdays that
 repeat annually and preferably to handle those which occur, for example on
 every second thursday, or on the last friday in the month.

END_D
End:

