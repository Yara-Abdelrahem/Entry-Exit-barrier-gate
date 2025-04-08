#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <project-name>"
  exit 1
fi

PROJECT_NAME=$1

rm -rf "./APPS/$PROJECT_NAME/build"
echo "Cleaned build for project: $PROJECT_NAME"
