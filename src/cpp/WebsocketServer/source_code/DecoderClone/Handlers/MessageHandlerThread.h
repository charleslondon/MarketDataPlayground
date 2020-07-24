#pragma once
#include <thread>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "Abstract/IMessageHandler.h"

class MessageHandlerThread
{
public:
	MessageHandlerThread(std::shared_ptr<IMessageHandler> handler) :
		msgHandler(handler)
	{
		createThread();
	}

	~MessageHandlerThread() {}

	bool createThread()
	{
		if (m_thread == nullptr)
		{
			m_thread = std::make_unique<std::thread>(&MessageHandlerThread::Process, this);
		}

		return true;
	}

	void ProcessMessage(char data[])
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.push(data);
		m_cv.notify_one();
	}

private:
	std::unique_ptr<std::thread> m_thread;
	std::queue<char*> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::shared_ptr<IMessageHandler> msgHandler;

	MessageHandlerThread(const MessageHandlerThread&);
	MessageHandlerThread& operator=(const MessageHandlerThread&);

	void Process()
	{
		while (true)
		{
			char* msg = nullptr;

			std::unique_lock<std::mutex> lock(m_mutex);
			while (m_queue.empty())
			{
				m_cv.wait(lock);
			}

			if (!m_queue.empty())
			{
				msg = m_queue.front();
				m_queue.pop();
				msgHandler->handleMessage(msg);
			}
		}
	}
};