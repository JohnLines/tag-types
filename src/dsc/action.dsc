Tag-dsc-version: 1.0
Describes: action
Introduction:: END_I


The record format has the following goals:
1) It should be readable by people, and capable of being produced manually.
2) it should be flexible and compact

These records could be special cases of a general mime format for tagged
text records, in which case the format will be
Label: line of text
or
Label:: Delimiter
Lines of text
...
Delimiter
Individual records end with an End: label, which could be End: id
to allow for some kind of verification.

There may be tags which do not belong to any individual item, but belong
to the whole block of data - in general these will be things like version
information.

Actions
-------
Each action file (or message) starts with the line
Action-Version: 1.00
(This is required so that the action system can be sure that it is
dealing with a stream of action data. (It need not be in a file, it
could be in a stream of data returned by an application ???)
You only need one version line per file, but can have more than one.

The action system could be used as a helpdesk system, by allowing multiple
updates to a single action. (A little like the system for mail received
headers)

The various tags are described below - but here is an example action

Id: tagfiles.actedit.12
Title: Sort/select by Assigned-to
End:

This is a minimal, but still valid action.


There could be an email interface for doing things like finding out how many
actions were in a particular queue,etc. Mail to some address could be
parsed automatically into action records. Any particular action would then
end up with a reference of (for example) aeat.internet.support.143
The question is should this reference (or Id) be really certain to be
unique. Perhaps there should be an implicit header for all these fields
which would be (for example uk.aeat.co ??? - this could make these really
unique ??)



Each action entry ends with an End: label, which can be of the form
End: id, where id is a repeat of the Id: line.



Things to do within the actions system.
Add a new action
Update an action
Close an action


All of these things should be able to be done by mail or Web over the network.

Things which need to be done

Need a way to deal with recurrent actions

END_I
End:

Tagname: Id
Type: Primary
Description:: END_D
provides an identifier for that action.
For use within a particular environment the identifier will typically be
of the form project[.subproject].n , e.g. tagfiles.actedit.27
Other examples - compiler.frontend.devel.1,  doc.958, etc 

When sent out to an external environment the Id may be qualifed by the
origination host name and username, for example
john.paladin.demon.co.uk..tagfiles.actedit.27
- note the double period. This allows for generation of unique action Ids
for collaborative projects over the Internet.
END_D
End: Id

Tagname: Original-Id
Description: Preserves the original Id if being saved in another action stream
End:

Tagname: Costcode
Type: Optional
Description: provides a billing reference - free text
End: Costcode

Tagname: Log
Type: NonUnique,Optional
Description:: END_D
Provides a way to identify a history of a particular action, as might be used
by a help desk system or project management system. Like mail Received
headers the Log tags should be added to and previous ones preserved.

It may be possible to use the time and date of the log as an end tag for
the text, but only experience will tell if this is a good idea or not.
END_D
End:

Tagname: Expected-cost
Description: budgeted cost
End:

Tagname: Current-cost
Description: the actual cost so far
End:

Tagname: Expected-time
End:

Tagname: Current-time
Description: acutal time so far
End:


Tagname: Precursor
Description: action which must be completed before this one can start
End:

Tagname: Priority
Description:: END_D
A number from 1 to 255, 1 is the highest.
There may need to be a conversion scheme between other action systems,
for example on converting from a system which uses priorities from 1 to 5
 the incoming priorities could be multiplied by 10.
END_D
End:

Tagname: Text
Description:: END_D
Allows for a lump of associated text, for example a complete email
message about the action, which may contain priorities and other
information which has not been broken out into action fields
END_D
End:


Tagname: Description
Description:: END_D
 Free text description of the action - more detailed than the title
Could be HTML if the first character is <
END_D
End:

Tagname: Title
Description: One line description of the action
End:

Tagname: Date
Description: The date the action was entered.
End:

Tagname: Assigned-to
Description: The person who is to do the action, for example as an email address
End:

Tagname: Status-notify
Description:: END_D
List of email addresses to be notified on a change of status
END_D
End:

Tagname: Active-after
Description:: END_D
The action is not valid before this date - for example delete a computer
user after some date.
END_D
End:

Tagname: Status
Description:: END_D
Gives the current status of the action
This can be -
 Done [Date]
Which says that the action has been done, and optionally when

If this field is blank, or not present then it is assumed that the action
is active (though it may be blocked because it has not yet reached its
Active-after date, or because some Precursor action has not been done
yet)

The full set of action states (so far)

Unclaimed - for helpdesk style systems - the action is in the system but
	not owned by anyone yet
Pending - In the things to do queue
Active - actually being worked on
Blocked - waiting for another action, for a date, for authorisation, or some
	external event
Completed
Aborted - note that authorise actions can be completed or aborted.

Each one gets (or can get) a Date field i.e. Pending-Date (or Date-Pending ???)


END_D
End:

Tagname: Keywords
Description:: END_D
Keywords describing the action - for the sake of search systems
END_D
End:

Tagname: Contact-email
Description: email address of the 'customer' for the action
End:

Tagname: Contact
Description: Text description of contact. E.g. name, phone, fax, postal address
End:

Tagname: Analysis
Description: Initial analysis of the problem
End:

Tagname: Solution
Description:: END_D
Describes solution of the problem. This may be useful in building up a
searchable database of past actions where the actions are used to
distribute problem notifications.
Can also be sent to the original customer of the problem when it is closed.
END_D
End:

Tagname: Reason
Description: Describes the reason for doing an action - as text
End:

Tagname: Subtask-of
Description: task Id of the task of which this is a subtask
End:


