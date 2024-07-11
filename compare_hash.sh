#!/bin/bash

#this is a case sensitive tool. if the inputted or calculated hash is different case(lower, upper) from 
#the one it is compared to, it won't deem them the same.

if [ $# -ne 3 ]; then
    echo "Usage: $0 <file> <hash> <algorithm>"
    exit 1
fi

file=$1
provided_hash=$2
algorithm=$3

if [ ! -f "$file" ]; then
    echo "Error: File '$file' does not exist."
    exit 1
fi

calculate_hash() {
    local file=$1
    local algorithm=$2
    local hash_value

    case $algorithm in
        md5)
            hash_value=$(md5 -q "$file")
            ;;
        sha1)
            hash_value=$(shasum -a 1 "$file" | awk '{print $1}')
            ;;
        sha256)
            hash_value=$(shasum -a 256 "$file" | awk '{print $1}')
            ;;
        *)
            echo "Error: Unsupported algorithm '$algorithm'."
            exit 1
            ;;
    esac

    echo $hash_value
}

calculated_hash=$(calculate_hash "$file" "$algorithm")

if [ "$calculated_hash" == "$provided_hash" ]; then
    echo "Success: Hash matches using $algorithm"
else
    echo "Error: Hash does not match."
fi
