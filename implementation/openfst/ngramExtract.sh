#!/bin/bash

while getopts "n:" OPT
do
  case $OPT in
    n)N=${OPTARG};;
  esac
done
shift $(($OPTIND-1))

xz -cd ./data/nwc2010-ngrams/word/over999/${N}gms/${N}gm-0000.xz
