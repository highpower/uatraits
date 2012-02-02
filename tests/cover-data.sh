#!/bin/sh

cd $(dirname $0)
xsltproc cover-data.xsl cover.xml > cover-data2.txt

