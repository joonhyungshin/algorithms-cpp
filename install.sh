#!/bin/bash

BUILD=./build

mkdir $BUILD || { echo 'delete build directory and try again'; exit 1; }

for install in $(find ./snippets -name 'install.txt')
do
	codedir=$(dirname $install)
	cat $install | while read code
	do
		if [[ ! -z $code ]]
		then
			snippet=$BUILD/$code.sublime-snippet
			cat <<EOT >> $snippet
<snippet>
	<content><![CDATA[
EOT
			cat $codedir/$code.cpp >> $snippet
			cat <<EOT >> $snippet
]]></content>
	<tabTrigger>$code</tabTrigger>
	<scope>source.c++</scope>
</snippet>
EOT
		fi
	done
done

echo "snippets collected to $BUILD"

SUBLIME_PATH="$HOME/Library/Application Support/Sublime Text 3/Packages/User"
SNIPPET_DIR=algorithms-cpp
if [ -d "$SUBLIME_PATH" ]
then
	rm -rf "$SUBLIME_PATH/$SNIPPET_DIR"
	mkdir "$SUBLIME_PATH/$SNIPPET_DIR"
	cp $BUILD/* "$SUBLIME_PATH/$SNIPPET_DIR/"
	echo "snippets installed to $SUBLIME_PATH/$SNIPPET_DIR"
fi

