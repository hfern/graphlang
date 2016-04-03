#pragma once
#include <string>
#include <memory>

enum class ValueType
{
	None,
	String,
	Number,
};

class NodeValue
{
	ValueType ty;
	union
	{
		double num;
		std::string* str;
	} u;

public:
	NodeValue();
	NodeValue(std::unique_ptr<std::string> strptr);
	NodeValue(double val);
	NodeValue(const NodeValue& other);
	~NodeValue();

	const std::string& get_string() const;
	double get_num() const;

	void set_to(std::unique_ptr<std::string> strptr);
	void set_to(double d);
};

