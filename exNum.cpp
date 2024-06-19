#include "exNum.h"

void ExNum::SetMax(float value)
{
	m_Max = std::max(m_Min, value);
	if (m_Max < Get()) Set(m_Max);
}

void ExNum::SetMin(float value)
{
	m_Min = std::min(value, m_Max);
	if (Get() < m_Min) Set(m_Min);
}

void ExNum::Set(float value)
{
	value = std::min(value, m_Max);
	value = std::max(value, m_Min);
	m_Current = value;
}

float ExNum::Add(float value)
{
	Set(Get() + value);
	return Get();
}

float ExNum::AddMax(float value)
{
	SetMax(GetMax() + value);
	return GetMax();
}

float ExNum::AddMin(float value)
{
	SetMin(GetMin() + value);
	return GetMin();
}

float ExNum::rate()
{
	return (Get() / GetMax()); // Œ»Ý’l / Å‘å’l
}
