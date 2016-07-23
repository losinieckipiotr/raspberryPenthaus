#ifndef ITEMS_POOL_H
#define ITEMS_POOL_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <stdexcept>
#include <chrono>

template<class Item>
class ItemsPool
{
public:
	ItemsPool() : closeFlag_(false) { };
	ItemsPool(ItemsPool&) = delete;
	ItemsPool(ItemsPool&&) = delete;
	~ItemsPool() = default;
	ItemsPool& operator=(const ItemsPool&) = delete;

	void Push(const Item item)
	{
		std::unique_lock<std::mutex> lck(mt_);
		fifo_.push(item);
		lck.unlock();
		consume_.notify_one();
	}

	Item Pop()
	{
		std::unique_lock<std::mutex> lck(mt_);
		while (fifo_.empty())
			consume_.wait(lck);
		if (closeFlag_)
			throw std::runtime_error("Pool has been closed.");
		else
		{
			Item it = fifo_.front();
			fifo_.pop();
			lck.unlock();
			return it;
		}
	}

	Item Pop(unsigned int timeout)
	{
		std::unique_lock<std::mutex> lck(mt_);
		while (fifo_.empty())
		{
            consume_.wait_for
			(lck, std::chrono::milliseconds(timeout));
		}
		if (closeFlag_)
			throw std::runtime_error("Pool has been closed.");
		else
		{
			Item it = fifo_.front();
			fifo_.pop();
			lck.unlock();
			return it;
		}
	}

	void Close()
	{
		closeFlag_ = true;
		std::unique_lock<std::mutex> lck(mt_);
		while (!fifo_.empty())
			fifo_.pop();
		fifo_.push(Item());
		consume_.notify_all();
	}

	void Open()
	{
		std::unique_lock<std::mutex> lck(mt_);
		while (!fifo_.empty())
			fifo_.pop();
		closeFlag_ = false;
	}

private:

	std::queue<Item> fifo_;
	std::mutex mt_;
	std::condition_variable consume_;
	std::atomic<bool> closeFlag_;
};

#endif // ITEMS_POOL_H
