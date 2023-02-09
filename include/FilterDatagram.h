/*
 * FilterDatagram.h
 *
 *      Author: marius
 */

#ifndef FILTERDATAGRAM_H_
#define FILTERDATAGRAM_H_

#include <DatagramHeader.h>

extern "C"
{

enum FilterType {
	FilterTypeLowNoise = 0,
	FilterTypeLowResolution = 1,
	FilterTypeStandardResolution = 2,
	FilterTypeHighResolution = 3,
	FilterTypeFullResolution = 4,
	FilterTypeHydrophone = 5,
	FilterTypeHighResolutionReducedAttenuation = 6
};

typedef struct FilterDatagram
{

	//DatagramHeader DgHeader;
	short	Stage;
	char	Spare[2];
	char	FilterType;
	char	ChannelId[128];
	short	NoOfCoefficients;
	short	DecimationFactor;
	float	Coefficients[];

}FilterDatagram,*pFilterDatagram;


}



#endif /* FILTERDATAGRAM_H_ */
