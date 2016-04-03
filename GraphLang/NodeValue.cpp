#include "stdafx.h"
#include "NodeValue.h"
#include <cassert>

using namespace std;

NodeValue::NodeValue()
{
	ty = ValueType::None;
	u.num = 0;
}

NodeValue::NodeValue(std::unique_ptr<std::string> str)
{
	ty = ValueType::String;
	u.str = str.release();
}

NodeValue::NodeValue(double val)
{
	ty = ValueType::Number;
	u.num = val;
}

NodeValue::NodeValue(const NodeValue& other)
{
	ty = other.ty;
	switch(other.ty)
	{
	case ValueType::Number:
		u.num = other.u.num;
		break;
	case ValueType::String:
		u.str = new std::string(*other.u.str);
		break;
	}
}

NodeValue::~NodeValue()
{
	switch (ty)
	{
	case ValueType::String:
		delete u.str;
		break;
	}
}

const std::string& NodeValue::get_string() const
{
	assert(ty == ValueType::String);
	return *u.str;
}

double NodeValue::get_num() const
{
	assert(ty == ValueType::Number);
	return u.num;
}

void NodeValue::set_to(std::unique_ptr<std::string> str)
{
	if (ty == ValueType::String)
	{
		delete u.str;
	}

	ty = ValueType::String;
	u.str = str.release();
}

void NodeValue::set_to(double d)
{
	if (ty == ValueType::String)
	{
		delete u.str;
	}
	ty = ValueType::Number;
	u.num = d;
}