#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init();

	UFUNCTION(Exec)
	void Host() override;

	UFUNCTION(Exec)
	void Join(const FString& Address) override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadGameMenu();

	virtual void LoadMainMenu() override;

private:
	TSubclassOf<class UUserWidget> m_MenuClass;
	TSubclassOf<class UUserWidget> m_GameMenuClass;

	class UMainMenu* m_Menu;
	class UGameMenu* m_GameMenu;

	IOnlineSessionPtr m_SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> m_SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void CreateSession();
	void OnFindSessionComplete(bool Success);

};
