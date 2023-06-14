#pragma once

#include "CoreMinimal.h"
#include "DebugHUD.h"

#include "AaronDebugHUD.generated.h"

UCLASS()
class AAaronDebugHUD : public ADebugHUD
{
	GENERATED_BODY()

public:
	static void ToggleBasicHUD(const TArray<FString>& Strings, UWorld* World);
	
	static bool bEnableBasicHUD;

	void DrawDebugHUD(UCanvas* Cnv, APlayerController* PC);

	static void CreateHUD(UWorld* World);
		
	static FDelegateHandle DrawDebugDelegateHandle;
};
