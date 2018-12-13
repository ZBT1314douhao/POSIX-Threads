#!/bin/sh

# Infra编译调试脚本
make_infra()
{
    make
}

mak_default()
{
    make
    if [ $? -eq 0 ]
    then
    ./challenge
    fi
}

name=$1 

if [ $# -gt 0 && ${name} = "infra" ]
then
    make_infra
else
    mak_default
fi

echo "------------------------------------------"

