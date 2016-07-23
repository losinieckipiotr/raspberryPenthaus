#ifndef IWRITEABLE_H
#define IWRITEABLE_H

namespace device
{
	class IWriteVal
	{
	public:
		virtual ~IWriteVal() { }
	};

	class IWriteable
	{
	public:
		virtual ~IWriteable() { }

		virtual void WriteDefault() = 0;
		virtual bool Write(IWriteVal* val) = 0;
	};
}
#endif // !IWRITEABLE_H
