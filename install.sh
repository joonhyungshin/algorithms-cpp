#!/bin/bash

SOURCE_DIR=./snippets
BUILD_DIR=./build

mkdir $BUILD_DIR || { echo 'delete build directory and try again'; exit 1; }

for install in $(find $SOURCE_DIR -name 'install.txt')
do
	fullcodedir=$(dirname $install)
	codedir=${fullcodedir#$SOURCE_DIR/}
	cat $install | while read code
	do
		if [[ ! -z $code ]]
		then
			mkdir -p $BUILD_DIR/$codedir
			snippet=$BUILD_DIR/$codedir/$code.sublime-snippet
			cat <<EOT >> $snippet
<snippet>
	<content><![CDATA[
EOT
			cat $fullcodedir/$code.cpp >> $snippet
			cat <<EOT >> $snippet
]]></content>
	<tabTrigger>$code</tabTrigger>
	<scope>source.c++</scope>
</snippet>
EOT
			echo "collected $codedir/$code"
		fi
	done
done

echo "snippets collected to $BUILD_DIR"

SUBLIME_PATH="$HOME/Library/Application Support/Sublime Text 3/Packages/User"
SNIPPET_DIR=algorithms-cpp
if [ -d "$SUBLIME_PATH" ]
then
	rm -rf "$SUBLIME_PATH/$SNIPPET_DIR"
	mkdir "$SUBLIME_PATH/$SNIPPET_DIR"
	cp -r $BUILD_DIR/* "$SUBLIME_PATH/$SNIPPET_DIR/"
	echo "snippets installed to $SUBLIME_PATH/$SNIPPET_DIR"
fi

