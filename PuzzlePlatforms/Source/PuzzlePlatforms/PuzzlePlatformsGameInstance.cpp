#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h" 
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "PlatformTrigger.h"
#include "MainMenu.h"
#include "MenuWidget.h"

const static FName SESSION_NAME = TEXT("PuzzlePlatformsSession");

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
		m_SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
		m_SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionComplete);

		m_SessionSearch = MakeShareable(new FOnlineSessionSearch());

		if (m_SessionSearch.IsValid()) {

			m_SessionSearch->bIsLanQuery = true;
			//m_SessionSearch->QuerySettings will be usefull for steam, etc.

			UE_LOG(LogTemp, Warning, TEXT("searching session"));
			m_SessionInterface->FindSessions(0, m_SessionSearch.ToSharedRef());
		}
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionComplete(bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("finished session search"));

	if (Success && m_SessionSearch.IsValid()) {
		for (const FOnlineSessionSearchResult& SearchResult : m_SessionSearch->SearchResults) {
			UE_LOG(LogTemp, Warning, TEXT("Session found : %s"), *SearchResult.GetSessionIdStr());
		}
	}
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (m_SessionInterface.IsValid()) {

		FNamedOnlineSession* ExistingSession = m_SessionInterface->GetNamedSession(SESSION_NAME);

		if (ExistingSession != nullptr) {
			m_SessionInterface->DestroySession(SESSION_NAME);
		} else {
			CreateSession();
		}
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

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success) {
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (m_SessionInterface.IsValid()) {
		
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true; //make it visible to find session

		m_SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
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