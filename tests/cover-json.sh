#!/bin/sh

cd $(dirname $0)
xsltproc cover.xsl cover.xml

