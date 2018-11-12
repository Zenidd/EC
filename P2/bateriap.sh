#!/bin/bash
for i in $(seq 1 19); do
rm media
gcc -x assembler-with-cpp -D TEST=$i -no-pie -nostartfiles media.s -o media
echo "-------------------------------------------" 
echo -n "T#$i "; ./media
echo "-------------------------------------------" 
done