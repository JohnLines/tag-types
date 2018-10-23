Tag-dsc-version: 1.0
Describes: fintrans
Status: Draft
Introduction:: END_I
The Tag-fintrans file format is designed to provide an interchange format
for financial transactions. It is not targetted at being a storage format
for data (in other words a finance program would use some internal
database, but to be able to export and import in this format).

Auditing
--------

This format is designed to be manipulated with the tag utilities which can
be found at http://www.paladin.demon.co.uk/tag-types/

One of the goals of the system is that it should make accounts of small
charities or businesses easier to audit in that the auditor can use their
own copy of a set of auditing utilties (these dont exist yet !!) to slice
the accounts any way they wish, without having to understand the particular
accounting software being used by their client.

Comments from auditors on the general concept and specific format would be
appreciated.

END_I

End:

Tagname: Id
Description:: END_D
For a transfer format it is useful if every transaction has some form of
unique identifier. (Unique within a particular accounting system at least).
For example if you enter your transactions into a palmtop computer as you
spend your money, and export those transactions from time to time to another
system which holds your main finances then the importing system should be
able to avoid entering the transaction a second time, but it SHOULD check
to see if the transaction data has changed, in which case it may wish to
update the master copy.
END_D
End:

Tagname: Date
Description:: END_D
The date on which the transaction occurred. This should be written in a
standard format - which should include a 4 digit year.
This should be ISO 8601 format i.e. yyyy-mm-dd
END_D
ValueType: Date
End:

Tagname: Account
Description:: END_D
The account to which this transaction relates. If this is in the header then
all transactions in the file relate to this account, otherwise this field
must be in every record.
END_D
End:

Tagname: Payee
Description:: END_D
The external account, or person which this transaction is going to, or coming
from if it is a credit.

END_D
End:

Tagname: Transfer-account
Description:: END_D
The other account, also able to be referenced as a tagged file, to which a
transfer is being made. Similar to Payee, but it may be constrained to
be the name of a known account
END_D
End:


Tagname: Reference
Description:: END_D
This is the bank (or other accounting system) reference for this transaction,
for example for a cheque it would be the cheque number.
END_D
End:

Tagname: Status
Description:: END_D
Keywords related to this transaction - the only defined one so far is
	Reconciled - indicates that the transaction has been confirmed by
		appearing in your bank statement.


(Actually I am in a bit of a dilemma on a Reconciled flag - it may be
better to use the Cleared-Date field below, and fake one if we are
converting from QIF)

END_D
End:

Tagname: Description
Description: Long description of the transaction
End:

Tagname: Cleared-Date
Description:: END_D
Date the transaction cleared.

When converting from Quicken this field will be filled in with the
transaction date as that is the only one Quicken knows.
When converting to QIF the Reconciled flag will be set if this field
is present.
END_D
ValueType: Date
End:


Tagname: Amount
Description:: END_D
The amount of the debit from the account, or credit to the account (specified
as a negative number).
This will be the amount in the currency in which the account is maintained.
END_D
End:

Tagname: Foreign-Amount
Description:: END_D
The amount of the debit from, or credit to, the account, in some currency
which is not the normal currency in which the account is maintained.
END_D
End:

Tagname: Currency
Description:: END_D
As a header tag the default currency for all transactions in this account,
or for a foreign currency transaction it should be the currency of this
particular transaction.

This should be a 3 character code from ISO4217, e.g. GPB for UK Pounds or
USD for US Dollars
END_D
End:


Tagname: Asset
Description:: END_D
This transaction relates to something which will be regarded as an asset.
This name must be used consistenly to refer the to asset.
END_D
End:

Tagname: Value
Description:: END_D
The value of the asset at the time of the transaction.

Note that 'revaluation' transactions are permitted, which have no Amount
entry, but which just indicate that the nominal value of a asset, such
as a share or a house, has changed.

Of course valuation records have no 'real' meaning, in that if your shares
or house value seem to have gone dramatically up or down this is not actually
a change in your net worth until you actually change that asset into cash.
END_D
End:

Tagname: Quantity
Description:: END_D
This goes with an Asset and Value entry. If it is not present the implied
quanity is 1. It indicates the number of Assets which have the given value,
for example you could have 100 shares with a value of 625 pounds then you
could express the value as

Asset: Some Share
Value: 625
Quantity: 100

or as

Asset: Some Share
Value: 6.25

(where you keep track of the number of shares you hold by knowing that you
have not acquired or disposed of any)

END_D
End:

Tagname: Item
Description:: END_D
This can be used to indicate that more than one item is refered to in this
transaction.
END_D
End:

Tagname: Cost
Description:: END_D
The cost associated with a particular item, for example
Item.1: Petrol
Cost.1: 12.9
Quantity.1: 24.3
Item.2: Newspaper
Cost.2: .40

END_D
End:

Tagname: Subamount
Description:: END_D
Similar to Cost - possibly a more descriptive term for implementing Quicken
style splits.
END_D
End:


Tagname: TaxCredit
Description:: END_D
The amount of tax deemed to have already been paid on a share dividend payment.
This will not affect the amount of the transaction, but may be used by
specialised tax software in preparing annual returns.
END_D
End:

Tagname: Catagory
Description:: END_D
A key phrase which describes the overall purpose of this transaction
END_D
End:

