// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Native construct to associate our buttons with our OnClicked events.
void UInGameUI::NativeConstruct()
{
	if (PlayerVSPlayerButton)
	{
		PlayerVSPlayerButton->OnClicked.AddDynamic(this, &UInGameUI::PlayerVSPlayerOnClicked);
	}

	if (PlayerVSBotButton)
	{
		PlayerVSBotButton->OnClicked.AddDynamic(this, &UInGameUI::PlayerVSBotOnClicked);
	}


	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UInGameUI::QuitOnClicked);
	}
}

void UInGameUI::PlayerVSPlayerOnClicked()
{
}

void UInGameUI::PlayerVSBotOnClicked()
{
	RemoveFromParent();

	UWorld* TheWorld = GetWorld();

	FString CurrentLevel = TheWorld->GetMapName();

	UGameplayStatics::OpenLevel(this, "FirstPersonExampleMap");
}

void UInGameUI::SettingsGameOnClicked()
{
}

void UInGameUI::CreditsGameOnClicked()
{
}

// Quit game 
void UInGameUI::QuitOnClicked()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}


