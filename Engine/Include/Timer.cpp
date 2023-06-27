#include "Timer.h"

CTimer::CTimer() : m_Second{}, m_Time{}, m_DeltaTime(0.f), m_FPS(0.f), m_FPSTime(0.f), m_Tick(0)
{
}

CTimer::~CTimer()
{
}

void CTimer::Init()
{
	QueryPerformanceFrequency(&m_Second);

	QueryPerformanceCounter(&m_Time);
}

void CTimer::Update()
{
	LARGE_INTEGER Time;
	QueryPerformanceCounter(&Time);

	m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / static_cast<float>(m_Second.QuadPart);

	m_Time = Time;

	m_FPSTime += m_DeltaTime;
	m_Tick++;

	if (m_Tick == 60)
	{
		m_FPS = m_Tick / m_FPSTime;
		m_FPSTime = 0.f;
		m_Tick = 0;
	}
}
