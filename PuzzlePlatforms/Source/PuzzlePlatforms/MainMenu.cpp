#include "MainMenu.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/CircularThrobber.h" 

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	m_ServerRowClass = ServerRowBPClass.Class;
}

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

	if (m_MenuInterface != nullptr) {
		m_MenuInterface->RefreshServerList();
	}

}

void UMainMenu::CancelJoinMenu()
{
	if (!ensure(m_MenuSwitcher != nullptr)) return;
	if (!ensure(m_MenuMain != nullptr)) return;
	m_MenuSwitcher->SetActiveWidget(m_MenuMain);
}

void UMainMenu::JoinServer()
{
	if (m_MenuInterface != nullptr && m_SelectedIndex.IsSet()) {
		m_MenuInterface->Join(m_SelectedIndex.GetValue());
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	m_SelectedIndex = Index;
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	m_ServerList->ClearChildren();

	uint32 i = 0;

	for (const FString& ServerName : ServerNames) {
		UServerRow* ServerRow = CreateWidget<UServerRow>(World, m_ServerRowClass);
		if (!ensure(ServerRow != nullptr)) return;

		ServerRow->m_ServerName->SetText(FText::FromString(ServerName));
		ServerRow->Setup(this, i);
		++i;

		m_ServerList->AddChild(ServerRow);
	}
}

void UMainMenu::Quit()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}

void UMainMenu::HideThrobber()
{
	if (!ensure(m_LoaderAnim != nullptr)) return;
	m_LoaderAnim->SetVisibility(ESlateVisibility::Hidden);
}