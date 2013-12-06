#!/bin/bash

echo "" > results
for i in {10..20}; do
   echo "Running on $i..."
   (time ./naive-single ./static/$i.txt $i) >> results 2>&1
done
