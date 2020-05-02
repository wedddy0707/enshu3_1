#!/bin/bash

usage_exit() {
  echo "Usage: ${0} [-i isymbols] [-o osymbols] hoge.fst" 1>&2
}

while getopts "i:o:" OPT
do
  case $OPT in
    i)OPT_FLAG_I=1
      ISYMBOLS=${OPTARG};;
    o)OPT_FLAG_O=1
      OSYMBOLS=${OPTARG};;
  esac
done
shift $(($OPTIND-1))

if [ -n ${OPT_FLAG_I} ]; then
  if [ -n ${OPT_FLAG_O} ]; then
    fstdraw --isymbols=${ISYMBOLS} --osymbols=${OSYMBOLS} $1 ${1%%.fst}.dot
  else
    fstdraw --isymbols=${ISYMBOLS} $1 ${1%%.fst}.dot
  fi
else
  if [ -n ${OPT_FLAG_O} ]; then
    fstdraw --osymbols=${OSYMBOLS} $1 ${1%%.fst}.dot
  else
    fstdraw $1 ${1%%.fst}.dot
  fi
fi

dot -Tpng ${1%%.fst}.dot -o ${1%%.fst}.png -Gdpi=1000
