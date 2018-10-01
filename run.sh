./inp $1 < $1.graph > $1.satinput
minisat $1.satinput $1.satoutput
./main2 < $1.graph > $1.subgraphs $1
cat $1.subgraphs
