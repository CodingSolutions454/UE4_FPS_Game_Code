// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	
    // Player vs player button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PlayerVSPlayerButton;

	// New Player vs bot button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PlayerVSBotButton;

	// Settings game button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SettingsGameButton;

	// Credits game button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* CreditsGameButton;

	// Quit game button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* QuitButton;

	virtual void NativeConstruct() override;

	// Player vs player Button OnClicked event
	UFUNCTION()
		void PlayerVSPlayerOnClicked();

	// Player vs bot Button OnClicked event
	UFUNCTION()
		void PlayerVSBotOnClicked();

	// Settings Game Button OnClicked event
	UFUNCTION()
		void SettingsGameOnClicked();

	// Credits Game Button OnClicked event
	UFUNCTION()
		void CreditsGameOnClicked();
	// Quit game event
	UFUNCTION()
		void QuitOnClicked();
};
