#pragma once

#ifndef STRFORMAT_H
#define STRFORMAT_H

#include <memory>
#include <iostream>
#include <string>
#include <cstdio>

//using namespace std; //Don't if you're in a header-file

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[size]);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

#endif