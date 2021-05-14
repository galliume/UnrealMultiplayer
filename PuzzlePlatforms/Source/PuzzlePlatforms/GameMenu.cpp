#include "GameMenu.h"

#include "Components/Button.h"

bool UGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(m_CloseBtn != nullptr)) return false;
	m_CloseBtn->OnClicked.AddDynamic(this, &UGameMenu::Close);

	if (!ensure(m_GoMainBtn != nullptr)) return false;
	m_GoMainBtn->OnClicked.AddDynamic(this, &UGameMenu::GoMain);

	return true;
}

void UGameMenu::Close()
{
	Teardown();
}

void UGameMenu::GoMain()
{
	if (m_MenuInterface != nullptr) {
		Teardown();
		m_MenuInterface->LoadMainMenu();
	}
}