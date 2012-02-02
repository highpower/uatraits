<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output indent="no" omit-xml-declaration="yes" method="text" encoding="UTF-8" media-type="text/plain"/>
    <xsl:strip-space elements="*"/>

    <xsl:template match="/">
        <xsl:apply-templates select="/tests/test"/>
    </xsl:template>
    
    <xsl:template match="test">
        <xsl:value-of select="question"/><xsl:text>&#0010;</xsl:text>
    </xsl:template>
    
</xsl:stylesheet>