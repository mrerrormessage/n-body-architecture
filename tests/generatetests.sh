#!/bin/sh

echo `./testgen.pl 200 200 .005 2 50 > nbodytest1.txt`
echo `./testgen.pl 200 200 .005 3 50 > nbodytest2.txt`
echo `./testgen.pl 200 200 .005 50 50 > nbodytest3.txt`

git add nbodytest*
