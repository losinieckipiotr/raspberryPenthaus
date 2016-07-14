#ifndef MY_FRAME_H
#define MY_FRAME_H
#include <array>
#include <exception>
#include <sstream>
#include <cctype>

namespace io
{
	namespace network
	{
		namespace transport
		{
			class MyFrame
			{
			public:
				MyFrame() : _message_length(0) { }
				~MyFrame() { }

				//zwraca wskaznik poczatku ramki
				inline char* BeginData()
				{
					
					return _data.data();
				}

				//zwraca wskaznik konca ramki
				inline char* EndData()
				{
					return _data.data() + frameSize;
				}

				//dekoduje i zwraca wiadomosc zawarta w ramce
				inline std::string GetMsg()
				{
					try
					{
						//utworzenie stringa z naglowka
						std::string header(_data.data(), headerLength);
						//zdekodowanie dlugosci ramki
						_message_length = _StrToUint8(header);
						//zwrocenie wiadomosci zawartej w ramce
						return std::string(_data.data() + headerLength, _message_length);
					}
					catch (std::exception&)
					{
						//blad zwroc pusty string
						return "";
					}
				}

				//zwraca dlugosc wiadomosci w ramce
				inline unsigned short int MsgLength()
				{
					return _message_length;
				}

				//laduje wiadomosc i koduje naglowek ramki
				inline bool LoadMessage(const std::string& message)
				{
					try
					{
						//sprawdzenie czy nie przekroczono maksymalnego rozmiaru ramki
						if (message.size() > maxMessageSize)
							return false;
						//wyzerowanie ramki
						_data.fill('\0');
						//zapisanie rozmiaru ramki
						_message_length = (uint8_t)message.size();
						//zakodowanie rozmiaru ramki
						std::string header = _Uint8ToHexStr(_message_length);
						//skopiowanie naglowka do ramki (naglowek moze miec rozmiar 1 lub 2)
						std::memcpy(_data.data(), header.data(), header.size());
						//skopiowanie wiadomosci do ramki
						std::memcpy(_data.data() + headerLength,
							message.data(), _message_length);
						return true;
					}
					catch (std::exception&)
					{
						//wystapil blad, ustawienie dlugosci na zero 
						_message_length = 0;
						//wyzerowanie ramki
						_data.fill('\0');
						//zwrocenie niepowodzenia
						return false;
					}
				}

				static const unsigned short int headerLength = 2U;
				static const unsigned short int maxMessageSize = 255U;
				static const unsigned short int frameSize = headerLength + maxMessageSize;

			private:
				//konwersja liczby 8 bitowej na string zakodowany szesnastkowo
				inline std::string _Uint8ToHexStr(uint8_t val)
				{
					std::string s;
					uint8_t t = val % 16;
					if (t < 10)
						s = t + 48;
					else
						s = t + 87;
					val /= 16;
					if (val != 0)
					{
						t = val % 16;
						if (t < 10)
							return (char(t + 48)) + s;
						else
							return (char(t + 87)) + s;
					}
					return s;
				}

				//konwersja stringa zakodowaniego szesnastkowo na liczbe 8 bitowa
				//sprawdza tylko i wylacznie 2 pierwsze znaki, jezeli nie sa znaki szesnastkowe,
				//zwrocone bedzie zero
				inline uint8_t _StrToUint8(const std::string& s)
				{
					uint8_t t = s[0];
					if (!isxdigit(t))
						return 0;
					uint8_t t2 = s[1];
					if (isdigit(t))
						t -= 48;
					else
						t -= 87;
					if (t2 != '\0')
					{
						if (!isxdigit(t2))
							return 0;
						t *= 16;
						if (isdigit(t2))
							t2 -= 48;
						else
							t2 -= 87;
						t += t2;
					}
					return t;
				}

				uint8_t _message_length;
				std::array<char, frameSize> _data;
			};
		}
	}
}

#endif // !MY_FRAME_H