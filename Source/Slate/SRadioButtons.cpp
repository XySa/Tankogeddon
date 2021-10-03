// Fill out your copyright notice in the Description page of Project Settings.


#include "SRadioButtons.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRadioButtons::Construct(const FArguments& InArgs)
{
	OnRadioButtonChanged = InArgs._OnRadioButtonChanged;
	CheckBoxStyle = &InArgs._RadioButtonsStyle->CheckBoxStyle;
	TextStyle = &InArgs._RadioButtonsStyle->TextStyle;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			CreateRadioButton(ERadioButtonId::Radio_1, TEXT("Option 1"))
		]
		+ SVerticalBox::Slot()
		[
			CreateRadioButton(ERadioButtonId::Radio_2, TEXT("Option 2"))
		]
		+ SVerticalBox::Slot()
		[
			CreateRadioButton(ERadioButtonId::Radio_3, TEXT("Option 3"))
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

ECheckBoxState SRadioButtons::IsChecked(ERadioButtonId ButtonId) const
{
	return CurrentRadioButton == ButtonId ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SRadioButtons::OnCheckBoxChanged(ECheckBoxState NewState, ERadioButtonId ButtonId)
{
	if (NewState == ECheckBoxState::Checked)
	{
		CurrentRadioButton = ButtonId;
		OnRadioButtonChanged.ExecuteIfBound(ButtonId);
	}
}

TSharedRef<SWidget> SRadioButtons::CreateRadioButton(ERadioButtonId ButtonId, const FString& Text) const
{
	return SNew(SCheckBox)
	.IsChecked(MakeAttributeRaw(this, &SRadioButtons::IsChecked, ButtonId))
	.OnCheckStateChanged(this, &SRadioButtons::OnCheckBoxChanged, ButtonId)
	.Style(CheckBoxStyle)
	[
		SNew(STextBlock)
		.Text(FText::FromString(Text))
		.TextStyle(TextStyle)
	];
}
