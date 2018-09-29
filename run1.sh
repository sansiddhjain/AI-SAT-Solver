./inp $1 < $1.graph > test.satinput
minisat test.satinput test.satoutput
./main2 < $1.graph > $1.subgraphs
cat $1.subgraphs
