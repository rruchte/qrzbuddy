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
                            <xsl:apply-templates select="//Callsign" mode="jumbotron"/>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-12">
                            <table class="table table-bordered">
                                <thead>
                                    <tr>
                                        <th colspan="2">Details</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <xsl:apply-templates select="//Callsign" mode="details"/>
                                </tbody>
                            </table>
                        </div>
                    </div>
                </div>
            </body>
        </html>
    </xsl:template>
    <xsl:template match="Callsign" mode="jumbotron">
        <div class="jumbotron">
            <h1><xsl:value-of select="call"/></h1>
            <p class="lead"><xsl:value-of select="name_fmt"/></p>
            <hr />
            <p><xsl:value-of select="addr2"/>, <xsl:value-of select="state"/>, <xsl:value-of select="country"/></p>
        </div>
    </xsl:template>
    <xsl:template match="Callsign" mode="details">
                            <tr>
                                <th>Callsign</th>
                                <td>
                                    <xsl:value-of select="call"/>
                                </td>
                            </tr>
        <tr>
                                <th>First Name</th>
                                <td>
                                    <xsl:value-of select="fname"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Last Name</th>
                                <td>
                                    <xsl:value-of select="name"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Formatted Name</th>
                                <td>
                                    <xsl:value-of select="name_fmt"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Nickname</th>
                                <td>
                                    <xsl:value-of select="nickname"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Address</th>
                                <td>
                                    <xsl:value-of select="addr1"/>
                                </td>
                            </tr>
                            <tr>
                                <th>City</th>
                                <td>
                                    <xsl:value-of select="addr2"/>
                                </td>
                            </tr>
                            <tr>
                                <th>State</th>
                                <td>
                                    <xsl:value-of select="state"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Zip</th>
                                <td>
                                    <xsl:value-of select="zip"/>
                                </td>
                            </tr>
                            <tr>
                                <th>FIPS county identifier (USA)</th>
                                <td>
                                    <xsl:value-of select="fips"/>
                                </td>
                            </tr>
                            <tr>
                                <th>County</th>
                                <td>
                                    <xsl:value-of select="county"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Aliases</th>
                                <td>
                                    <xsl:value-of select="aliases"/>
                                </td>
                            </tr>
                            <tr>
                                <th>XRef</th>
                                <td>
                                    <xsl:value-of select="xref"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Country</th>
                                <td>
                                    <xsl:value-of select="country"/>
                                </td>
                            </tr>
                            <tr>
                                <th>DXCC country name</th>
                                <td>
                                    <xsl:value-of select="ccode"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Latitude</th>
                                <td>
                                    <xsl:value-of select="lat"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Longitude</th>
                                <td>
                                    <xsl:value-of select="lon"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Grid</th>
                                <td>
                                    <xsl:value-of select="grid"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Effective Date</th>
                                <td>
                                    <xsl:value-of select="efdate"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Expiration Date</th>
                                <td>
                                    <xsl:value-of select="expdate"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Previous Callsign</th>
                                <td>
                                    <xsl:value-of select="p_call"/>
                                </td>
                            </tr>
                            <tr>
                                <th>License Class</th>
                                <td>
                                    <xsl:value-of select="class"/>
                                </td>
                            </tr>
                            <tr>
                                <th>License type codes (USA)</th>
                                <td>
                                    <xsl:value-of select="codes"/>
                                </td>
                            </tr>
                            <tr>
                                <th>QSL manager info</th>
                                <td>
                                    <xsl:value-of select="qslmgr"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Email address</th>
                                <td>
                                    <xsl:value-of select="email"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Web page address</th>
                                <td>
                                    <xsl:value-of select="url"/>
                                </td>
                            </tr>
                            <tr>
                                <th>QRZ web page views</th>
                                <td>
                                    <xsl:value-of select="u_views"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Metro Service Area (USPS)</th>
                                <td>
                                    <xsl:value-of select="MSA"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Telephone Area Code (USA)</th>
                                <td>
                                    <xsl:value-of select="AreaCode"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Time Zone (USA)</th>
                                <td>
                                    <xsl:value-of select="TimeZone"/>
                                </td>
                            </tr>
                            <tr>
                                <th>GMT Time Offset</th>
                                <td>
                                    <xsl:value-of select="GMTOffset"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Daylight Saving Time Observed</th>
                                <td>
                                    <xsl:value-of select="DST"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Will accept e-qsl</th>
                                <td>
                                    <xsl:value-of select="eqsl"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Will return paper QSL</th>
                                <td>
                                    <xsl:value-of select="mqsl"/>
                                </td>
                            </tr>
                            <tr>
                                <th>CQ Zone identifier</th>
                                <td>
                                    <xsl:value-of select="cqzone"/>
                                </td>
                            </tr>
                            <tr>
                                <th>ITU Zone identifier</th>
                                <td>
                                    <xsl:value-of select="ituzone"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Operator's year of birth</th>
                                <td>
                                    <xsl:value-of select="born"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Managing callsign on QRZ</th>
                                <td>
                                    <xsl:value-of select="user"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Will accept LOTW</th>
                                <td>
                                    <xsl:value-of select="lotw"/>
                                </td>
                            </tr>
                            <tr>
                                <th>IOTA Designator</th>
                                <td>
                                    <xsl:value-of select="iota"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Source of lat/long data</th>
                                <td>
                                    <xsl:value-of select="geoloc"/>
                                </td>
                            </tr>
                            <tr>
                                <th>Attention address line</th>
                                <td>
                                    <xsl:value-of select="attn"/>
                                </td>
                            </tr>
                        </xsl:template>
    <xsl:template match="text()"/>
</xsl:stylesheet>