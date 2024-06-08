#ifndef QRZBUDDY_MAIDENHEADUTILS_H
#define QRZBUDDY_MAIDENHEADUTILS_H


#include <utility>
#include <string>

class MaidenheadUtils
{
public:
	MaidenheadUtils(){};
	// Converts Maidenhead grid locator to degrees of West longitude and North latitude
	static std::pair<double, double> grid2Deg(const std::string &maidenhead);
	static std::string deg2Grid(double latitude, double longitude);
};


#endif //QRZBUDDY_MAIDENHEADUTILS_H
