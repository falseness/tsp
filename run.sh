#!/bin/bash
echo "$(timeout 1s ./build/tsp < input.txt)"
