/**
*
* Debugging HUD.
*
* Original author: Rob Baker.
* Current maintainer: ...
*
* Copyright Caged Element Inc, code provided for educational purposes only.
*
***********************************************************************************/

#pragma once

#include "AbilitySystemDebugHUD.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"
#include "DebugHUD.generated.h"

/**
* Debugging HUD, a base class for deriving implementations from.
***********************************************************************************/

UCLASS(Abstract, NotBlueprintable)
class ADebugHUD : public AHUD
{
	GENERATED_BODY()

protected:

#define LOCTEXT_NAMESPACE "DebugHUD"

	// Construct the debugging HUD, mainly establishing a font to use for display.
	ADebugHUD();
	
	void DrawWithBackground(UFont* InFont, const FString& Text, const FColor& TextColor, EAlignHorizontal::Type HAlign, float& OffsetX, EAlignVertical::Type VAlign, float& OffsetY, float Alpha = 1.0f);

	// Add a FText to the HUD for rendering.
	void AddText(const TCHAR* title, const FText& value, float& Y, const float X)
	{ RenderStatistic(title, value, Y, X); }

	// Add a float to the HUD for rendering.
	void AddFloat(const TCHAR* title, float value, float& Y, const float X)
	{ RenderStatistic(title, FText::AsNumber(value), Y, X); }

	// Add an int32 to the HUD for rendering.
	void AddInt(const TCHAR* title, int32 value, float& Y, const float X)
	{ RenderStatistic(title, FText::AsNumber(value), Y, X); }

	// Add a bool to the HUD for rendering.
	void AddBool(const TCHAR* title, bool value, float& Y, const float X)
	{ RenderStatistic(title, BoolToText(value), Y, X, (value == false) ? FLinearColor::Red : FLinearColor::Green); }

	// Add a vector to the HUD for rendering.
	void AddVector(const TCHAR* title, const FVector& value, float& Y, const float X)
	{ RenderStatistic(title, VectorToText(value), Y, X); }

	// Add a rotator to the HUD for rendering.
	void AddRotator(const TCHAR* title, const FRotator& value, float& Y, const float X)
	{ RenderStatistic(title, RotatorToText(value), Y, X); }

	// Add a float with a text title to the HUD for rendering at a particular location.
	void AddTextFloatAt(const TCHAR* text, float value, const FVector& at, float xoffset = 0.0f, float yoffset = 0.0f) const
	{
		FFormatNamedArguments arguments;

		arguments.Add(TEXT("X"), CStringToText(text));
		arguments.Add(TEXT("Y"), CStringToText(TEXT(": ")));
		arguments.Add(TEXT("Z"), FText::AsNumber(value));

		bool frontal = false;

		FCanvasTextItem item0(WorldToCanvas(Canvas, at, xoffset, yoffset, frontal), FText::Format(LOCTEXT("Text", "{X} {Y} {Z}"), arguments), MainFont, FLinearColor::White);

		if (frontal == true)
		{
			item0.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
			Canvas->DrawItem(item0);
		}
	}

	// Add an int with a text title to the HUD for rendering at a particular location.
	void AddTextIntAt(const TCHAR* text, int32 value, const FVector& at, float xoffset = 0.0f, float yoffset = 0.0f) const
	{
		FFormatNamedArguments arguments;

		arguments.Add(TEXT("X"), CStringToText(text));
		arguments.Add(TEXT("Y"), CStringToText(TEXT(": ")));
		arguments.Add(TEXT("Z"), FText::AsNumber(value));

		bool frontal = false;

		FCanvasTextItem item0(WorldToCanvas(Canvas, at, xoffset, yoffset, frontal), FText::Format(LOCTEXT("Text", "{X} {Y} {Z}"), arguments), MainFont, FLinearColor::White);

		if (frontal == true)
		{
			item0.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
			Canvas->DrawItem(item0);
		}
	}

	// Add a box to the HUD for rendering.
	void AddBox(const FVector& position, const FLinearColor& color, float size = 10.0f) const
	{
		bool frontal = false;

		FCanvasBoxItem item(WorldToCanvas(Canvas, position, size * 0.5f, size * 0.5f, frontal), FVector2D(size, size));

		if (frontal == true)
		{
			item.SetColor(color);
			item.LineThickness = 2.0f;
			Canvas->DrawItem(item);
		}
	}

	// Add a line to the HUD for rendering.
	void AddLine(const FVector& start, const FVector& end, const FLinearColor& color, float thickness = 2.0f) const
	{
		bool frontal0 = false, frontal1 = false;

		FCanvasLineItem item(WorldToCanvas(Canvas, start, 0, 0, frontal0), WorldToCanvas(Canvas, end, 0, 0, frontal1));

		if (frontal0 == true &&
			frontal1 == true)
		{
			item.SetColor(color);
			item.LineThickness = thickness;
			Canvas->DrawItem(item);
		}
	}

	// The horizontal offset to render the statistic values at.
	float HorizontalOffset = 150.0f;


	// The line height to separate each HUD entry.
	float LineHeight = 16.0f;

private:

	// Convert a TCHAR pointer to FText.
	FText CStringToText(const TCHAR* text) const
	{ return FText::FromString(text); }

	// Convert a bool to FText.
	FText BoolToText(bool value) const
	{ return CStringToText((value == true) ? TEXT("true") : TEXT("false")); }

	// Convert a vector to FText.
	FText VectorToText(const FVector& value) const
	{
		FFormatNamedArguments arguments;

		arguments.Add(TEXT("X"), FText::AsNumber(value.X));
		arguments.Add(TEXT("Y"), FText::AsNumber(value.Y));
		arguments.Add(TEXT("Z"), FText::AsNumber(value.Z));

		return FText::Format(LOCTEXT("", "{X} {Y} {Z}"), arguments);
	}

	// Convert a rotator to FText.
	FText RotatorToText(const FRotator& value) const
	{
		FFormatNamedArguments arguments;

		arguments.Add(TEXT("Yaw"), FText::AsNumber((int32)value.Yaw));
		arguments.Add(TEXT("Pitch"), FText::AsNumber((int32)value.Pitch));
		arguments.Add(TEXT("Roll"), FText::AsNumber((int32)value.Roll));

		return FText::Format(LOCTEXT("", "{Yaw} {Pitch} {Roll}"), arguments);
	}

	// Render a statistic onto the HUD canvas.
	void RenderStatistic(const TCHAR* title, const FText& value, float& Y, const float X, const FLinearColor& valueColor = FLinearColor::White)
	{
		FCanvasTextItem item0(FVector2D(X, Y), CStringToText(title), MainFont, FLinearColor::White);
		item0.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
		Canvas->DrawItem(item0);
		FCanvasTextItem item1(FVector2D(X + HorizontalOffset, Y), value, MainFont, valueColor);
		item1.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
		Canvas->DrawItem(item1);
		Y += LineHeight;
	}

	// Convert a world coordinate to a 2D canvas coordinate.
	FVector2D WorldToCanvas(UCanvas* canvas, FVector position, float xoffset, float yoffset, bool& frontal) const
	{
		FVector toScreen = canvas->Project(position);

		frontal = toScreen.Z > 0.0f;

		return FVector2D(toScreen.X - xoffset, toScreen.Y - yoffset);
	}

#undef LOCTEXT_NAMESPACE

	// Font used to render the debug information.
	UPROPERTY(Transient)
		UFont* MainFont = nullptr;
};
