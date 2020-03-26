#!/bin/bash

((cntr=0))

while [ 1 ]
do
    (( ++cntr ))
    curl -X POST "http://127.0.0.1:8888/v1/item" -H "accept: application/json" -H "Content-Type: application/json" -d "{\"key\": $cntr, \"value\": \"decoder$cntr\"}"

done



