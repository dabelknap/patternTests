# patternTests
Kira's code for generating RCT input/output patterns

For more detailed information <a href="https://github.com/uwcms/patternTests/blob/kirascode_safecopy/README.md">please read Kira's introduction on pattern tests</a>.

This package has two different modes:
a) Create random / generic patterns (e.g, walking 1s)
b) Convert a Monte Carlo file (eg, ttbar) into a pattern that we can feed to the RCT

b) Can be done by running 
*cmsRun test/testNoFilter.py*
which will create three files:
- GGH2TauInputNew.txt  ---> Input to be fed to the RCT
- GGH2TauLog.txt       ---> Emulated log for diff. comparisons (old style pattern test) 
- rct64GGH2TauEventsFilter.root  --> the 64 events used, for plotting a reference for graphical analysis 

(cmsRun test/testFilter.py will select particular crates only. It needs some cleaning).


a) Should be done through *L1RCTPatternProducer.cc* (see Kira's notes), which in this branch is a reduced version of the original code. 
Right now this part of the code is not fully operational, we are still using the same generic patterns as in Run1.
Some cleaning / preparing is needed. 
An additional feature we need to incorporate is to reverse engineer the pattern production so we can derive rctDigis from 
the old pattern input.txt files, to allow graphical interpretation of the more simple tests.



