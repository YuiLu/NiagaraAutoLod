#include "SSelectModuleDialog.h"

#define LOCTEXT_NAMESPACE "FSelectModuleToolkit"

void SSelectModuleDialog::Construct(const FArguments& InArgs)
{
	TSharedRef<SVerticalBox> RootBox =
	SNew(SVerticalBox)
	// Creation mode toggle buttons.
	+ SVerticalBox::Slot()
	.Padding(0, 5, 0, 5)
	.AutoHeight()
	[
		SNew(SBox)
		.Padding(FEditorStyle::GetMargin("StandardDialog.SlotPadding"))
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			.Padding(FMargin(7))
			[
				SAssignNew(AssetPickerList, SBox)
			]
		]
	];
	
	SWindow::Construct(
		SWindow::FArguments()
		.Title(LOCTEXT("NewSelectModuleWindowTitle", "Pick a module script for your system(s)"))
		.SizingRule(ESizingRule::UserSized)
		.ClientSize(FVector2D(450,600))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		[
			SAssignNew(Wizard, SWizard)
			.OnCanceled(this, &SSelectModuleDialog::OnCancelButtonClicked)
			.OnFinished(this, &SSelectModuleDialog::OnOkButtonClicked)
			.CanFinish(this, &SSelectModuleDialog::IsOkButtonEnabled)
			.ShowPageList(false)
			+SWizard::Page()
			.CanShow(true)
			[
				RootBox
			]
		]
	);
}

bool SSelectModuleDialog::IsOkButtonEnabled() const
{
	return true;
}

void SSelectModuleDialog::OnOkButtonClicked()
{
	RequestDestroyWindow();
}

void SSelectModuleDialog::OnCancelButtonClicked()
{
	RequestDestroyWindow();
}

#undef LOCTEXT_NAMESPACE