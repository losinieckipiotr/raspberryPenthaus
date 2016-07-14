#ifndef ITO_STRING_H
#define ITO_STRING_H
#include <string>

//#include <chrono>
//#include <ctime>
//static std::string TimeToString(std::chrono::time_point<std::chrono::system_clock>& time)
//{
//	time_t point;
//	point = std::chrono::system_clock::to_time_t(time);
//	std::string s(ctime(&point));
//	s.resize(s.length() - 1);
//	return s;
//}

namespace print
{
	class IToString
	{
	public:
		virtual ~IToString() { }

		static std::string BoolToString(bool b)
		{
			if (b)
				return std::string("true");
			else
				return std::string("false");
		}

		virtual std::string ToString() const = 0;
	};
}

#endif // !ITO_STRING_H


