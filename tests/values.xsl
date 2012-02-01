<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:exslt="http://exslt.org/common" version="1.0" exclude-result-prefixes="exslt">
    <xsl:output indent="yes"/>
    <xsl:key name="defines" match="define" use="@name"/>
    <xsl:template match="/">
        <xsl:variable name="doc">
            <xsl:apply-templates
                select="//define[generate-id() = generate-id(key('defines',@name)[1])]">
                <xsl:sort select="@name" data-type="text"/>
            </xsl:apply-templates>
        </xsl:variable>
        <defines>
            <xsl:for-each select="exslt:node-set($doc)//define">
                <define name="{@name}">
                    <xsl:apply-templates select="value">
                        <xsl:sort select="." data-type="text"/>
                    </xsl:apply-templates>
                </define>
            </xsl:for-each>
        </defines>
    </xsl:template>
    <xsl:template match="value">
        <xsl:if test="not(preceding-sibling::value = .)">
            <value>
                <xsl:value-of select="."/>
            </value>
        </xsl:if>
    </xsl:template>
    <xsl:template match="define">
        <xsl:variable name="name" select="@name"/>
        <define name="{@name}">
            <xsl:if test="not(contains(@name, 'Version'))">
                <xsl:apply-templates select="//define[@name = $name]" mode="defines"/>
            </xsl:if>
        </define>
    </xsl:template>
    <xsl:template match="define[pattern]" mode="defines">
        <xsl:apply-templates select="pattern"/>
    </xsl:template>
    <xsl:template match="define" mode="defines">
        <value>
            <xsl:choose>
                <xsl:when test="@value">
                    <xsl:value-of select="@value"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="."/>
                </xsl:otherwise>
            </xsl:choose>
        </value>
    </xsl:template>
    <xsl:template match="pattern">
        <xsl:choose>
            <xsl:when test="@type = 'regex'">
                <xsl:if test="not(contains(@value, '$'))">
                    <value>
                        <xsl:value-of select="@value"/>
                    </value>
                </xsl:if>
            </xsl:when>
            <xsl:otherwise>
                <value>
                    <xsl:value-of select="@value"/>
                </value>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
</xsl:stylesheet>
