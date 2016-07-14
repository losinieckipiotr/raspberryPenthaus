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

		inline const std::list<std::string>& GetHello(){ return _helloMsg; }

		virtual std::list<std::string> Input(std::string& message, network::Session& session);

		void StandardOutput(const std::string line);
		void ErrorOutput(const std::string line);

	protected:
		std::list<std::string> _helloMsg;

	private:
		std::mutex _std_out_mutex;
		std::mutex _std_error_mutex;
	};
}

#endif // !STD_IO_H