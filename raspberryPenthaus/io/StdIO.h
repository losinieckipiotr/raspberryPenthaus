#ifndef STD_IO_H
#define STD_IO_H
#include <list>
#include <string>
#include <mutex>

namespace io
{
	namespace network
	{
		class Session;
	}

	class StdIO
	{
	public:
		StdIO();
		virtual ~StdIO();

		static void StandardOutput(const std::string& line);
		static void ErrorOutput(const std::string& line);

		inline const std::list<std::string>& GetHello(){ return _helloMsg; }

		virtual std::list<std::string> Input(std::string& message, network::Session& session);

	protected:
		std::list<std::string> _helloMsg;

	private:
		static std::mutex _std_out_mutex;
		static std::mutex _std_error_mutex;
	};
}

#endif // !STD_IO_H