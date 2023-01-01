#pragma once
#include "Widgets/SItemSelector.h"

typedef SItemSelector<FText, FAssetData> SModuleAssetItemSelector;

class SModuleScriptAssetPickerList : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModuleScriptAssetPickerList)
	{}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, UClass* AssetClass);

private:
	TArray<FAssetData> GetAssetDataForSelector(UClass* AssetClass);
	TArray<FText> OnGetCategoriesForItem(const FAssetData& Item);
	// bool OnCompareCategoriesForEquality(const FText& CategoryA, const FText& CategoryB) const;
	// bool OnCompareCategoriesForSorting(const FText& CategoryA, const FText& CategoryB) const;
	// bool OnCompareItemsForSorting(const FAssetData& ItemA, const FAssetData& ItemB) const;
	// bool OnDoesItemMatchFilterText(const FText& FilterText, const FAssetData& Item);
	// TSharedRef<SWidget> OnGenerateWidgetForCategory(const FText& Category);
	// TSharedRef<SWidget> OnGenerateWidgetForItem(const FAssetData& Item);
	// void OnItemActivated(const FAssetData& Item);
	// bool DoesItemPassCustomFilter(const FAssetData& Item);
	
	TSharedPtr<SModuleAssetItemSelector> ItemSelector;
};