#!/bin/bash


((cntr=0))

while [ 1 ]
do
    (( ++cntr ))
    curl -X GET "http://127.0.0.1:8888/v1/item/$cntr" -H "accept: application/json"
done
