/**
*
* Debugging HUD for Metal in Motion.
*
* Original author: Rob Baker.
* Current maintainer: Rob Baker.
*
***********************************************************************************/

#include "DebugHUD.h"

#include "AbilitySystemDebugHUD.h"
#include "UObject/ConstructorHelpers.h"

/**
* Construct the debugging HUD, mainly establishing a font to use for display.
***********************************************************************************/

ADebugHUD::ADebugHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Engine/EngineFonts/Roboto"));

	MainFont = Font.Object;
}

void ADebugHUD::DrawWithBackground(UFont* InFont, const FString& Text, const FColor& TextColor, EAlignHorizontal::Type HAlign, float& OffsetX, EAlignVertical::Type VAlign, float& OffsetY, float Alpha)
{
	float SizeX, SizeY;
	Canvas->StrLen(InFont, Text, SizeX, SizeY);

	const float PosX = (HAlign == EAlignHorizontal::Center) ? OffsetX + (Canvas->ClipX - SizeX) * 0.5f :
		(HAlign == EAlignHorizontal::Left) ? Canvas->OrgX + OffsetX :
		Canvas->ClipX - SizeX - OffsetX;

	const float PosY = (VAlign == EAlignVertical::Center) ? OffsetY + (Canvas->ClipY - SizeY) * 0.5f :
		(VAlign == EAlignVertical::Top) ? Canvas->OrgY + OffsetY :
		Canvas->ClipY - SizeY - OffsetY;

	const float BoxPadding = 5.0f;

	const float InX = PosX - BoxPadding;
	const float InY = PosY - BoxPadding;
	const float Z1 = 0.1f;
	FCanvasTileItem TileItem(FVector2D(InX, InY), FVector2D(SizeX + BoxPadding * 2.0f, SizeY + BoxPadding * 2.0f), FLinearColor(0.75f, 0.75f, 0.75f, Alpha));
	Canvas->DrawItem(TileItem);

	FLinearColor TextCol(TextColor);
	TextCol.A = Alpha;
	FCanvasTextItem TextItem(FVector2D(PosX, PosY), FText::FromString(Text), GEngine->GetSmallFont(), TextCol);
	Canvas->DrawItem(TextItem);

	OffsetY += 25;
}
