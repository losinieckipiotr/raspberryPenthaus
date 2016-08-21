#ifndef MI_LIGHT_PILOT_H
#define MI_LIGHT_PILOT_H

#include <vector>
#include <cstring>

#include "../../config.h"

#ifdef WP
#include "MiLightRadio.h"
#include "PL1167_nRF24.h"
#else
class MyRadio
{
public:
    MyRadio() { }

    int begin() { return 0; }
    bool available() { return true; }
    int read(uint8_t frame[], size_t &frame_length) { return 0; }
    int dupesReceived() { return 0; }
    int write(uint8_t frame[], size_t frame_length) { return 0; }
    int resend() { return 0; }
};
#endif // WP

namespace device
{
	namespace milight
	{
		class MiLightPilot
		{
		public:
			enum CMDS
			{
				ALL_ON = 0x01,
				ALL_OFF = 0x02,
				CH1_ON = 0x03,
				CH1_OFF = 0x04,
				CH2_ON = 0x05,
				CH2_OFF = 0x06,
				CH3_ON = 0x07,
				CH3_OFF = 0x08,
				CH4_ON = 0x09,
				CH4_OFF = 0x0A,
				DISCO_SPEED_PLUS = 0x0B,
				DISCO_SPEED_MINUS = 0x0C,
				DISCO = 0x0D,
				SET_LIGHT = 0x0E,
				SET_COLOR = 0x0F,
			};

			enum COLORS
			{
				RED = 0x1E,
				BLUE = 0xB9,
				GREEN = 0x69,
				YELLOW = 0x40
			};

			enum LIGHT_VALS
			{
				LIGHT_MIN = 0x81,
				LIGHT_MAX = 0xB9
			};

			enum
			{
				HOLD = 0x10
			};

			enum BYTE_POS
			{
				ID = 0,
				COLOR = 3,
				LIGHT = 4,
				CMD = 5,
				SEQ = 6
			};

			MiLightPilot();
			virtual ~MiLightPilot();

			void Init();

			inline void SetID(std::vector<uint8_t> id) { if (id.size() == 3) memcpy(frame_ + BYTE_POS::ID, id.data(), 3); }
			inline void SetColor(uint8_t color) { frame_[BYTE_POS::COLOR] = color; }
			inline void SetLight(uint8_t light) { frame_[BYTE_POS::LIGHT] = light; }
			inline void SetLightInPerc(unsigned int light) { frame_[BYTE_POS::LIGHT] = GetLightVal(light); }
			inline void SetCmd(CMDS cmd, bool hold = false) { frame_[BYTE_POS::CMD] = (hold) ? static_cast<uint8_t>(HOLD + cmd) : static_cast<uint8_t>(cmd); }
			inline void SetSeq(uint8_t seq) { frame_[BYTE_POS::SEQ] = seq; }

			void SendCmd(CMDS cmd, bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendFromBuf(const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendFrame(const std::vector<uint8_t> &frame, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);

			void SendALL_ON(bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendALL_OFF(bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendCH1_ON(bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendCH1_OFF(bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendCH2_ON(bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendCH2_OFF(bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendCH3_ON(bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendCH3_OFF(bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendCH4_ON(bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendCH4_OFF(bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendDISCO_SPEED_PLUS(bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendDISCO_SPEED_MINUS(bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendDISCO(bool hold = false, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);

			void SendSET_LIGHT(uint8_t light, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);
			void SendSET_COLOR(uint8_t color, const unsigned int resends = 0, unsigned int seqLength = 32, unsigned int delayVal = 0);

			uint8_t GetLightVal(unsigned int lightInPerc);

		protected:
			uint8_t frame_[7];

		private:
            #ifdef WP
			RF24 RF24_;
			PL1167_nRF24 PL1167_nRF24_;
			MiLightRadio radio_;
			#else
			MyRadio radio_;
			#endif // WP
		};
	}
}

#endif // MI_LIGHT_PILOT_H
