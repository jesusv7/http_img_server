#include <thread>
#include <mutex>
#include <queue>

namespace thread_save
{
	template <class T>
	class Queue
	{
		std::queue<T> m_queue;
		std::mutex m_mutex;
	public:
		void push(T x)
		{
			std::lock_guard<std::mutex> lk(m_mutex);
			m_queue.push(x);
		}
		
		T pop()
		{
			std::lock_guard<std::mutex> lk(m_mutex);
			auto ret = m_queue.front();
			m_queue.pop();
			return ret;
		}
		
		bool empty() //if(!empty()) pop();
		{
			std::lock_guard<std::mutex> lk(m_mutex);
			return m_queue.empty();
		}
	};
}

