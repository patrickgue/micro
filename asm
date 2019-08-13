#!/bin/sh

ASMC=$PWD/asmc/asmc
ASMS=$PWD/asms/asms
TMP_FILE=./tmp$(date +%s).asms

$ASMC $1 $TMP_FILE
$ASMS $TMP_FILE $2
rm $TMP_FILE
