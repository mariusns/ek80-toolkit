/*
 * DgTypes.cpp
 *
 *      Author: marius
 */
#include "DgTypes.hpp"

/*
typedef enum RawDType{
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
*/

RawDType& operator++(RawDType& d)
{
	if(d == RawDType::NumTypes) return d;
	if(d == RawDType::None) return d;
	return d =(d == RawDType::Phase) ? RawDType::Text : static_cast<RawDType>(static_cast<int>(d) + 1);
}
