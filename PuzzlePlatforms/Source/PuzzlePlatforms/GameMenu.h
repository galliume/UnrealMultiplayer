#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.h"
#include "GameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UGameMenu : public UMenuWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* m_CloseBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* m_GoMainBtn;

	UFUNCTION()
	void Close();

	UFUNCTION()
	void GoMain();
};
