#include "SModuleScriptAssetPickerList.h"

#include "NiagaraScript.h"

#include "AssetRegistryModule.h"

#define LOCTEXT_NAMESPACE "SModuleScriptAssetSelector"

void SModuleScriptAssetPickerList::Construct(const FArguments& InArgs, UClass* AssetClass)
{
	TArray<FAssetData> ModuleScriptsToShow = GetAssetDataForSelector(AssetClass);
	ChildSlot[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SAssignNew(ItemSelector, SModuleAssetItemSelector)
			.Items(ModuleScriptsToShow)
			.OnGetCategoriesForItem(this, &SModuleScriptAssetPickerList::OnGetCategoriesForItem)
			.OnCompareCategoriesForEquality(this, &SModuleScriptAssetPickerList::OnCompareCategoriesForEquality)
			.OnCompareCategoriesForSorting(this, &SModuleScriptAssetPickerList::OnCompareCategoriesForSorting)
			.OnCompareItemsForSorting(this, &SModuleScriptAssetPickerList::OnCompareItemsForSorting)
			.OnDoesItemMatchFilterText(this, &SModuleScriptAssetPickerList::OnDoesItemMatchFilterText)
			.OnGenerateWidgetForCategory(this, &SModuleScriptAssetPickerList::OnGenerateWidgetForCategory)
			.OnGenerateWidgetForItem(this, &SModuleScriptAssetPickerList::OnGenerateWidgetForItem)
			.OnItemActivated(this, &SModuleScriptAssetPickerList::OnItemActivated)
			.AllowMultiselect(false)
			.OnDoesItemPassCustomFilter(this, &SModuleScriptAssetPickerList::DoesItemPassCustomFilter)
			.RefreshItemSelectorDelegates(InArgs._RefreshItemSelectorDelegates)
			.ClickActivateMode(InArgs._ClickActivateMode)
			.ExpandInitially(true)
		]
	];
}

TArray<FAssetData> SModuleScriptAssetPickerList::GetAssetDataForSelector(UClass* AssetClass)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	TArray<FAssetData> ModuleScriptAssets;
	AssetRegistryModule.Get().GetAssetsByClass(AssetClass->GetFName(), ModuleScriptAssets);
	return ModuleScriptAssets;
}

TArray<FText> SModuleScriptAssetPickerList::OnGetCategoriesForItem(const FAssetData& Item)
{
	TArray<FText> Categories;
	FText ScriptAssetCategory;
	UNiagaraScript* ScriptAsset = Cast<UNiagaraScript>(Item.GetAsset());

	if(Item.GetClass() == UNiagaraScript::StaticClass())
	{
		
	}
}

#undef LOCTEXT_NAMESPACE