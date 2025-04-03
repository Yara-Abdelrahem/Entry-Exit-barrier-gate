#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <project-name>"
  exit 1
fi

PROJECT_NAME=$1

st-flash erase
st-flash --format binary write "./APPS/$PROJECT_NAME/build/$PROJECT_NAME.bin" 0x08000000
st-flash reset
echo "Flashed project: $PROJECT_NAME"
