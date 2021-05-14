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

protected:
	virtual bool Initialize();

private:
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
	class UEditableTextBox* m_IPAddressText;

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
};
