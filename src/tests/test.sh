#!/bin/sh

# Test the tag utilities
#tagbindir=/usr/local/bin
tagbindir=..
finalstatus=0

# Make sure we are not confused by results of previous tests
rm -f *.out *.err testok


$tagbindir/tagextract -m Priority == 50 <test1.act >test1.out
cmp test1.out test1.res
finalstatus=$finalstatus+$?

# test tagcount
$tagbindir/tagcount -r < test1.act >test2.out
cmp test2.out test2.res
finalstatus=$finalstatus+$?

# test tagshow -t
$tagbindir/tagshow -t Priority <test2.act >test3.out
cmp test3.out test3.res

# test tagcheck

$tagbindir/tagcheck test4.tag >test4.out
cmp test4.out test4.res

# a hard test - we expect some complaints about this one
$tagbindir/tagcheck test5.tag >test5.out 2>test5.err


touch testok
#exit $finalstatus

