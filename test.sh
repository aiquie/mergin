#!/bin/sh

MERGIN=${1:-./mergin}
TMPDIR=$(mktemp -d)

trap "rm -rf $TMDIR" 0

echo '-------------------'
echo '| 3 files         |'
echo '-------------------'
echo 1 >${TMPDIR}/11
echo 2 >${TMPDIR}/12
echo 3 >${TMPDIR}/13
$MERGIN ${TMPDIR}/11 ${TMPDIR}/12 ${TMPDIR}/13

echo '------------------'
echo '| 3 pipes        |'
echo '------------------'
timeout 1 $MERGIN \
    <(sleep 0.3 && echo 3) \
    <(sleep 0.2 && echo 2) \
    <(sleep 0.1 && echo 1)

echo '-------------------'
echo '| stdin + 2 files |'
echo '-------------------'
echo 2 >${TMPDIR}/22
echo 3 >${TMPDIR}/23
echo 1 | $MERGIN - ${TMPDIR}/22 ${TMPDIR}/23

echo '-------------------'
echo '| stdin + 2 pipes |'
echo '-------------------'
mkfifo ${TMPDIR}/31
mkfifo ${TMPDIR}/32
( sleep 0.1; echo 1 >${TMPDIR}/31 )&
( sleep 0.2; echo 2 >${TMPDIR}/32 )&
( sleep 0.3; echo 3 ) | timeout 1 $MERGIN - ${TMPDIR}/31 ${TMPDIR}/32

exit 0
