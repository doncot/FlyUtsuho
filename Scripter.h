/*
Scripter.h
スクリプトエンジン
*/
#pragma once
#include<string>
#include<iostream>

namespace Inferno
{
class Scripter
{
public:
	void LoadSceneFromScript(const std::wstring& filename)
	{
		std::wifstream in(filename.c_str());
	}


};

}