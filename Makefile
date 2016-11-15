#!/usr/bin/make -f
MKFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOTDIR := $(notdir $(patsubst %/,%,$(dir $(MKFILE))))
BUILDDIR ?= build

.PHONY:		default
default:	all

PACKAGE := expio
DESCRIPTION := A Library for experiments
VERSION := 0.2

include gnudirs.mk

SHELL = /bin/sh
CPP ?= cpp
CPPFLAGS ?=
CC ?= gcc
CXX ?= g++
AR ?= ar
CFLAGS ?= -O3 -std=gnu99 -fPIC
CXXFLAGS ?= -O3 -std=c++11 -fPIC
LDFLAGS ?=
LDLIBS ?=
ARFLAGS ?= rcs

REQUIREMENTS := sqlite3
OPTIONALS :=

include rules.mk
