#ifndef ABSTRACTPL1167_H_
#define ABSTRACTPL1167_H_

#include <stdint.h>
#include <stdlib.h>

namespace device
{
	namespace milight
	{

		class AbstractPL1167 {
		public:
			virtual int open() = 0;

			virtual int setPreambleLength(uint8_t preambleLength) = 0;
			virtual int setSyncword(uint16_t syncword0, uint16_t syncword3) = 0;
			virtual int setTrailerLength(uint8_t trailerLength) = 0;
			virtual int setMaxPacketLength(uint8_t maxPacketLength) = 0;
			virtual int setCRC(bool crc) = 0;
			virtual int writeFIFO(const uint8_t data[], size_t data_length) = 0;
			virtual int transmit(uint8_t channel) = 0;
			virtual int receive(uint8_t channel) = 0;
			virtual int readFIFO(uint8_t data[], size_t &data_length) = 0;
		};
	}
}

#endif /* ABSTRACTPL1167_H_ */
