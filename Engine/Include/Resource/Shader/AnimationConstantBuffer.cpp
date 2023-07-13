#include "AnimationConstantBuffer.h"

CAnimationConstantBuffer::CAnimationConstantBuffer() : m_BufferData{}
{
}

CAnimationConstantBuffer::CAnimationConstantBuffer(const CAnimationConstantBuffer& Buffer) : CConstantBufferData(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CAnimationConstantBuffer::~CAnimationConstantBuffer()
{
}

bool CAnimationConstantBuffer::Init()
{
	SetConstantBuffer("Animation");

	return true;
}

void CAnimationConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CAnimationConstantBuffer* CAnimationConstantBuffer::Clone()
{
	return new CAnimationConstantBuffer(*this);
}
