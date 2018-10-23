Tag-dsc-version: 1.0
Describes: biblio
Introduction:: END_I

This is heavily based on RFC1807 - A Format for Bibliographic Records
by R. Lasher and D. Cohen
- much of the following text is directly from their RFC.


   Many universities and other R&D organizations routinely announce new
   technical reports by mailing (via the postal services) the
   bibliographic records of these reports.

   These mailings have non-trivial cost and delay.  In addition, their
   recipients cannot conveniently file them, electronically, for later
   retrieval and searches.

   Publishing organizations that wish to use e-mail or file transfer to
   obtain these announcements can do so by using the following format.

   Organizations may automate to any degree (or not at all) both the
   creation of these records (about their own publications) and the
   handling of the records received from other organizations.

   This format is designed to be simple, for people and for machines, to
   be easy to read ("human readable") and create without any special
   programs.

   This RFC defines the format of bibliographic records, not how to
   process them.



   This format is a "tagged" format with self-explaining alphabetic
   tags. It should be possible to prepare and to read bibliographic
   records using any text editor, without any special programs.

   This RFC includes the CR-CATEGORY, a field useful for Computer
   Science publications.  It is expected that similar fields will be
   added for other domains.


   This RFC does not place any limitations on the dissemination of the
   bibliographic records.  If there are limitations on the dissemination
   of the publication, it should be protected by some means such as
   passwords.  This RFC does not address this protection.

   The use of this format is encouraged.  There are no limitations on
   its use.


The Information Fields

   The various fields should follow the format described below.

   <M> means Mandatory; a record without it is invalid.
   <O> means Optional.

   The tags (aka Field-IDs) are shown in upper case.

           <M>  BIB-VERSION of this bibliographic records format
           <M>  ID
           <M>  ENTRY date
           <O>  ORGANIZATION
           <O>  TITLE
           <O>  TYPE
           <O>  REVISION
           <O>  WITHDRAW
           <O>  AUTHOR
           <O>  CORP-AUTHOR
           <O>  CONTACT for the author(s)
           <O>  DATE of publication
           <O>  PAGES count
           <O>  COPYRIGHT, permissions and disclaimers
           <O>  HANDLE
           <O>  OTHER_ACCESS
           <O>  RETRIEVAL
           <O>  KEYWORD
           <O>  CR-CATEGORY
           <O>  PERIOD
           <O>  SERIES
           <O>  MONITORING organization(s)
           <O>  FUNDING organization(s)
           <O>  CONTRACT number(s)
           <O>  GRANT number(s)
           <O>  LANGUAGE name
           <O>  NOTES
           <O>  ABSTRACT
           <M>  END








The Actual Format

   The term "Open Ended Format" in the following means arbitrary text.

   In the following double-quotes indicate complete strings.  They are
   included only for grouping and are not expected to be used in the
   actual records.



A Note Regarding the Controlled Symbols of the Publishers

   In order to avoid conflicts among the symbols of the publishing
   organizations (the XXX part of the "ID:: XXX//YYY") it is suggested
   that the various organizations that publish reports (such as
   universities, departments, and laboratories) register their
   <publisher-ID> symbols and names, in a way similar to the
   registration of other key parameters and names in the Internet.



   Rebecca Lasher (RLASHER@Forsythe.stanford.edu), of Stanford working
   with CNRI has agreed to coordinate this registration with the IANA
   for the publishers of Computer Science technical reports.  It is
   suggested that before using this format the publishing organizations
   would coordinate with her (by e-mail) their symbols and the names of
   their organizations.

   In order to help automated handling of the received bibliographic
   records, it is expected that the producers of bibliographic records
   will always use the same name, exactly, in the ORGANIZATION field.

END_I
End:


Tagname: Id
Type: Primary
Description:: END_D
 This is the second field of any record.  It is also a
        mandatory field.   The ID field identifies the bibliographic
        record and is used in management of these records.
        Its format is "ID:: XXX//YYY", where XXX is the
        publisher-ID (the controlled symbol of the publisher)
        and YYY is the ID (e.g., report number) of the
        publication as assigned by the publisher.  This ID is
        typically printed on the cover, and may contain slashes.

        The organization symbols "DUMMY" and "TEST" (case
        independent) are reserved for test records that should NOT
        be incorporated in the permanent database of the
        recipients.

        Format:   Id: <publisher-ID>//<free-text>


            **** See the note at the end regarding the ****
            **** controlled symbols of the publishers *****

END_D
Example:  Id: OUKS//CS-TR-91-123
End:


Tagname: Entry
Type: Mandatory
Description:: END_D
 This is a mandatory field.  It is the date of
        creating this bibliographic record.

        The format for ENTRY date is "Month Day, Year".  The
        month must be alphabetic (spelled out).  The "Day" is a
        1- or 2-digit number.  The "Year" is a 4-digit number.

        Format:   ENTRY:: <date>

END_D
Example:  Entry: January 15, 1992
End:

Tagname: Organisation
Type: Optional
Description:: END_D
 It is the full name spelled out (no acronyms,
        please) of the publishing organization.  The use of this
        name is controlled together with the controlled symbol of
        the publisher (as discussed above for the ID field).

        Avoid acronyms because there are many common acronyms,
        such as ISI and USC.  Please provide it in ascending
        order, such as "X University, Y Department" (not "Y
        Department, X University").

        Format:   ORGANIZATION:: <free-text>
END_D
Example:  Organization: Stanford University, Department of Computer Science
End:

Tagname: Title
Type: Optional
Description:: END_D
 This is the title of the work as assigned by the
        author. This field should include the complete title with
        all the subtitles, if any.

        Format:   TITLE:: <free-text>

        Example:  TITLE:: The Computerization of Oceanview with
                        High Speed Fiber Optics Communication
END_D
End:


Tagname: Type
Type: Optional
Description:: END_D
 Indicates the type of publication (summary, final
        project report, etc.) as assigned by the issuing
        organization.

        Format:   TYPE:: <free-text>
END_D
Example:  Type: Technical Report
End: Type

Tagname: Revision
Type: Optional
Description:: END_D
 Indicates that the current bibliographic record is
        a revision of a previously issued record and is intended
        to replace it.  Revision information consists of a date
        and/or followed by a semicolon and by text in an open
        ended format. The revised bibliographic record should
        contain a complete record for the publication, not just a
        list of changes to the old record.  If revision is
        omitted, the record is assumed to be a new record and not
        a revision.  If the revision date is specified as 0, this
        is assumed to be January 1, 1900 (the previous RFC, used
        revision data of 0, 1, 2, 3, etc. this specification is for
        programs that might process records from RFC1357).

        The text before the semicolon in this field is a date of
        the form month day, year.  Any record with a more recent
        revision date replaces completely any record with an
        earlier revision date (supplied either explicitly or by
        default).  Use the text to describe the revision.
        Reasons to send out a revised record include an error in
        the original, or change in the access information.

        Format:  REVISION:: January 1, 1995; <free-text>

        Example: REVISION:: January 1, 1995; FTP information
                        added

END_D
End:

Tagname: Withdraw
Type: Optional
Description:: END_D
 Withdraw means the document is no longer
        available.  Some Institutions choose to delete the record
        others remove some of the fields.  It is up to each
        institution to decide how to process withdraw records.

        A withdraw record has all of the mandatory fields plus the
        withdraw field and a mandatory revision field.
        The Withdraw field should indicate the reason for the
        withdraw in free text.
        Example for withdrawing a bibliographic record::

            Tag-Bib-version:  CS-TR-v2.1
            Id: OUKS//CS-TR-91-123
            Entry:        January 21, 1995
            Organization: Oceanview University, Kansas, Computer
                           Science
            TITLE:        The Computerization of Oceanview with
                           High Speed Fiber Optics Communication
            Revision:     January 21, 1995
            Withdraw:     Withdrawn, found to be irrelevant
            End: OUKS//CS-TR-91-123

END_D
End:

Tagname: Author
Type: Optional
Description:: END_D
 Personal names only.  Normal last name first
        inversion.  Editors should be listed here as well,
        identified with the usual "(ed.)" as shown below in the last
        example.

        If the report was not authored by a person (e.g., it was
        authored by a committee or a panel) use CORP-AUTHOR (see
        below) instead of AUTHOR.

        Multiple authors are entered by using multiple lines, each
        in the form of "AUTHOR:: <free-text>".

        The system preserves the order of the authors.

        Format:   AUTHOR:: <free-text>

        Example:  AUTHOR:: Finnegan, James A.
                  AUTHOR:: Pooh, Winnie The
                  AUTHOR:: Lastname, Firstname (ed.)
END_D
End:

Tagname: Corp-author
Type:Optional
Description:: END_D
 The corporate author (e.g., a committee or a
        panel) that authored the report, which may be different
        from the ORGANIZATION issuing the report.

        In entering the corporate name please omit initial "the"
        or "a".  If it is really part of the name, please invert it.

        Format:   CORP-AUTHOR:: <free-text>

        Example:  CORP-AUTHOR:: Committee on long-range computing
END_D
End:

Tagname: Contact
Type: Optional
Description:: END_D
 The contact for the author(s).
        Open-ended, most likely E-mail and postal addresses.

        A CONTACT field for each author should be provided,
        separately, or for all the AUTHOR fields.
        E-mail addresses should always be in "pointy brackets"
        (as in the example below).

        Format:   CONTACT:: <free-text>

        Example:  CONTACT:: Prof. J. A. Finnegan, CS Dept,
                           Oceanview Univ., Oceanview, Kansas, 54321
                           Tel: 913-456-7890 <Finnegan@cs.ouks.edu>
END_D
End:

Tagname: Date
Type: Optional
Description:: END_D
The publication date.  The formats are "Month Year"
        and "Month Day, Year".  The month must be alphabetic
        (spelled out).  The "Day" is a 1- or 2-digit number.  The
        "Year" is a 4- digit number.

        Format:   DATE:: <date>

        Example:  DATE:: January 1992
        Example:  DATE:: January 15, 1992
END_D
End:

Tagname: Pages
Type: Optional
Description:: END_D
Total number of pages, without being too picky about
        it.  Final numbered page is actually preferred, if it is a
        reasonable approximation to the total number of pages.

        Format:   PAGES:: <number>

        Example:  PAGES:: 48
END_D
End:

Tagname: Copyright
Type: Optional
Description:: END_D
Copyright information.  Open ended format.  The
        COPYRIGHT field applies to the cited report, rather than
        to the current bibliographic record.

        Format:  COPYRIGHT:: <free-text>

        Example: COPYRIGHT:: Copyright for the report (c) 1991,
                            by J. A. Finnegan.  All rights
                            reserved.
                            Permission is granted for any academic
                            use of the report.
END_D
End:

Tagname: Handle
Type: Optional
Description:: END_D
 Handles are unique permanent identifiers that are
        used in the Handle Management System to retrieve location
        data.  A handle is a printable string which when given to
        a handle server returns the location of the data.

        Handles are used to identify digital objects stored within
        a digital library.  If the technical report is available in
        electronic form, the Handle MUST be supplied in the
        bibliographic record.

        Format is "HANDLE:: hdl:<naming authority>/string
        of characters".  The string of characters can be the
        report number of the technical report as assigned by the
        publisher.  For more information on handles and handle
        servers see the CNRI WEB page at


        http://www.cnri.reston.va.us.

  **** NOTE:  White space in HANDLE due to line wrap is ignored.

        Format:  HANDLE:: hdl:<naming authority>/string of
                          characters

        Example: HANDLE:: hdl:oceanview.electr/CS-TR-91-123
END_D
End:


Tagname: Other-access
Type: Optional
Description:: END_D
For URLs, URNs, and other yet to be invented
       formatted retrieval systems.

        Only one URL or URN per occurrence of the field.

        URL and URN information is available in the internet
        drafts from the IETF (Internet Engineering Task Force).
        The most recent drafts can be found on the CNRI WEB page
        at http://www.cnri.reston.va.us.

**** NOTE: White space in a URL or URN due to line wrap is ignored.

        Format:  OTHER_ACCESS:: URL:<URL>
                 OTHER_ACCESS:: URN:<URN>

        Example: OTHER_ACCESS:: URL:http://elib.stanford.edu/Docume
        nt/STANFORD.CS:CS-TN-94-1

        Example: OTHER_ACCESS:: URL:ftp://JUPITER.CS.OUKS.EDU/PUBS/
        computerization.txt.

        When the URN standard is finalized naming authorities will
        be registered and URNs will be viable unique identifiers.
        Until then this is a place holder.  For the latest URN
        drafts see CNRI WEB page at http://www.cnri.reston.va.us.
END_D
End:

Tagname: Retrieval
Type: Optional
Description:: END_D
 Open-ended format describing how to get
        a copy of the full text.  This is an optional, repeatable
        field.

        No limitations are placed on the dissemination of the
        bibliographic records.  If there are limitations on the
        dissemination of the publication, it should be protected
        by some means such as passwords.  This format does not
        address this protection.

        Format:  RETRIEVAL:: <free-text>


                 RETRIEVAL:: for full text with color pictures
                           send a self-addressed stamped envelope to
                           Prof. J.A.  Finnegan, CS Dept,
                           Oceanview University, Oceanview, KS 54321
END_D
End:

Tagname: Keyword
Type: Optional
Description:: END_D
 Specify any keywords, controlled or uncontrolled.
        This is an optional, repeatable field.  Multiple keywords
        are entered using multiple lines in the form of
        "KEYWORD::  <free-text>.

        Format:   KEYWORD:: <free-text>

        Example:  KEYWORD:: Scientific Communication
                  KEYWORD:: Communication Theory
END_D
End:

Tagname: CR-catagory
Type: Optional
Description:: END_D
Specify the CR-category.  The CR-category (the
        Computer Reviews Category) index (e.g., "B.3") should
        always be included, optionally followed by the name of that
        category.  If the name is specified it should be fully
        specified with parent levels as needed to clarify it, as in
        the second example below.  Use multiple lines for multiple
        categories.

        Every year, the January issue of CR has the full list
        of these categories, with a detailed discussion of the
        CR Classification System, and a full index.  Typically the
        full index appears in every January issue, and the top two
        levels in every issue.

        Format:   CR-CATEGORY:: <free-text>

        Example:  CR-CATEGORY:: D.1

        Example:  CR-CATEGORY:: B.3 Hardware, Memory Structures
END_D
End:

Tagname: Period
Type: Optional
Description:: END_D
Time period covered (date range).  Applicable
        primarily to progress reports, etc.  Any format is
        acceptable, as long as the two dates are separated with
        " to " (the word "to" surrounded by spaces) and each date
        is in the format allowed for dates, as described above for
        the date field.

        Format:   PERIOD:: <date> to <date>

        Example:  PERIOD:: January 1990 to March 1990
END_D
End:

Tagname: Series
Type: Optional
Description:: END_D
Series title, including volume number within series.
        Open-ended format, with producing institution strongly
        encouraged to be internally consistent.

        Format:   SERIES:: <free-text>
END_D
Example: Series: Communication
End:

Tagname: Funding
Type: Optional
Description:: END_D
The name(s) of the funding organization(s).

        Format:   FUNDING:: <free-text>

        Example:  FUNDING:: ARPA
END_D
End:

Tagname: Monitoring
Type: Optional
Description:: END_D
The name(s) of the monitoring organization(s).

        Format:   MONITORING:: <free-text>
END_D
Example: Monitoring: ONR
End:

Tagname: Contract
Type: Optional
Description:: END_D
The contract number(s).

        Format:   CONTRACT:: <free-text>

        Example:  CONTRACT:: MMA-90-23-456
END_D
End:

Tagname: Grant
Type: Optional
Description: The grant number(s).
Example: Grant: NASA-91-2345
End:

Tagname: Language
Type: Optional
Description:: END_D
The language in which the report is written.
        Please use the full English name of that language.

        Please include the Abstract in English, if possible.

        If the language is not specified, English is assumed.

        Format:   LANGUAGE:: <free-text>

        Example:  LANGUAGE:: English
        Example:  LANGUAGE:: French
END_D
End:

Tagname: Notes
Type: Optional
Description:: END_D
Miscellaneous free text.

        Format:   NOTES:: <free-text>

        Example:  NOTES:: This report is the full version of the
                        paper with the same title in IEEE Trans ASSP
                        Dec 1976
END_D
End:


Tagname: Abstract
Type: Optional
Description:: END_D
 Highly recommended, but not mandatory.  Even
        though no limit is defined for its length, it is suggested
        not to expect applications to be able to handle more than
        10,000 characters.

        The ABSTRACT is expected to be used for subject searching
        since titles are not enough.  Even if the report is not in
        English, an English ABSTRACT is preferable.  If no formal
        abstract appears on document, the producers of the
        bibliographic records are encouraged to use pieces of the
        introduction, first paragraph, etc.

        Format:  ABSTRACT:: xxxx .............. xxxxxxxx
                            xxxx .............. xxxxxxxx

                            xxxx .............. xxxxxxxx
                            xxxx .............. xxxxxxxx

END_D
End:


