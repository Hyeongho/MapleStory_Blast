#include "EditorInput.h"

CEditorInput::CEditorInput() : m_Text{}, m_wText{}, m_TextUTF8{}, m_HintText{}, m_HintwText{}, m_HintTextUTF8{}, m_InputType(EImGuiInputType::String), m_MultiLine(false), m_Flag(0), m_ValueInt(0), m_ValueFloat(0.f)
{
}

CEditorInput::~CEditorInput()
{
}

void CEditorInput::SetText(const char* Text)
{
	memset(m_Text, 0, 1024);
	memset(m_wText, 0, 2048);
	memset(m_TextUTF8, 0, 1024);

	strcpy_s(m_Text, Text);

	int Length = (int)MultiByteToWideChar(CP_ACP, 0, Text, -1, nullptr, 0);

	MultiByteToWideChar(CP_ACP, 0, Text, -1, m_wText, Length);

	Length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, nullptr, 0, nullptr, nullptr);

	WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, m_TextUTF8, Length, nullptr, nullptr);
}

void CEditorInput::SetHintText(const char* Text)
{
	memset(m_HintText, 0, 1024);
	memset(m_HintwText, 0, 2048);
	memset(m_HintTextUTF8, 0, 1024);

	strcpy_s(m_HintText, Text);

	int Length = (int)MultiByteToWideChar(CP_ACP, 0, Text, -1, nullptr, 0);

	MultiByteToWideChar(CP_ACP, 0, Text, -1, m_HintwText, Length);

	Length = WideCharToMultiByte(CP_UTF8, 0, m_HintwText, -1, nullptr, 0, nullptr, nullptr);

	WideCharToMultiByte(CP_UTF8, 0, m_HintwText, -1, m_HintTextUTF8, Length, nullptr, nullptr);
}

void CEditorInput::SetInt(int Value)
{
	m_ValueInt = Value;
}

void CEditorInput::SetFloat(float Value)
{
	m_ValueFloat = Value;
}

void CEditorInput::SetMultiLine(bool MultiLine)
{
	m_MultiLine = MultiLine;
}

void CEditorInput::AddFlag(ImGuiInputTextFlags_ Flag)
{
	m_Flag |= Flag;
}

void CEditorInput::ReadOnly(bool Enable)
{
	if (Enable)
	{
		m_Flag |= ImGuiInputTextFlags_ReadOnly;
	}

	else if (m_Flag & ImGuiInputTextFlags_ReadOnly)
	{
		m_Flag ^= ImGuiInputTextFlags_ReadOnly;
	}
}

void CEditorInput::PasswordMode(bool Enable)
{
	if (Enable)
	{
		m_Flag |= ImGuiInputTextFlags_Password;
	}

	else if (m_Flag & ImGuiInputTextFlags_Password)
	{
		m_Flag ^= ImGuiInputTextFlags_Password;
	}
}

void CEditorInput::SetInputType(EImGuiInputType Type)
{
	m_InputType = Type;
}

bool CEditorInput::Empty() const
{
	return strlen(m_Text) == 0;
}

bool CEditorInput::Init()
{
	return true;
}

void CEditorInput::Render()
{
	ImGui::PushItemWidth(m_Size.x);

	bool Input = false;

	if (m_MultiLine)
	{
		Input = ImGui::InputTextMultiline(m_Name.c_str(), m_Text, 1024, m_Size, m_Flag);
	}

	else
	{
		switch (m_InputType)
		{
		case EImGuiInputType::String:
			// HintText가 있을 경우와 없을 경우로 나눈다.
			if (strlen(m_HintText) > 0)
			{
				Input = ImGui::InputTextWithHint(m_Name.c_str(), m_HintTextUTF8, m_Text, 1024, m_Flag);
			}

			else
			{
				Input = ImGui::InputText(m_Name.c_str(), m_Text, 1024, m_Flag);
			}
			break;
		case EImGuiInputType::Int:
			Input = ImGui::InputInt(m_Name.c_str(), &m_ValueInt, 1, 10, m_Flag);
			break;
		case EImGuiInputType::Float:
			Input = ImGui::InputFloat(m_Name.c_str(), &m_ValueFloat, 0.f, 0.f, "%.5f", m_Flag);
			break;
		}
	}

	// 입력된게 있는지를 판단한다.
	if (Input)
	{
		// 문자를 입력받았을 경우 widechar와 utf8을 만들어준다.
		if (m_InputType == EImGuiInputType::String)
		{
			memset(m_wText, 0, 2048);
			memset(m_TextUTF8, 0, 1024);

			int Length = (int)MultiByteToWideChar(CP_ACP, 0, m_Text, -1, nullptr, 0);

			MultiByteToWideChar(CP_ACP, 0, m_Text, -1, m_wText, Length);

			Length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, nullptr, 0, nullptr, nullptr);

			WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, m_TextUTF8, Length, nullptr, nullptr);
		}

		if (m_InputCallback)
		{
			m_InputCallback();
		}
	}


	ImGui::PopItemWidth();
}
