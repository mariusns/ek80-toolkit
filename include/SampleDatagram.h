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

typedef enum class SampleDataType {
	Power = 0x0,
	Angle = 0x2,
	CFloat16 = 0x4,
	CFloat32 = 0x8,
	NumSamples = 0x7
};

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

typedef struct SampleDatagram
{
	//DatagramHeader DgHeader;
	char ChannelID[128];
	int16_t Datatype;
	char Spare[2];
	ULONG Offset;
	ULONG Count;
	BYTE Samples[];
}SampleDatagram,*pSampleDatagram;



#endif /* SAMPLEDATAGRAM_H_ */
