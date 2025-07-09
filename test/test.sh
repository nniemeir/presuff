#!/bin/bash

temp_dir="./test/temp"

mkdir "$temp_dir"

touch "$temp_dir"/file1.txt "$temp_dir"/file2.txt "$temp_dir"/file3.csv

echo "Testing appending pattern to txt files"

./bin/presuff -a -d "$temp_dir" -p _pat -x txt

if [[ -f "$temp_dir"/file1_pat.txt && -f "$temp_dir"/file2_pat.txt && ! -f "$temp_dir"/file3_pat.csv ]]; then
    echo "Result: PASS"
else
    echo "Result: FAIL"
fi

echo "Testing prepending pattern to all files"

./bin/presuff -b -d "$temp_dir" -p pat_

if [[ -f "$temp_dir"/pat_file1_pat.txt && -f "$temp_dir"/pat_file2_pat.txt && -f "$temp_dir"/pat_file3.csv ]]; then
    echo "Result: PASS"
else
    echo "Result: FAIL"
fi

rm -rf "$temp_dir"