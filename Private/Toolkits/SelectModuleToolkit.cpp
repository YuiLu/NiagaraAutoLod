#include "SelectModuleToolkit.h"

#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IMainFrameModule.h"
#include "Widgets/SSelectModuleDialog.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "FSelectModuleToolkit"

// Initialize
void FSelectModuleToolkit::Initialize(TArray<FAssetData>& InNiagaraAssets)
{
	const FVector2D DEFAULT_WINDOW_SIZE = FVector2D(450, 640);
	TSharedRef< SWindow > SelectModuleDialogWindow = SNew(SWindow)
		.Title(LOCTEXT("NewSelectModuleWindowTitle", "Pick a module script for your system(s)"))
		.ClientSize(DEFAULT_WINDOW_SIZE);
	TSharedRef< SSelectModuleDialog > SelectModuleDialog =
		SNew(SSelectModuleDialog, InNiagaraAssets)
		.ParentWindow(SelectModuleDialogWindow);
	SelectModuleDialogWindow->SetContent(SelectModuleDialog);
	GEditor->EditorAddModalWindow(SelectModuleDialogWindow);
}

#undef LOCTEXT_NAMESPACE