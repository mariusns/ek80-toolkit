/*
 * CurrentProfileStruct.h
 *
 *      Author: marius
 */

#ifndef CURRENTPROFILESTRUCT_H_
#define CURRENTPROFILESTRUCT_H_

#include <DatagramHeader.h>

extern "C" {

typedef struct CurrentProfileStruct
{
	short MessageId;
	short MessageVersion;
	ULONG PingTime;
	float StartRange;
	float SampleInterval;
	short NumberOfValues;
	bool IsVesselRelative;
	bool IsValidBottomData;
	float BottomVelocityNorth;
	float BottomVelocityEast;
	float VesselHeading;
}CurrentProfileStruct;


typedef struct CurrentSampleStruct
{
	float VelocityNorth;
	float VelocityEast;
	bool	ValidData;
}CurrentSampleStruct;

}


#endif /* CURRENTPROFILESTRUCT_H_ */
