#pragma once

#include <functional>

template<typename T>
struct  Property // with custom getter ad setter
{
public:
	Property() {}
	Property(std::function<T*()> getFunc, std::function<void(const T& value)> setFunc)
		: Get(getFunc), Set(setFunc) {}
	~Property() {}

	std::function<T*()> Get;
	std::function<void(const T& value)> Set;
};

template<typename T>
struct AutoProperty : public Property<T> // with default getter and setter
{
private:
	T _defaultBackingField;
public:
	AutoProperty(const T& defaultValue) : _defaultBackingField(defaultValue), Property<T>()
	{
		this->Get = [this]()->int*{return &_defaultBackingField;};
		this->Set = [this](const int& value) {_defaultBackingField = value;};
	}
};