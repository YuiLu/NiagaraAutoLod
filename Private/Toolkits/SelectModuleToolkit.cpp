#include "SelectModuleToolkit.h"

#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IMainFrameModule.h"
#include "Widgets/SSelectModuleDialog.h"

#define LOCTEXT_NAMESPACE "FSelectModuleToolkit"

// Initialize
void FSelectModuleToolkit::Initialize(TArray<FAssetData>& InNiagaraAssets)
{
	// 获取UE4主编辑器窗口的SWindow作为ParentWindow
	IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	TSharedPtr<SWindow> ParentWindow = MainFrame.GetParentWindow();
	// 添加自定义模态窗口
	TSharedRef<SSelectModuleDialog> SelectModuleDialog = SNew(SSelectModuleDialog, InNiagaraAssets);
	FSlateApplication::Get().AddModalWindow(SelectModuleDialog, ParentWindow);
}

#undef LOCTEXT_NAMESPACE