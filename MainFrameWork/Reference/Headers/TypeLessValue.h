#pragma once


struct tagTypeLess    // Interface
{
	virtual void* GetValue() PURE;

	tagTypeLess() {};
	virtual ~tagTypeLess() {};
};

template<typename T>
struct tagTypeLessData : public tagTypeLess
{
public:
	tagTypeLessData() = default;
	virtual ~tagTypeLessData() {};
	tagTypeLessData(T _value) :value(_value) {}
	virtual void* GetValue() override { return &value; }
	void            SetValue(T _value) { value = _value; }

private:
	T value;
};

template<typename T>
struct tagTypeLessData<T*> : public tagTypeLess
{
public:
	tagTypeLessData() = default;
	virtual ~tagTypeLessData() 
	{
		Safe_Delete_Array(value);
	};
	tagTypeLessData(T* _value) :value(_value) {}
	virtual void* GetValue() override { return value; } // 이거 캐스팅 안되면 value 바로 가져오도록
	void            SetValue(T* _value) { value = _value; }

private:
	T* value;
};
