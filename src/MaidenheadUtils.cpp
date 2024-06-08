#include <stdexcept>
#include "MaidenheadUtils.h"
 std::pair<double, double> MaidenheadUtils::grid2Deg(const std::string &maidenhead)
{
	if(maidenhead.length() < 6)
	{
		throw std::invalid_argument("Maidenhead locator must be at least 6 characters long.");
	}

	std::string grid = maidenhead;

	// Ensuring that the Maidenhead locator string is in the correct format
	grid[0] = toupper(grid[0]);
	grid[1] = toupper(grid[1]);
	grid[4] = tolower(grid[4]);
	grid[5] = tolower(grid[5]);

	double lon = -180.0 + 20.0 * (grid[0]-'A') + 2.0 * (grid[2]-'0') + 5.0/60.0 * (grid[4]-'a'+0.5);
	double lat = -90.0 + 10.0 * (grid[1]-'A') + 1.0 * (grid[3]-'0') + 2.5/60.0 * (grid[5]-'a'+0.5);

	return {lat, lon};
}

std::string MaidenheadUtils::deg2Grid(double latitude, double longitude)
{
	char maidenhead[6];

	// Adjustment for negative lat/lon
	longitude += 180;
	latitude += 90;

	// First pair (fields)
	maidenhead[0] = 'A' + static_cast<int>(longitude / 20);
	maidenhead[1] = 'A' + static_cast<int>(latitude / 10);

	// Leftover from field calculation
	longitude -= static_cast<int>(longitude / 20) * 20;
	latitude -= static_cast<int>(latitude / 10) * 10;

	// Second pair (square)
	maidenhead[2] = '0' + static_cast<int>(longitude / 2);
	maidenhead[3] = '0' + static_cast<int>(latitude);

	// Leftover from square calculation
	longitude -= static_cast<int>(longitude / 2) * 2;
	latitude -= static_cast<int>(latitude);

	// Third pair (subsquares)
	maidenhead[4] = 'a' + static_cast<int>(longitude * 12);
	maidenhead[5] = 'a' + static_cast<int>(latitude * 24);

	return std::string(maidenhead, 6);
}