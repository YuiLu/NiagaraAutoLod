#include "NiagaraAutoModules.h"

#include "Styling/AppStyle.h"

#include "NiagaraSystem.h"
#include "NiagaraScript.h"

#include "NiagaraEditor\Private\Toolkits\NiagaraSystemToolkit.h"
#include "NiagaraEditor\Public\ViewModels\NiagaraEmitterHandleViewModel.h"	/* NiagaraEmitterHandleViewModel */
#include "NiagaraEditor\Public\ViewModels\Stack\NiagaraStackItemGroup.h"	/* UNiagaraStackItemGroup */
#include "NiagaraEditor\Public\NiagaraClipboard.h"							/* UNiagaraClipboardFunction */
#include "NiagaraEditor\Public\ViewModels\NiagaraSystemViewModel.h"			/* FNiagaraSystemViewModel */
#include "NiagaraEditor\Public\ViewModels\Stack\NiagaraStackViewModel.h"	/* UNiagaraStackViewModel */

#include "Toolkits/SelectModuleToolkit.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

TArray<TWeakObjectPtr<UNiagaraSystem>> NiagaraSystemAssets;
TSharedPtr<IToolkitHost> EditWithinLevelEditor;

void FNiagaraAutoModules::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);
	NiagaraSystemAssets = GetTypedWeakObjectPtrs<UNiagaraSystem>(InObjects);
	MenuBuilder.AddMenuEntry(
		LOCTEXT("NiagaraSystemAsset_AddModule", "AutoModule"),
		LOCTEXT("NiagaraSystemAsset_AddModuleToolTip", "Automatically add module for all emitters in this niagara system"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.LOD"),
		FUIAction(FExecuteAction::CreateRaw(this, &FNiagaraAutoModules::ExtensionButtonClicked))
	);
}

void FNiagaraAutoModules::ExtensionButtonClicked()
{
	for(TWeakObjectPtr<UNiagaraSystem> NiagaraSystemAsset : NiagaraSystemAssets)
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu Extension Execute!"));
		UNiagaraSystem* NiagaraAsset = Cast<UNiagaraSystem>(NiagaraSystemAsset);

		if(NiagaraAsset != nullptr)
		{
			TSharedRef<FSelectModuleToolkit> SelectModuleToolkit = MakeShareable(new FSelectModuleToolkit);
			SelectModuleToolkit->Initialize(NiagaraAsset);
			// TSharedPtr<FNiagaraSystemViewModel> const SystemViewModel = NiagaraEditorModule.GetExistingViewModelForSystem(NiagaraAsset);
			// AddMoudules(SystemViewModel); 
		}
	}
}

//TODO: 添加模块/动态输入 逻辑，参考 https://zhuanlan.zhihu.com/p/525642874
void FNiagaraAutoModules::AddMoudules(TSharedPtr<FNiagaraSystemViewModel> SystemViewModel)
{
	/* FNiagaraSystemViewModel			特效整体 (Niagara System)
	 * FNiagaraEmitterHandleViewModel	发射器 (Niagara Emitter)
	 * UNiagaraStackViewModel			模块堆栈 
	 * UNiagaraStackItemGroup			Group (Emitter Spawn, Particle Update .etc)
	 * UNiagaraStackModuleItem			Module			*/
	if (!SystemViewModel.IsValid())	{ return; }
	const TArray<TSharedRef<FNiagaraEmitterHandleViewModel>>& EmitterViewModels = SystemViewModel->GetEmitterHandleViewModels();
	// 每个发射器
	for (TSharedRef<FNiagaraEmitterHandleViewModel> EmitterHandleViewModel : EmitterViewModels)
	{
		UNiagaraStackViewModel* StackViewModel = EmitterHandleViewModel->GetEmitterStackViewModel();
		TArray<UNiagaraStackItemGroup*> StackItemGroups;
		StackViewModel->GetRootEntry()->GetUnfilteredChildrenOfType<UNiagaraStackItemGroup>(StackItemGroups);
		// 每个 StackItemGroup
		for (UNiagaraStackItemGroup* StackItemGroup : StackItemGroups)
		{
			// 筛选出 ParticleSpawn Stack
			FName const ExecutionCategoryName = StackItemGroup->GetExecutionCategoryName();
			if (ExecutionCategoryName != UNiagaraStackEntry::FExecutionCategoryNames::Particle)	{continue;}

			FName const ExecutionSubcategoryName = StackItemGroup->GetExecutionSubcategoryName();
			if (ExecutionSubcategoryName != UNiagaraStackEntry::FExecutionSubcategoryNames::Spawn)	{continue;}

			// 自动添加一个预设 Module
			UNiagaraClipboardContent* const ClipboardContent = UNiagaraClipboardContent::Create();
			UNiagaraScript* const Script = LoadObject<UNiagaraScript>(nullptr, TEXT(""), nullptr, LOAD_None, nullptr);
			const UNiagaraClipboardFunction* const ClipboardFunction = UNiagaraClipboardFunction::CreateScriptFunction(ClipboardContent, "", Script, FGuid());
			ClipboardContent->Functions.Add(ClipboardFunction);
			// Commit the clipboard content to the target stack entry
			FText PasteWarning = FText();
			StackItemGroup->Paste(ClipboardContent, PasteWarning);
		}
	}
}

#undef LOCTEXT_NAMESPACE