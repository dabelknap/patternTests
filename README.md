Creating Patterns
=====================
How to Create Patterns
----------------------
The examples of the files referenced below can be found here:
http://www.hep.wisc.edu/~grogg/files/createPatterns/.

There is now a short script to run through all the patterns quickly in case the
parameters need to change. `runPatterns.sh` takes either a file (ending .txt)
with a list of pattern names, or a pattern name, and a true/false as a second
parameter to include/omit the region sums, or the second parameter can be "many"
when paried with "random" test to run several randoms in a row. It calls
`makePatternConfig.sh`, which simply rewrites the config file with the chosen
pattern name, and then does `cmsRun`, and copies the input/output files to the
`scratch0` directory.

To run on a file with extension `.txt` (make sure it simply has a single column
of usable pattern names): `sh runPatterns.sh <fileName> <includeJetSums>` (the
second parameter is a boolean for including/omitting JetSum information or
`many` when creating many random patterns at once.)

Now the patterns should be in the appropriate folders chosen in `runPattern.sh`.

To run a single pattern:
```sh
$ sh runPatterns.sh <patternName> <includeJetSum/many>
```

Without the script, the pattern making can be run by modifying the file
`rctPattern_cfg.py` as desired and then running the command
```sh
$ cmsRun rctPattern_cfg.py > pattern.log
```

The file `<patternName>.txt` is the output file compared with the output of the
crates, a parameter of `NCrateTest_Analyze.cc`. The file
`RctDigiToSourceCardText.dat` is the expected Source Card output based on the
RCT emulator digis (to be compared to the crates) -- only needed for debugging
and can be used as a parameter of `NCrateTest_Analyze.cc`.

Creating Simulated Events
-------------------------
Patterns based off of simulated physics events can be created using the config
file `rctFilterTtbar_cfg.py` and adding the desired file under `PoolSource`.
First, `rctHeader_cfg.py` has to be run with only one event to create the header
(for finding in the source card data later). Run the commands:
```sh
$ cmsRun rctHeader_cfg.py
$ cmsRun rctFilterTtbar_cfg.py
```

The first command will create `ttbar.txt` and `ttbarInput.txt` files with just
the header event. The second command will *append* the `ttbar` events to these
files (so be sure to move, rename, or remove these files is running more than
once). The config file currently uses a filter to ensure certian crates get some
significant activity. The filter is not in CVS yet.

Creating Link Tests
-------------------
Link tests are new integrated into the usual config file, and files will be
created whenever running, unless `process.ecalSimRawData` and/or
`process.htr_xml` are removed from the path. ECAL files are put into the
directory `data/<patternName>` and the HCAL files are put in the
`/tmp/grogg/<patternName>` directory (they are very large files).

To see the non-zero output of the HCAL files, `histo_check.C` can be run in
`root`, with the first parameter being the min rank to plot, and the second
parameter being the pattern name. It creates a `ps` file with all the plots from
the `root` file that have info above the min rank. The plots are labelled crate,
slot (htr, 1-18), FPGA (top/bottom), channel (24) (do not have a simple mapping
yet). The actual file used by HCAL is a large `xml` file named
`<patternName>_all.xml`.

ECAL not working properly yet, the mapping is strange!
For ECAL, there is one file made per TCC (37-72), each TCC is half of a crate
(top/even or bottom/odd) and covers 1-18 in eta, or cards 0-3 +2 towers of cards
4 and 5 (This is just the barrel). TCC 37 is the top crate 2, TCC 38 is the
bottom of crate 1, ..., TCC 54 is the bottom of crate 2, TCC 55 is the top of
Crate 11, ..., TCC 72 is the bottom of crate 11. See the TWiki page
https://twiki.cern.ch/twiki/bin/viewauth/CMS/RCTLinkTesting for full mapping.
The columns are tower (different numbering from RCT, goes from 1-72), crossing
(0-1023), ECAL rank, HCAL rank (always 0).
