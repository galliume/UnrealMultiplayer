// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(class UMainMenu* Parent, uint32 Index);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* m_ServerName;

	UPROPERTY(BlueprintReadOnly)
	bool m_Selected = false;

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* m_ServerBtn;

	UPROPERTY()
	class UMainMenu* m_Parent;

	UFUNCTION()
	void OnClicked();

	uint32 m_Index;
};
