
#pragma once

#include <string>
#include <cstdlib>
#include <cassert>
#include <cctype>
#include <algorithm>

//表达式类
class Expression
{
	//表达式字符串
	std::string exp;
	//下一个字符位置
	size_t pt = 0;
	//是否已经被计算过
	bool calced;
	//计算出的值
	double val;

	//获得下一个字符
	char _next() const
	{
		return exp[pt];
	}
	//获得下一个字符并使字符位置加1
	char _pop()
	{
		++pt;
		assert(pt <= exp.size());
		return exp[pt - 1];
	}
	//处理数字串并返回一个double
	double _nextNum()
	{
		std::string num;
		//是否为数字或小数点
		while (isdigit(_next()) || _next() == '.')
		{
			num.push_back(_pop());
		}
		//前后逆转字符串，并不需要，这个是错的
		//d::reverse(num.begin(), num.end());
		//转换为double
		return strtod(num.c_str(), nullptr);
	}
	/*
	* 在递归树中共使用了两个函数，_calc和_next_exp
	* 其中：_calc是用来计算当前位置直到表达式末尾或下一个匹配括号的值
	*	   _calc中的参数flag是为了决定是否pop掉下一个括号
				，防止在递归调用_calc时pop掉其它字符
	*      _next_exp是用来计算下一个式子(数字或一个配对的括号)的值
	* 这样做的原因是为了引入优先级
	*/
	//递归计算表达式
	double _calc(int flag = 0)
	{
		//首先获得下一个表达式
		double current(_next_exp());
		//循环
		while (_next() != '\0' && _next() != ')')
		{
			//处理符号
			switch (_pop())
			{
				//对*/和+-的不同处理以体现优先级
			case'*':
				current *= _next_exp();
				break;
			case'/':
				current /= _next_exp();
				break;
			case'+':
				current += _calc(1);
			case'-':
				current -= _calc(1);
			default:
				break;
			}
		}
		//决定是否pop掉)
		if (!flag && _next() == ')')
		{
			_pop();
		}
		return current;
	}
	//计算下一个式子的值
	double _next_exp()
	{
		double current(0);
		//如果是数字则直接返回
		if (isdigit(_next()))
		{
			current = _nextNum();
		}
		//如果是数字则处理括号内内容
		else if (_next() == '(')
		{
			_pop();
			current = _calc();
		}
		return current;
	}

public:
	Expression() = default;
	explicit Expression(const std::string &str) :
		exp(str), calced(false), val(0)
	{
		//cpp11标准规定string以 \0 结尾
		//exp.push_back('\0');
	}
	explicit Expression(char str[]) :
		exp(str), calced(false), val(0)
	{

	}
	explicit Expression(const Expression &e) :
		exp(e.exp), calced(e.calced), val(e.val)
	{

	}
	void setString(const std::string &str)
	{
		//重置字符位置
		pt = 0;
		exp = str;
		//exp.push_back('\0');
		calced = false;
	}
	const std::string &getString() const
	{
		return exp;
	}
	const Expression &operator =(const std::string &str)
	{
		setString(str);
		return *this;
	}
	const Expression &operator =(const Expression &e)
	{
		setString(e.exp);
		if (e.calced)
		{
			val = e.val;
			calced = true;
		}
		return *this;
	}
	operator double()
	{
		return value();
	}
	size_t size() const
	{
		return exp.size();
	}
	double value()
	{
		if (exp.size() > 2000)
		{
			std::cerr << "Too long argument" << std::endl;
			return 0;
		}
		if (calced)
		{
			return val;
		}
		calced = true;
		return (val = _calc());
	}
};
