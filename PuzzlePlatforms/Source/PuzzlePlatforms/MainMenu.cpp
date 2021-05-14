#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(m_HostBtn != nullptr)) return false;
	m_HostBtn->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(m_JoinBtn != nullptr)) return false;
	m_JoinBtn->OnClicked.AddDynamic(this, &UMainMenu::JoinMenu);

	if (!ensure(m_CancelBtn != nullptr)) return false;
	m_CancelBtn->OnClicked.AddDynamic(this, &UMainMenu::CancelJoinMenu);

	if (!ensure(m_GOBtn != nullptr)) return false;
	m_GOBtn->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(m_QuitBtn != nullptr)) return false;
	m_QuitBtn->OnClicked.AddDynamic(this, &UMainMenu::Quit);

	return true;
}


void UMainMenu::HostServer()
{
	if (!ensure(m_MenuSwitcher != nullptr)) return;
	m_MenuInterface->Host();
}

void UMainMenu::JoinMenu()
{
	if (!ensure(m_MenuSwitcher != nullptr)) return;
	if (!ensure(m_JoinMenu != nullptr)) return;
	m_MenuSwitcher->SetActiveWidget(m_JoinMenu);
}

void UMainMenu::CancelJoinMenu()
{
	if (!ensure(m_MenuSwitcher != nullptr)) return;
	if (!ensure(m_MenuMain != nullptr)) return;
	m_MenuSwitcher->SetActiveWidget(m_MenuMain);
}

void UMainMenu::JoinServer()
{
	if (!ensure(m_MenuSwitcher != nullptr)) return;
	if (!ensure(m_IPAddressText != nullptr)) return;

	const FString& IPAdress = m_IPAddressText->GetText().ToString();

	m_MenuInterface->Join(IPAdress);
}

void UMainMenu::Quit()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}