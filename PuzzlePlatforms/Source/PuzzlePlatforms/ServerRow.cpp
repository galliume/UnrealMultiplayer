#include "ServerRow.h"
#include "Components/Button.h"

#include "MainMenu.h"

void UServerRow::Setup(class UMainMenu* Parent, uint32 Index)
{
	m_Parent = Parent;
	m_Index = Index;
	m_ServerBtn->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
	m_Parent->SelectIndex(m_Index);
}