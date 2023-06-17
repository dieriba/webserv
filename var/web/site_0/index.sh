#!/bin/bash

echo "Content-Type: text/plain"
echo ""

# Check the request method
request_method="$REQUEST_METHOD"

# Handle GET request
if [ "$request_method" = "GET" ]; then
    # Parse the query string
    query_string="$QUERY_STRING"
    IFS='&' read -ra params <<< "$query_string"

    # Output the key-value pairs
    for param in "${params[@]}"; do
        IFS='=' read -r key value <<< "$param"
        echo "$key: $value"
    done

# Handle POST request
elif [ "$request_method" = "POST" ]; then
    # Read the input from stdin
    read -r input

    # Parse the input as key-value pairs
    IFS='&' read -ra params <<< "$input"

    # Output the key-value pairs
    for param in "${params[@]}"; do
        IFS='=' read -r key value <<< "$param"
        echo "$key: $value"
    done
fi
