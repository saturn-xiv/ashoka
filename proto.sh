#!/bin/sh

PROTOBUF_VERSION="3.13.0"
PROTOBUF_HASH="d3058fa461095e3b567f10d3a72b4fac1de53cda"
PROTOBUF_HOME=$HOME/.conan/data/protobuf/$PROTOBUF_VERSION/jeremy/local/package/$PROTOBUF_HASH

echo "using $PROTOBUF_HOME"
$PROTOBUF_HOME/bin/protoc -I $PROTOBUF_HOME/include -I protos  --cpp_out=src protos/*.proto

echo 'done.'