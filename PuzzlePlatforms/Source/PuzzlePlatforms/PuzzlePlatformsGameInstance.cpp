#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h" 
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "PlatformTrigger.h"
#include "MainMenu.h"
#include "MenuWidget.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MainMenuClass.Class != nullptr)) return;
	m_MenuClass = MainMenuClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> GameMenuClass(TEXT("/Game/MenuSystem/WBP_GameMenu"));
	if (!ensure(GameMenuClass.Class != nullptr)) return;
	m_GameMenuClass = GameMenuClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init"));
	UE_LOG(LogTemp, Warning, TEXT("Found main menu class %s"), *m_MenuClass->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Found game menu class %s"), *m_GameMenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (m_Menu != nullptr)
	{
		m_Menu->Teardown();
	}

	UEngine* Engine = GetEngine();

	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();

	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	if (m_Menu != nullptr)
	{
		m_Menu->Teardown();
	}

	UEngine* Engine = GetEngine();

	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(m_MenuClass != nullptr)) return;
	m_Menu = CreateWidget<UMainMenu>(this, m_MenuClass);

	m_Menu->SetUp();
	m_Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadGameMenu()
{
	if (!ensure(m_GameMenuClass != nullptr)) return;
	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, m_GameMenuClass);

	Menu->SetUp();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}