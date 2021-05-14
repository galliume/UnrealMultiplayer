#include "MainMenu.h"

#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	if (!ensure(m_Host != nullptr)) return false;

	m_Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	return true;
}

void UMainMenu::HostServer()
{
	if (m_MenuInterface != nullptr) {
		m_MenuInterface->Host();
	}
}

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterface)
{
	m_MenuInterface = MenuInterface;
}