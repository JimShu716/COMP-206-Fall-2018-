a=$(head -1 $1)
b=$(tail -1 $1)
cat $2 | tr 'a-z' $a | tr 'A-Z' $b

