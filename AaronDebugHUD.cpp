#include "AaronDebugHUD.h"

#include "AbilitySystemDebugHUD.h"
#include "EngineUtils.h"
#include "Debug/DebugDrawService.h"

bool AAaronDebugHUD::bEnableBasicHUD = false;

FDelegateHandle AAaronDebugHUD::DrawDebugDelegateHandle;

void AAaronDebugHUD::DrawDebugHUD(UCanvas* Cnv, APlayerController* PC)
{
	if (!Cnv)
	{
		return;
	}

	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}
	
	Canvas = Cnv;
	
	UFont* Font = GEngine->GetSmallFont();
	FColor Color(38, 128, 0);
	float OffsetX = 100.0f;
	float OffsetY = 200.0f;

	FString String = FString::Printf(TEXT("%.2f"), GetWorld()->GetTimeSeconds());

	// CopyPaste from AAbilitySystemDebugHud::DrawWithBackground
	DrawWithBackground(Font, String, Color, EAlignHorizontal::Left, OffsetX, EAlignVertical::Top, OffsetY);

	const APawn* PlayerPawn = PlayerController->GetPawn();
	const FVector PlayerPawnLocation = PlayerPawn->GetActorLocation();

	// Draw a box with 200.0f size at the player's location
	AddBox(PlayerPawnLocation, FLinearColor::Green, 200.0f);
	
	// Draw a line to show the player's velocity 
	AddLine(PlayerPawnLocation, PlayerPawnLocation + PlayerPawn->GetVelocity() * 100.0f, FLinearColor::Red);

	{
		// Draw this bit on the right side of the screen
		constexpr float RightOffsetX = 1000.0f;
		AddBool(TEXT("Some Bool"), true, OffsetY, RightOffsetX);
		AddFloat(TEXT("Some Other Stat Which is not a bool"), 1000.0f, OffsetY, RightOffsetX);
	}
}

void AAaronDebugHUD::CreateHUD(UWorld* World)
{
	AAaronDebugHUD* HUD = World->SpawnActor<AAaronDebugHUD>();
	
	FDebugDrawDelegate DrawDebugDelegate = FDebugDrawDelegate::CreateUObject(HUD, &AAaronDebugHUD::DrawDebugHUD);
	
	DrawDebugDelegateHandle = UDebugDrawService::Register(TEXT("GameplayDebug"), DrawDebugDelegate);
}

void AAaronDebugHUD::ToggleBasicHUD(const TArray<FString>& Strings, UWorld* World)
{
	// this is not used ability system uses it for another layer of debugging see AbilitySystemDebugHUD.cpp
	bEnableBasicHUD = !bEnableBasicHUD;

	if (!World)
	{
		return;
	}

	static FDelegateHandle PostWorldInitDelegateHandle;

	AAaronDebugHUD* HUD = nullptr;
	for (TActorIterator<AAaronDebugHUD> It(World); It; ++It)
	{
		HUD = *It;
		break;
	}

	if (!HUD)
	{
		CreateHUD(World);

		PostWorldInitDelegateHandle = FWorldDelegates::OnPostWorldInitialization.AddLambda([](UWorld* World, const UWorld::InitializationValues IVS)
			{
				if (World->WorldType == EWorldType::PIE || World->WorldType == EWorldType::Game)
				{
					CreateHUD(World);
				}
			});

	}
	else if (HUD)
	{
		// Cleanup and destroy the HUD
		FWorldDelegates::OnPostWorldInitialization.Remove(PostWorldInitDelegateHandle);

		UDebugDrawService::Unregister(DrawDebugDelegateHandle);
		
		HUD->Destroy();
	}
}

FAutoConsoleCommandWithWorldAndArgs AbilitySystemToggleDebugHUDCommand(
	TEXT("Aaron.DebugHud"),
	TEXT("Toggles Drawing a basic debug HUD for Aaron Systems"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(AAaronDebugHUD::ToggleBasicHUD)
);
