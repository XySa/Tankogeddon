// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SRadioButtons.h"
#include "RadioButtonsWidgetStyle.h"
#include "RadioButtons.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioButtonChangedEvent, ERadioButtonId, NewRadioButtonId);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTestM);
DECLARE_MULTICAST_DELEGATE(FTestS);

/**
 * 
 */
UCLASS()
class COURSEUI_API URadioButtons : public UWidget
{
	GENERATED_BODY()

public:

	virtual void ReleaseSlateResources(bool bRelealeseChildren) override;

	UPROPERTY(BlueprintAssignable)
	FOnRadioButtonChangedEvent OnRadioButtonChanged;

	UPROPERTY(EditAnywhere, category="Appearance")
	FRadioButtonsStyle WidgetStyle;

protected:

	void OnRadioButtonChangedFunc(ERadioButtonId ButtonId);

	virtual TSharedRef<SWidget> RebuildWidget() override;

	TSharedPtr<SRadioButtons> RadioButtonsHolder;
};
