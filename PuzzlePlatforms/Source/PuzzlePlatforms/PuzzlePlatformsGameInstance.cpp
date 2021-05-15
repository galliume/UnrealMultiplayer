#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h" 
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

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
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (!ensure(OnlineSubsystem != nullptr)) {
		UE_LOG(LogTemp, Warning, TEXT("unable to load subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("subsystem %s loaded"), *OnlineSubsystem->GetSubsystemName().ToString());
	
	m_SessionInterface = OnlineSubsystem->GetSessionInterface();
	
	if (m_SessionInterface.IsValid()) {
		m_SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
	}
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (m_SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSettings;
		m_SessionInterface->CreateSession(0, TEXT("PuzzlePlatformsSession"), SessionSettings);
	}
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{

	if (!Success) {
		UE_LOG(LogTemp, Warning, TEXT("Unable to create a session"));
		return;
	}

	if (m_Menu != nullptr) {
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
	if (m_Menu != nullptr) {
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