/*
'
* Distributed under the terms of the GNU General Public License v2
* Copyright 2023 Marius Norvoll Sletten
*
*/

/*
 * DgTypes.hpp
 *
 *      Author: marius
 */

#ifndef DGTYPES_HPP_
#define DGTYPES_HPP_
#include "DatagramHeader.h"
#include "XmlDatagram.hpp"
#include "DepthDatagram.h"
#include "FilterDatagram.h"
#include "IndexFileDatagram.h"
#include "MRUDatagram.h"
#include "SampleDatagram.h"
#include "TextDatagram.h"
#include "XmlDatagram.hpp"


typedef enum DgTypes
{

	xmlDatagram = '0LMX',
	textDatagram = '0GAT',
	sampleDatagram = '3WAR',
	filterDatagram = '1LIF',
	mru0Datagram ='0URM',
	mru1Datagram ='1URM',
	nmeaDatagram ='0EMN',
	noneDatagram = -1
}DgTypes;

enum class RawDType{
	Text,
	Xml0,
	Mru,
	Sample,
	Depth,
	Filter,
	Xml0Configuration,
	Xml0Init,
	Xml0Filt1,
	Xml0Filt2,
	Xml0Environment,
	Xml0Parameter,
	Mru1,
	Mru2,
	Sv,
	Pw,
	Angle,
	Phase,
	NumTypes,
	None = -1
};

#endif /* DGTYPES_HPP_ */
