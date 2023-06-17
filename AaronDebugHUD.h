#pragma once

#include "CoreMinimal.h"
#include "DebugHUD.h"

#include "AaronDebugHUD.generated.h"

UCLASS()
class AAaronDebugHUD : public ADebugHUD
{
	GENERATED_BODY()

public:
	// individual commands
	// see the end of AaronDebugHUD.cpp for the implementation
	static void ToggleBasicHUD(const TArray<FString>& Strings, UWorld* World);
	static void ToggleWorldBoxes(const TArray<FString>& Strings, UWorld* World);

private:
	// Draw loop
	void DrawDebugHUD(UCanvas* Cnv, APlayerController* PC);

	// Create the HUD and register the draw loop
	static void CreateHUD(UWorld* World);

	static bool bEnableBasicHUD;
	static bool bEnableWorldBoxes;

	static FDelegateHandle DrawDebugDelegateHandle;
};
