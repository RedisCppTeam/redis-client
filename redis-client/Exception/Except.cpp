/**
* Copyright (c) 2015, 爱wifi（版权声明）
*
* @file Except.cpp
* @brief Taiji 的基本异常类
*
*
* @author: yuhaiyang
* @date: 2016/4/11
*/

#include "Except.h"

namespace Taiji {






Except::Except(const std::string &errInfo) :
    _errInfo(errInfo)
{

}

Except::~Except()
{

}

const char *Except::what() const noexcept
{
    return _errInfo.c_str();
}






}

