#include "AaronDebugHUD.h"

#include "AbilitySystemDebugHUD.h"
#include "EngineUtils.h"
#include "Debug/DebugDrawService.h"
#include "Kismet/GameplayStatics.h"

bool AAaronDebugHUD::bEnableBasicHUD = false;
bool AAaronDebugHUD::bEnableWorldBoxes = false;

FDelegateHandle AAaronDebugHUD::DrawDebugDelegateHandle;

void AAaronDebugHUD::ToggleBasicHUD(const TArray<FString>& Strings, UWorld* World)
{
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
		FWorldDelegates::OnPostWorldInitialization.Remove(PostWorldInitDelegateHandle);
		UDebugDrawService::Unregister(DrawDebugDelegateHandle);
		HUD->Destroy();
	}
}

void AAaronDebugHUD::ToggleWorldBoxes(const TArray<FString>& Strings, UWorld* World)
{
	bEnableWorldBoxes = !bEnableWorldBoxes;
}

void AAaronDebugHUD::DrawDebugHUD(UCanvas* Cnv, APlayerController* PC)
{
	Canvas = Cnv;
	if (!Canvas)
	{
		return;
	}

	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}
	
	UFont* Font = GEngine->GetSmallFont();
	FColor Color(38, 128, 0);
	float OffsetX = 100.0f;
	float OffsetY = 200.0f;

	const FString String = FString::Printf(TEXT("Some Float: %.2f"), GetWorld()->GetTimeSeconds());

	// CopyPaste from AAbilitySystemDebugHud::DrawWithBackground
	DrawWithBackground(Font, String, Color, EAlignHorizontal::Left, OffsetX, EAlignVertical::Top, OffsetY);

	const APawn* PlayerPawn = PlayerController->GetPawn();
	const FVector PlayerPawnLocation = PlayerPawn->GetActorLocation();

	// Draw a box with 200.0f size at the player's location
	AddBox(PlayerPawnLocation, FLinearColor::Green, 200.0f);

	if (bEnableWorldBoxes)
	{
		// Draw a box around all actors with the DebugBox tag
		TArray<AActor*> ActorsToDraw;
		UGameplayStatics::GetAllActorsWithTag(PlayerPawn, TEXT("DebugBox"), ActorsToDraw);

		for (const AActor* DebugActor : ActorsToDraw)
		{
			if (IsValid(DebugActor))
			{
				AddBox(DebugActor->GetActorLocation(), FLinearColor::Blue, 50.0f);
			}
		}
	}

	// Draw a line to show the player's velocity 
	AddLine(PlayerPawnLocation, PlayerPawnLocation + PlayerPawn->GetVelocity() * 100.0f, FLinearColor::Red);

	{
		// Draw this bit on the right side of the screen
		constexpr float RightOffsetX = 1000.0f;
		AddBool(TEXT("Some Bool"), true, OffsetY, RightOffsetX);
		AddFloat(TEXT("Some Float Without Background"), 1000.0f, OffsetY, RightOffsetX);
	}
}

void AAaronDebugHUD::CreateHUD(UWorld* World)
{
	AAaronDebugHUD* HUD = World->SpawnActor<AAaronDebugHUD>();
	
	FDebugDrawDelegate DrawDebugDelegate = FDebugDrawDelegate::CreateUObject(HUD, &AAaronDebugHUD::DrawDebugHUD);
	
	DrawDebugDelegateHandle = UDebugDrawService::Register(TEXT("GameplayDebug"), DrawDebugDelegate);
}

FAutoConsoleCommandWithWorldAndArgs AaronSystemsDebugHUDCommand(
	TEXT("Aaron.DebugHud"),
	TEXT("Toggles Drawing a basic debug HUD for Aaron Systems"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(AAaronDebugHUD::ToggleBasicHUD)
);

FAutoConsoleCommandWithWorldAndArgs AaronSystemsToggleExtraScanCommand(
	TEXT("Aaron.ToggleWorldBoxes"),
	TEXT("Toggles Drawing a debug box around actors with the DebugBox tag"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(AAaronDebugHUD::ToggleWorldBoxes)
);
