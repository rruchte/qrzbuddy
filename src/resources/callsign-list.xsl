<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:param name="part"/>
    <xsl:output method="html" indent="yes"/>
    <xsl:template match="/">
        <html lang="en">
            <head>
                <meta charset="utf-8"/>
                <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no"/>
                <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css"
                      integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T"
                      crossorigin="anonymous"/>
            </head>
            <body>
                <div class="container">
                    <div class="row">
                        <div class="col-12">
                            <table class="table table-bordered">
                                <thead>
                                    <tr>
                                        <th>Callsign</th>
                                        <th>Name</th>
                                        <th>Class</th>
                                        <th>Address</th>
                                        <th>City</th>
                                        <th>State</th>
                                        <th>Country</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <xsl:apply-templates select="//Callsign" mode="select"/>
                                </tbody>
                            </table>
                        </div>
                    </div>
                </div>
            </body>
        </html>
    </xsl:template>
    <xsl:template match="Callsign" mode="select">
                            <tr>
                                <td>
                                    <xsl:value-of select="call"/>
                                </td>
                                <td>
                                    <xsl:value-of select="name_fmt"/>
                                </td>
                                <td>
                                    <xsl:value-of select="class"/>
                                </td>
                                <td>
                                    <xsl:value-of select="addr1"/>
                                </td>
                                <td>
                                    <xsl:value-of select="addr2"/>
                                </td>
                                <td>
                                    <xsl:value-of select="state"/>
                                </td>
                                <td>
                                    <xsl:value-of select="country"/>
                                </td>
                            </tr>
    </xsl:template>
    <xsl:template match="text()"/>
</xsl:stylesheet>