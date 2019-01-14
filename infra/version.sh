#!/bin/sh

commit_ts=`git log -1 --format="%ct"`
echo "git commit_ts = ${commit_ts}"
commit_time=`date -d @$commit_ts +"%Y-%m-%d %H:%M"`
current_time=`date +"%Y-%m-%d %H:%M"`
git_version=`git log -1 --format="%h"`
echo "commit time = ${commit_time}, current time = ${current_time}"
echo "git version = ${git_version}"
version="git version:$git_version, commit time:$commit_time, build time:$current_time"
echo "git version = ${version}"

