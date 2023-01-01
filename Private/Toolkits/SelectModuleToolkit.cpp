#include "SelectModuleToolkit.h"

#include "Framework/Application/SlateApplication.h"
#include "MainFrame/Public/Interfaces/IMainFrameModule.h"

#include "Widgets/SModuleScriptAssetPickerList.h"
#include "Widgets/SSelectModuleDialog.h"

#define LOCTEXT_NAMESPACE "FSelectModuleToolkit"

// Initialize
void FSelectModuleToolkit::Initialize(UNiagaraSystem* InNiagaraAsset)
{
	IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	TSharedPtr<SWindow> ParentWindow = MainFrame.GetParentWindow();

	TSharedRef<SSelectModuleDialog> SelectModuleDialog = SNew(SSelectModuleDialog);
	FSlateApplication::Get().AddModalWindow(SelectModuleDialog, ParentWindow);
}

#undef LOCTEXT_NAMESPACE