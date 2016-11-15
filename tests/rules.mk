d		:= $(dir)
# Subdirectories, in random order

dir	:= $(d)/log
include		$(dir)/rules.mk

dir	:= $(d)/fqdn
include		$(dir)/rules.mk

dir	:= $(d)/stats
include		$(dir)/rules.mk

dir	:= $(d)/stats_tsv
include		$(dir)/rules.mk

dir	:= $(d)/stats_sqlite3
include		$(dir)/rules.mk
