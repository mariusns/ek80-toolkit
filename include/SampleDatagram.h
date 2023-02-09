/*
 * SampleDatagram.h
 *
 *      Author: mariuss
 */

#ifndef SAMPLEDATAGRAM_H_
#define SAMPLEDATAGRAM_H_
#include <DatagramHeader.h>
#include <boost/cstdfloat.hpp>

/*
 * Datatype:
 *  Bit 0: Power
 *  Bit 1: Angle
 *  Bit 2: ComplexFloat16
 *  Bit 3: ComplexFloat32
 *  Bit 8-10: Number of Complex per Samples
 */

typedef enum{
	Power = 0x1,
	Angle = 0x2,
	ComplexFloat16 = 0x4,
	ComplexFloat32 = 0x8,
	NumSamples = 0x700
}SampleTypes_t;


typedef struct
{
	cfloat32_t sample;

}sample1_t;

typedef struct
{
	cfloat32_t sample[2];

}sample2_t;

typedef struct
{
	cfloat32_t sample[3];

}sample3_t;


typedef struct
{
	cfloat32_t sample[4];

}sample4_t;

typedef struct
{
	cfloat32_t sample[5];

}sample5_t;



typedef struct
{
	uint8_t Power;
	uint8_t Angle;
	uint8_t ComplexFloat16;
	uint8_t ComplexFloat32;
	uint8_t NumSamples;
}DataType_t;


typedef struct SampleDatagram
{
	char ChannelID[128];
	int16_t Datatype;
	char Spare[2];
	ULONG Offset;
	ULONG Count;
	BYTE Samples[];
}SampleDatagram,*pSampleDatagram;



#endif /* SAMPLEDATAGRAM_H_ */
