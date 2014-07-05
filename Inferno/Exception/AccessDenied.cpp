#include"MyException.h"

namespace Inferno{

AccessDenied::AccessDenied(const std::string& cause)
	: MyExceptionBase_RuntimeError("")
{
	m_message = cause + "�ւ̃A�N�Z�X���ł��܂���ł����B";
}

AccessDenied::AccessDenied(const char* cause)
	: MyExceptionBase_RuntimeError("")
{
	m_message = std::string(cause) + "�ւ̃A�N�Z�X���ł��܂���ł����B";
}

AccessDenied::AccessDenied(const std::wstring& cause)
	: MyExceptionBase_RuntimeError("")
{
	m_message_w = cause + L"�ւ̃A�N�Z�X���ł��܂���ł����B";
}

AccessDenied::AccessDenied(const wchar_t* cause)
	: MyExceptionBase_RuntimeError("")
{
	m_message_w = std::wstring(cause) + L"�ւ̃A�N�Z�X���ł��܂���ł����B";
}

const char* AccessDenied::what() const
{
	return m_message.c_str();
}

const wchar_t* AccessDenied::what_w() const
{
	return m_message_w.c_str();
}

AccessDenied::~AccessDenied() {}

}