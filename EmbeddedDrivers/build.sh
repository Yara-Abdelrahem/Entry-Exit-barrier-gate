#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <project-name>"
  exit 1
fi

PROJECT_NAME=$1

cmake -S "./APPS/$PROJECT_NAME" -B "./APPS/$PROJECT_NAME/build"
cmake --build "./APPS/$PROJECT_NAME/build"
echo "Built project: $PROJECT_NAME"
