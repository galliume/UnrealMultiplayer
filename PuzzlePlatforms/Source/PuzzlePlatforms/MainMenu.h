#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);
	void SetServerList(TArray<FString> ServerNames);
	void SelectIndex(uint32 Index);
	void HideThrobber();

protected:
	virtual bool Initialize();

private:
	TSubclassOf<class UUserWidget> m_ServerRowClass;

	UPROPERTY(meta = (BindWidget))
	class UButton* m_HostBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* m_JoinBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* m_CancelBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* m_GOBtn;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* m_MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* m_JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* m_MenuMain;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* m_QuitBtn;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* m_ServerList;

	UPROPERTY(meta = (BindWidget))
	class UCircularThrobber* m_LoaderAnim;

	UFUNCTION()
	void HostServer();
	
	UFUNCTION()
	void JoinMenu();

	UFUNCTION()
	void CancelJoinMenu();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void Quit();

	TOptional<uint32> m_SelectedIndex;

	void UpdateServerRows();
};
