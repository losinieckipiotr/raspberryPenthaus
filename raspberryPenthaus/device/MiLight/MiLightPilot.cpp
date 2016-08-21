#include <stdexcept>
#include <cmath>

#include <iostream>

#include "MiLightPilot.h"
#include "../../config.h"

using namespace std;

using namespace device;
using namespace milight;

#ifdef RF24
MiLightPilot::MiLightPilot()
	:	RF24_(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_1MHZ),
		PL1167_nRF24_(RF24_),
		radio_(PL1167_nRF24_)
{

}
#else
void delay(unsigned int) { }

MiLightPilot::MiLightPilot()
	: RF24_(),
	PL1167_nRF24_(RF24_),
	radio_(PL1167_nRF24_)
{

}
#endif // RF24

MiLightPilot::~MiLightPilot()
{

}

void MiLightPilot::Init()
{
    radio_.begin();
}

void MiLightPilot::SendCmd(CMDS cmd, bool hold, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    switch (cmd)
    {
    case MiLightPilot::CMDS::ALL_ON:
    case MiLightPilot::CMDS::ALL_OFF:
    case MiLightPilot::CMDS::CH1_ON:
    case MiLightPilot::CMDS::CH1_OFF:
    case MiLightPilot::CMDS::CH2_ON:
    case MiLightPilot::CMDS::CH2_OFF:
    case MiLightPilot::CMDS::CH3_ON:
    case MiLightPilot::CMDS::CH3_OFF:
    case MiLightPilot::CMDS::CH4_ON:
    case MiLightPilot::CMDS::CH4_OFF:
    case MiLightPilot::CMDS::DISCO_SPEED_PLUS:
    case MiLightPilot::CMDS::DISCO_SPEED_MINUS:
    case MiLightPilot::CMDS::DISCO:
        frame_[BYTE_POS::CMD] = (hold) ? static_cast<uint8_t>(HOLD) : 0x00;
    case MiLightPilot::CMDS::SET_LIGHT:
    case MiLightPilot::CMDS::SET_COLOR:
        frame_[BYTE_POS::CMD] += static_cast<uint8_t>(cmd);
        break;
    default:
        return;
    }

    SendFromBuf(resends, seqLength, delayVal);
}

void MiLightPilot::SendFromBuf(const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    for (unsigned int i = 0; i < seqLength; ++i)
    {
        /*//print frame
        for (int k = 0; k < 7; ++k)
        {
            printf("%02X ", frame_[k]);
        }
        printf("\n");*/

        //copy frame to radio buffer and send
        radio_.write(frame_, 7);

        if (delayVal) delay(delayVal);

        //resend frame
        for (unsigned int j = 0; j < resends; ++j)
        {
            radio_.resend();
            if (delayVal) delay(delayVal);
        }

        //increment seq
        ++frame_[6];
    }
}

void MiLightPilot::SendFrame(const vector<uint8_t> &frame, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    if (frame.size() == 7 && seqLength > 0)
    {
        memcpy(frame_, frame.data(), 7);
        SendFromBuf(resends, seqLength, delayVal);
    }
}

void MiLightPilot::SendALL_ON(bool hold, const unsigned int resends , unsigned int seqLength, unsigned int delayVal)
{
    SetCmd(CMDS::ALL_ON, hold);
    SendFromBuf(resends, seqLength, delayVal);
}

void MiLightPilot::SendALL_OFF(bool hold, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    SetCmd(CMDS::ALL_OFF, hold);
    SendFromBuf(resends, seqLength, delayVal);
}

void MiLightPilot::SendCH1_ON(bool hold, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    SetCmd(CMDS::CH1_ON, hold);
    SendFromBuf(resends, seqLength, delayVal);
}

void MiLightPilot::SendCH1_OFF(bool hold, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    SetCmd(CMDS::CH1_OFF, hold);
    SendFromBuf(resends, seqLength, delayVal);
}

void MiLightPilot::SendCH2_ON(bool hold, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    SetCmd(CMDS::CH2_ON, hold);
    SendFromBuf(resends, seqLength, delayVal);
}

void MiLightPilot::SendCH2_OFF(bool hold, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    SetCmd(CMDS::CH2_OFF, hold);
    SendFromBuf(resends, seqLength, delayVal);
}

void MiLightPilot::SendCH3_ON(bool hold, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    SetCmd(CMDS::CH3_ON, hold);
    SendFromBuf(resends, seqLength, delayVal);
}

void MiLightPilot::SendCH3_OFF(bool hold, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    SetCmd(CMDS::CH3_OFF, hold);
    SendFromBuf(resends, seqLength, delayVal);
}

void MiLightPilot::SendCH4_ON(bool hold, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    SetCmd(CMDS::CH4_ON, hold);
    SendFromBuf(resends, seqLength, delayVal);
}

void MiLightPilot::SendCH4_OFF(bool hold, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    SetCmd(CMDS::CH4_OFF, hold);
    SendFromBuf(resends, seqLength, delayVal);
}
void MiLightPilot::SendDISCO_SPEED_PLUS(bool hold, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    SetCmd(CMDS::DISCO_SPEED_PLUS, hold);
    SendFromBuf(resends, seqLength, delayVal);
}

void MiLightPilot::SendDISCO_SPEED_MINUS(bool hold, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    SetCmd(CMDS::DISCO_SPEED_MINUS, hold);
    SendFromBuf(resends, seqLength, delayVal);
}
void MiLightPilot::SendDISCO(bool hold, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    SetCmd(CMDS::DISCO, hold);
    SendFromBuf(resends, seqLength, delayVal);
}

void MiLightPilot::SendSET_LIGHT(uint8_t light, const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    //TO DO: SAFE LIGHT VAL
    SetLight(light);
    SetCmd(CMDS::SET_LIGHT, false);
    SendFromBuf(resends, seqLength, delayVal);
}

void MiLightPilot::SendSET_COLOR(uint8_t color,const unsigned int resends, unsigned int seqLength, unsigned int delayVal)
{
    SetColor(color);
    SetCmd(CMDS::SET_COLOR, false);
    SendFromBuf(resends, seqLength, delayVal);
}

uint8_t MiLightPilot::GetLightVal(unsigned int lightInPerc)
{
    if (lightInPerc >= 100)
        return LIGHT_VALS::LIGHT_MAX;
    else if(lightInPerc == 0)
        return LIGHT_VALS::LIGHT_MIN;

    uint8_t t = static_cast<uint8_t>(round(lightInPerc / 4.0f));
    uint8_t v = 0xFF - 0x7E - t * 8;
    if(v < 0)
        v = 0xFF + 0x82 - t * 8;
    return v;
}
