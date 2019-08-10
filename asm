#!/bin/bash

ASMC=$PWD/asmc/asmc
ASMS=$PWD/asms/asms

$ASMC $1 /tmp/a
$ASMS /tmp/a $2
