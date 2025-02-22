#ifndef QRZ_CALLSIGNCSVRENDERER_H
#define QRZ_CALLSIGNCSVRENDERER_H

#include "Renderer.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include "../Util.h"
#include "../model/Callsign.h"

namespace qrz::render
{
	/**
	 * @class CallsignCSVRenderer
	 * @brief The CallsignCSVRenderer class is responsible for rendering Callsign objects in CSV format.
	 *
	 * This class derives from the Renderer class and provides implementation for rendering Callsign objects into a CSV string.
	 */
	class CallsignCSVRenderer : public Renderer<Callsign>
	{
	public:
		/**
		 * @brief Renders Callsign objects in CSV format.
		 *
		 * This function takes a vector of Callsign objects and renders them in CSV format.
		 * The CSV format will contain information from each Callsign object, separated by commas.
		 * The first row of the CSV format will contain column headers.
		 *
		 * @param callsigns The vector of Callsign objects to render.
		 */
		std::string Render(const std::vector<Callsign> &callsigns) override
		{
			return generateCSV(callsigns);
		}

	private:
		/**
		 * @brief Generates a CSV string from a list of Callsign objects.
		 *
		 * This function takes a vector of Callsign objects and converts them into a CSV string format.
		 * The CSV string will contain information from each Callsign object, separated by commas.
		 * The first row of the CSV string will contain column headers.
		 *
		 * @param callsignList The vector of Callsign objects to generate a CSV string from.
		 * @return The CSV string representation of the Callsign objects.
		 */
		static std::string generateCSV(const std::vector<Callsign> &callsignList)
		{
			std::vector<std::vector<std::string>> rows;

			rows.push_back({
						"call",
						"xref",
						"aliases",
						"dxcc",
						"fname",
						"name",
						"addr1",
						"addr2",
						"state",
						"zip",
						"country",
						"ccode",
						"lat",
						"lon",
						"grid",
						"county",
						"fips",
						"land",
						"efdate",
						"expdate",
						"p_call",
						"class",
						"codes",
						"qslmgr",
						"email",
						"url",
						"u_views",
						"bio",
						"biodate",
						"image",
						"imageinfo",
						"serial",
						"moddate",
						"MSA",
						"AreaCode",
						"TimeZone",
						"GMTOffset",
						"DST",
						"eqsl",
						"mqsl",
						"cqzone",
						"ituzone",
						"born",
						"user",
						"lotw",
						"iota",
						"geoloc",
						"attn",
						"nickname",
						"name_fmt"
						   });

			for (const Callsign &callsign: callsignList)
			{
				rows.push_back({
					   callsign.getCall(),
					   callsign.getXref(),
					   callsign.getAliases(),
					   callsign.getDxcc(),
					   callsign.getFname(),
					   callsign.getName(),
					   callsign.getAddr1(),
					   callsign.getAddr2(),
					   callsign.getState(),
					   callsign.getZip(),
					   callsign.getCountry(),
					   callsign.getCodes(),
					   callsign.getLat(),
					   callsign.getLon(),
					   callsign.getGrid(),
					   callsign.getCounty(),
					   callsign.getFips(),
					   callsign.getLand(),
					   callsign.getEfdate(),
					   callsign.getExpdate(),
					   callsign.getPcall(),
					   callsign.getClass(),
					   callsign.getCodes(),
					   callsign.getQslmgr(),
					   callsign.getEmail(),
					   callsign.getUrl(),
					   std::to_string(callsign.getUViews()),
					   std::to_string(callsign.getBio()),
					   callsign.getBiodate(),
					   callsign.getImage(),
					   callsign.getImageinfo(),
					   callsign.getSerial(),
					   callsign.getModdate(),
					   callsign.getMsa(),
					   callsign.getAreaCode(),
					   callsign.getTimeZone(),
					   std::to_string(callsign.getGmtOffset()),
					   callsign.getDst(),
					   callsign.getEqsl(),
					   callsign.getMqsl(),
					   std::to_string(callsign.getCqzone()),
					   std::to_string(callsign.getItuzone()),
					   callsign.getBorn(),
					   callsign.getUser(),
					   callsign.getLotw(),
					   callsign.getIota(),
					   callsign.getGeoloc(),
					   callsign.getAttn(),
					   callsign.getNickname(),
					   callsign.getNameFmt(),
			   });
			}

			std::stringstream ss;

			for (const auto &row: rows)
			{
				ss << VectorToCSV(row) << '\n';
			}

			return ss.str();
		}
	};
}


#endif //QRZ_CALLSIGNCSVRENDERER_H
