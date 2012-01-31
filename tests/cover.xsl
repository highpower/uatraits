<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output indent="no" omit-xml-declaration="yes" method="text" encoding="UTF-8" media-type="text/x-json"/>
    <xsl:strip-space elements="*"/>
    <!--contant-->
    <xsl:variable name="d">0123456789</xsl:variable>
    
    <!-- ignore document text -->
    <xsl:template match="text()[preceding-sibling::node() or following-sibling::node()]"/>
    
    <!-- string -->
    <xsl:template match="text()">
        <xsl:call-template name="escape-string">
            <xsl:with-param name="s" select="."/>
        </xsl:call-template>
    </xsl:template>
    
    <!-- Main template for escaping strings; used by above template and for object-properties 
        Responsibilities: placed quotes around string, and chain up to next filter, escape-bs-string -->
    <xsl:template name="escape-string">
        <xsl:param name="s"/>
        <xsl:text>"</xsl:text>
        <xsl:call-template name="escape-bs-string">
            <xsl:with-param name="s" select="$s"/>
        </xsl:call-template>
        <xsl:text>"</xsl:text>
    </xsl:template>
    
    <!-- Escape the backslash (\) before everything else. -->
    <xsl:template name="escape-bs-string">
        <xsl:param name="s"/>
        <xsl:choose>
            <xsl:when test="contains($s,'\')">
                <xsl:call-template name="escape-quot-string">
                    <xsl:with-param name="s" select="concat(substring-before($s,'\'),'\\')"/>
                </xsl:call-template>
                <xsl:call-template name="escape-bs-string">
                    <xsl:with-param name="s" select="substring-after($s,'\')"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:call-template name="escape-quot-string">
                    <xsl:with-param name="s" select="$s"/>
                </xsl:call-template>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    
    <!-- Escape the double quote ("). -->
    <xsl:template name="escape-quot-string">
        <xsl:param name="s"/>
        <xsl:choose>
            <xsl:when test="contains($s,'&quot;')">
                <xsl:call-template name="encode-string">
                    <xsl:with-param name="s" select="concat(substring-before($s,'&quot;'),'\&quot;')"/>
                </xsl:call-template>
                <xsl:call-template name="escape-quot-string">
                    <xsl:with-param name="s" select="substring-after($s,'&quot;')"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:call-template name="encode-string">
                    <xsl:with-param name="s" select="$s"/>
                </xsl:call-template>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    
    <!-- Replace tab, line feed and/or carriage return by its matching escape code. Can't escape backslash
        or double quote here, because they don't replace characters (&#x0; becomes \t), but they prefix 
        characters (\ becomes \\). Besides, backslash should be seperate anyway, because it should be 
        processed first. This function can't do that. -->
    <xsl:template name="encode-string">
        <xsl:param name="s"/>
        <xsl:choose>
            <!-- tab -->
            <xsl:when test="contains($s,'&#x9;')">
                <xsl:call-template name="encode-string">
                    <xsl:with-param name="s" select="concat(substring-before($s,'&#x9;'),'\t',substring-after($s,'&#x9;'))"/>
                </xsl:call-template>
            </xsl:when>
            <!-- line feed -->
            <xsl:when test="contains($s,'&#xA;')">
                <xsl:call-template name="encode-string">
                    <xsl:with-param name="s" select="concat(substring-before($s,'&#xA;'),'\n',substring-after($s,'&#xA;'))"/>
                </xsl:call-template>
            </xsl:when>
            <!-- carriage return -->
            <xsl:when test="contains($s,'&#xD;')">
                <xsl:call-template name="encode-string">
                    <xsl:with-param name="s" select="concat(substring-before($s,'&#xD;'),'\r',substring-after($s,'&#xD;'))"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise><xsl:value-of select="$s"/></xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    
    <!--     
        No Number, No Boolean hinting
    <xsl:template match="text()[not(string(number())='NaN')]">
        <xsl:value-of select="."/>
    </xsl:template>
    -->
    <xsl:template match="text()[translate(.,'TRUE','true')='true']">true</xsl:template>
    <xsl:template match="text()[translate(.,'FALSE','false')='false']">false</xsl:template>
    
    
    <!-- item:null -->
    <xsl:template match="*[count(child::node())=0]">
        <xsl:call-template name="escape-string">
            <xsl:with-param name="s" select="@name"/>
        </xsl:call-template>
        <xsl:text>:null</xsl:text>
        <xsl:if test="following-sibling::*">,</xsl:if>
    </xsl:template>
    
    <!-- object -->
    <xsl:template match="*" name="base">
        <xsl:if test="not(preceding-sibling::*)">{</xsl:if>
        <xsl:call-template name="escape-string">
            <xsl:with-param name="s" select="@name"/>
        </xsl:call-template>
        <xsl:text>:</xsl:text>
        <xsl:apply-templates select="child::node()"/>
        <xsl:if test="following-sibling::*">,</xsl:if>
        <xsl:if test="not(following-sibling::*)">}</xsl:if>
    </xsl:template>
            
    <!-- convert root element to an anonymous container -->
    <xsl:template match="/">
        <xsl:apply-templates select="/tests/test"/>
    </xsl:template>
    
    <xsl:template match="test">
        <xsl:value-of select="question"/><xsl:text>&#0010;</xsl:text>
        <xsl:apply-templates select="answer/field"/><xsl:text>&#0010;</xsl:text>
    </xsl:template>
    
</xsl:stylesheet>