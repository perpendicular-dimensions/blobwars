#!/bin/sh
sed 's/ $//g' levelBrief \
	| grep -v '^@EOF@' \
	| sed 's/^[^[]/  &/' \
	| sed -E 's/^\[(.*)\]/'\''\1'\'': >/' \
	> levelBrief.yaml
