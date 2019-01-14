#!/bin/sh

# Infra编译调试脚本
make_infra()
{
    cmake .. 
	make -j4
}

get_git_version()
{
	commit_ts=`git log -1 --format="%ct"`
	echo "git commit_ts = ${commit_ts}"
	commit_time=`date -d @$commit_ts +"%Y-%m-%d %H:%M"`
	current_time=`date +"%Y-%m-%d %H:%M"`
	git_version=`git log -1 --format="%h"`
	echo "commit time = ${commit_time}, current time = ${current_time}"
	echo "git version = ${git_version}"
	version="git version:$git_version, commit time:$commit_time, build time:$current_time"
	echo "git version = ${version}"
	echo $version
}

make_default()
{
	echo "make && execute challenge"
    make -j4
    if [ $? -eq 0 ]
    then
    ./challenge
    fi
}

name=$1 

echo "argc = $#, name = ${name}"
if [ $# -gt 0 ] && [ ${name} = "infra" ]
then
    make_infra
else
    make_default
fi

echo "------------------------------------------"

