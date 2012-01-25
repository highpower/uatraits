<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    version="1.0">
    <xsl:output method="text" indent="no"/>
    <xsl:template match="/"><xsl:apply-templates select="/tests/test"/></xsl:template>    
    <xsl:template match="test">{<xsl:for-each select="answer/field">'<xsl:value-of select="@name"/>': '<xsl:value-of select="."/>', </xsl:for-each>'UA': '<xsl:value-of select="question"/>'}
</xsl:template>
</xsl:stylesheet>