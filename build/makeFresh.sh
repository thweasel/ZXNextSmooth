#!/bin/bash

rm -fr ./bin/*
rm -fr ./src/*

make

#tree -s ./bin ./src