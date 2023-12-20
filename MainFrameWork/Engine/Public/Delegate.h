#pragma once

template <class... Args>
class FDelegate
{
public:
	typedef typename std::list<std::function<void(Args...)>>::iterator iterator;

	void operator () (const Args... args)
	{
		if (functions.empty())
			return;

		for (auto iter = functions.begin(); iter != functions.end();)
		{

			if (!(*iter))
				iter = functions.erase(iter);
			else
			{
				(*iter)(args...);
				iter++;
			}
		}
	}

	FDelegate& operator = (std::function<void(Args...)> const& func)
	{
		functions.clear();
		functions.push_back(func);
		return *this;
	}

	FDelegate& operator += (std::function<void(Args...)> const& func)
	{
		functions.push_back(func);

#ifdef _DEBUG
		if (functions.size() % 100 == 0)
			cout << "Warning: " << functions.size() << " functions have been bound to Delegate." << endl;
#endif // _DEBUG

		

		return *this;
	}

	FDelegate& operator -= (std::function<void(Args...)> const& func)
	{
		void(* const* func_ptr)(Args...) = func.template target<void(*)(Args...)>();
		const std::size_t func_hash = func.target_type().hash_code();

		if (nullptr == func_ptr)
		{
			for (auto iter = functions.begin(); iter != functions.end(); iter++)
			{
				if (func_hash == (*iter).target_type().hash_code())
				{
					functions.erase(iter);
					return *this;
				}
			}
		}

		else
		{
			for (auto iter = functions.begin(); iter != functions.end(); iter++)
			{
				void(* const* delegate_ptr)(Args...) = (*iter).template target<void(*)(Args...)>();
				if (nullptr != delegate_ptr && *func_ptr == *delegate_ptr)
				{
					functions.erase(iter);
					return *this;
				}
			}
		}

		return *this;
	}

	bool empty()		{ return functions.empty(); }
	size_t size()		{ return functions.size(); }
	iterator begin()	{ return functions.begin(); }
	iterator end()		{ return functions.end(); }
	void clear()		{ functions.clear(); }

private:
	std::list<std::function<void(Args...)>> functions;
};