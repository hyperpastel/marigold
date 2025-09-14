#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace marigold::util
{

template <typename T> class SafeQueue
{
	std::queue<T> m_queue;
	mutable std::mutex m_mutex;
	std::condition_variable m_cond;

  public:
	void push(T value);
	auto pop() -> T;
	auto empty() const -> bool;
};

template <typename T> void SafeQueue<T>::push(T value)
{
	const auto lock = std::lock_guard(m_mutex);
	m_queue.push(std::move(value));
	m_cond.notify_one();
}

template <typename T> auto SafeQueue<T>::pop() -> T
{
	auto lock = std::unique_lock(m_mutex);
	m_cond.wait(lock, [&] { return !m_queue.empty(); });
	const auto value = m_queue.front();
	m_queue.pop();
	return value;
}

template <typename T> auto SafeQueue<T>::empty() const -> bool
{
	const auto lock = std::lock_guard(m_mutex);
	return m_queue.empty();
}

} // namespace marigold::util
