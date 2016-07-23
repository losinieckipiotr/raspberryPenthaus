#ifndef IREADABLE_H
#define IREADABLE_H

namespace device
{
	class IReadVal
	{
	public:
		virtual ~IReadVal() { }
	};

	class IReadable
	{
	public:
		virtual ~IReadable() { }

		virtual IReadVal& Read() = 0;
		virtual unsigned int GetReadInterval() = 0;
	};
}

#endif // !IREADABLE_H
