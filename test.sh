#!/usr/bin/env bash

VAR_W=10
VAR_R=30

VAR_D=16
VAR_I="4K"
VAR_F="/tmp/io_uring_perf/test_file"
VAR_S="500M"

declare -A LOADER_ARGS=(
  ['round']="-r $VAR_R -w $VAR_W"
  ['timer']="-r $VAR_R -w $VAR_W"
)

declare -A TEST_ARGS=(
  ['fsync']=""
  ['nop']="-d $VAR_D"
  ['read']="-d $VAR_D -i $VAR_I -f $VAR_F -s $VAR_S"
  ['readv']="-d $VAR_D -i $VAR_I -f $VAR_F -s $VAR_S"
  ['write']="-d $VAR_D -i $VAR_I -f $VAR_F -s $VAR_S"
  ['writev']="-d $VAR_D -i $VAR_I -f $VAR_F -s $VAR_S"
  ['sqpoll-read']="-d $VAR_D -i $VAR_I -f $VAR_F -s $VAR_S"
  ['sqpoll-write']="-d $VAR_D -i $VAR_I -f $VAR_F -s $VAR_S"
)

cmake -S . -B cmake-build-debug/
make -C cmake-build-debug/ all

for l in "${!LOADER_ARGS[@]}"; do
  for t in "${!TEST_ARGS[@]}"; do
    echo "============================"
    echo "running : $l $t"
    sudo ./cmake-build-debug/${l}_${t} ${LOADER_ARGS[${l}]} ${TEST_ARGS[${t}]}
  done
done
