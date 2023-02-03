/*
 * DepthDatagram.h
 *
 *      Author: marius
 */

#ifndef DEPTHDATAGRAM_H_
#define DEPTHDATAGRAM_H_
#include <DatagramHeader.h>

extern "C" {

typedef struct ChannelDepth
{
	double Depth;
}ChannelDepth;


typedef struct DepthDatagram
{
	//DatagramHeader DgHeader;
	ULONG lTransducerCount;
	ChannelDepth Channel[];

}DepthDatagram;

}

#endif /* DEPTHDATAGRAM_H_ */
