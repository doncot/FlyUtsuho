#include"MyException.h"

namespace Inferno{

FileNotFound::FileNotFound(const std::string& filename)
	: MyExceptionBase_RuntimeError("")
{
	m_message = "�G���[�F�t�@�C��\"" + filename + "\"������܂���B";
}

FileNotFound::FileNotFound(const char* filename)
	: MyExceptionBase_RuntimeError("")
{
	m_message = "�G���[�F�t�@�C��\"" + std::string(filename) + "\"������܂���B";
}

FileNotFound::FileNotFound(const std::wstring& filename)
	: MyExceptionBase_RuntimeError("")
{
	m_message_w = L"�G���[�F�t�@�C��\"" + filename + L"\"������܂���B";
}

FileNotFound::FileNotFound(const wchar_t* filename)
	: MyExceptionBase_RuntimeError("")
{
	m_message_w = L"�G���[�F�t�@�C��\"" + std::wstring(filename) + L"\"������܂���B";
}

const char* FileNotFound::what() const
{
	return m_message.c_str();
}

const wchar_t* FileNotFound::what_w() const
{
	return m_message_w.c_str();
}

FileNotFound::~FileNotFound() {}

}