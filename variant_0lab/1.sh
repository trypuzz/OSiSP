#!/bin/bash

if gcc $1 -o 1.out; 
    then
    shift 1;
    ./1.out $@;
else
    echo "error: Ошибка компиляции" >&2;
fi
