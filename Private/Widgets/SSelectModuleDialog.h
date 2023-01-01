#pragma once
#include "AppFramework/Public/Widgets/Workflow/SWizard.h"

class SSelectModuleDialog : public SWindow
{
public:
	SLATE_BEGIN_ARGS(SSelectModuleDialog)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	bool IsOkButtonEnabled() const;
	void OnOkButtonClicked();
	void OnCancelButtonClicked();
	
	TArray<FAssetData> SelectedAssets;
	TSharedPtr<SWizard> Wizard;
	TSharedPtr<SBox> AssetPickerList;
};