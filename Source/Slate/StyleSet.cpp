#include "StyleSet.h"
#include "Engine.h"
#include "SlateBasics.h"
#include "Slate/SlateGameResources.h"
#include "Styling/ISlateStyle.h"

TSharedPtr<ISlateStyle> FindStyle(const FString& ScopeToDirectory)
{
    TSharedPtr<FSlateGameResources> Style = FSlateGameResources::New(FName("StyleSet"), ScopeToDirectory, ScopeToDirectory);
    Style->Set("Red", FLinearColor::Red);
    Style->Set("DefaultMargin", FMargin(40.f, 20.f));

    FSlateImageBrush* brush = new FSlateImageBrush(Style->RootToContentDir("RedFrog", TEXT(".JPG")), FVector2D(256.f, 256.f));

    Style->Set("RedFrogBrush", brush);

    return Style;
}

TSharedPtr<ISlateStyle> FStyleSet::StylePtr = nullptr;

void FStyleSet::Initialize()
{
    Shutdown();

    StylePtr = FindStyle("/Game/UI/Styles");
    FSlateStyleRegistry::RegisterSlateStyle(*StylePtr);
}

void FStyleSet::Shutdown()
{
    if (StylePtr.IsValid())
    {
		FSlateStyleRegistry::UnRegisterSlateStyle(*StylePtr);
		ensure(StylePtr.IsUnique());
		StylePtr.Reset();
    }
}

const ISlateStyle& FStyleSet::Get()
{
    if (!StylePtr.IsValid())
    {
		Initialize();
    }    
    return *StylePtr;
}