#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
CODE_DIRS=(src libs)
FORMAT_CMD="clang-format -i -style=file"

COMMIT=0
[ "$1" = "-c" ] || COMMIT=1


for d in "${CODE_DIRS[@]}"
do
    find $DIR/$d -type f \
                 \( -iname "*.cpp" -o -iname "*.h" -o -iname "*.hpp" -o -iname "*.cc" -o -iname "*.c" \) \
                 -exec $FORMAT_CMD '{}' \;
    [ $COMMIT = 1 ] || git add $d
done

[ $COMMIT = 1 ] || git commit --author="format script <script@script.org>" -m "[script] Format code."
