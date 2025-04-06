#!/bin/sh

find . -type f -executable | xargs rm
find . -type f -name ".png" -print
