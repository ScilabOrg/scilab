<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:fo="http://www.w3.org/1999/XSL/Format"
                xmlns:latex="http://forge.scilab.org/p/jlatexmath"
                version='1.0'>

<xsl:include href="STYLE_DOC"/>

<xsl:template match="latex:*">
  <xsl:choose>
    <xsl:when test="@align != ''">
      <fo:block>
	<xsl:attribute name="text-align">
	  <xsl:value-of select="@align"/>
	</xsl:attribute>
	<fo:instream-foreign-object>
	  <xsl:copy>
	    <xsl:copy-of select="@*"/>
	    <xsl:if test="@align = 'center'">
	      <xsl:attribute name="fwidth">
		<xsl:value-of select="concat($page.width,',-',$body.start.indent,',-',$page.margin.inner,',-',$page.margin.outer)"/>
	      </xsl:attribute>
	    </xsl:if>
	    <xsl:apply-templates/>
	  </xsl:copy>
	</fo:instream-foreign-object>
      </fo:block>
    </xsl:when>
    <xsl:otherwise>
      <fo:instream-foreign-object>
	<xsl:copy>
	  <xsl:copy-of select="@*"/>
	  <xsl:apply-templates/>
	</xsl:copy>
      </fo:instream-foreign-object>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

</xsl:stylesheet>
