#pragma once

template <class... Args>
class FDelegate
{
public:
	typedef typename std::list<std::function<void(Args...)>>::iterator iterator;

	void operator () (const Args... args)
	{
		if (m_listFunctions.empty())
			return;

		for (auto iter = m_listFunctions.begin(); iter != m_listFunctions.end();)
		{

			if (!(*iter))
				iter = m_listFunctions.erase(iter);
			else
			{
				(*iter)(args...);
				iter++;
			}
		}
	}

	FDelegate& operator = (std::function<void(Args...)> const& func)
	{
		m_listFunctions.clear();
		m_listFunctions.push_back(func);
		return *this;
	}

	FDelegate& operator += (std::function<void(Args...)> const& func)
	{
		m_listFunctions.push_back(func);

#ifdef _DEBUG
		if (m_listFunctions.size() % 100 == 0)
			cout << "Warning: " << m_listFunctions.size() << " m_listFunctions have been bound to Delegate." << endl;
#endif // _DEBUG

		return *this;
	}

	FDelegate& operator -= (std::function<void(Args...)> const& func)
	{
		void(* const* func_ptr)(Args...) = func.template target<void(*)(Args...)>();
		const std::size_t func_hash = func.target_type().hash_code();

		if (nullptr == func_ptr)
		{
			for (auto iter = m_listFunctions.begin(); iter != m_listFunctions.end(); iter++)
			{
				if (func_hash == (*iter).target_type().hash_code())
				{
					m_listFunctions.erase(iter);
					return *this;
				}
			}
		}

		else
		{
			for (auto iter = m_listFunctions.begin(); iter != m_listFunctions.end(); iter++)
			{
				void(* const* delegate_ptr)(Args...) = (*iter).template target<void(*)(Args...)>();
				if (nullptr != delegate_ptr && *func_ptr == *delegate_ptr)
				{
					m_listFunctions.erase(iter);
					return *this;
				}
			}
		}

		return *this;
	}

	bool empty()		{ return m_listFunctions.empty(); }
	size_t size()		{ return m_listFunctions.size(); }
	iterator begin()	{ return m_listFunctions.begin(); }
	iterator end()		{ return m_listFunctions.end(); }
	void clear()		{ m_listFunctions.clear(); }

private:
	std::list<std::function<void(Args...)>> m_listFunctions;
};