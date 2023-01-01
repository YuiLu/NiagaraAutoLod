#pragma once

#include "Widgets/SWindow.h"

class SWizard;
class SNiagaraModuleScriptTree;

class SSelectModuleDialog : public SWindow
{
public:
	SSelectModuleDialog();
	~SSelectModuleDialog();

	SLATE_BEGIN_ARGS(SSelectModuleDialog)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TArray<FAssetData> InSelectedAssets);

private:
	bool IsOkButtonEnabled() const;
	void OnOkButtonClicked();
	void OnCancelButtonClicked();
	
	// 获取所有有效的分类和子分类名称
	void GetValidCategory();

	const TSharedPtr<FString> NoneCategory = MakeShareable(new FString(TEXT("None")));

	TArray<FAssetData> SelectedAssets;
	TSharedPtr<SWizard> Wizard;
	FAssetData* SelectedModuleAssetData;
	TSharedPtr<FString> SelectedCategory;
	TSharedPtr<FString> SelectedSubCategory;

	// 可以选择的分类控件和功能接口
	TArray<TSharedPtr<FString>> CategoryNames;
	TMap<TSharedPtr<FString>, TArray<TSharedPtr<FString>>> SubCategoryNames;
	TSharedPtr<STextComboBox> CategoryComboBox;
	TSharedPtr<STextComboBox> SubCategoryComboBox;
	TArray<TSharedPtr<FString>> SubCategorySource;
	void OnCategorySelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	void OnSubCategorySelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);

	// Modules列表控件
	TSharedPtr<SNiagaraModuleScriptTree> NiagaraModuleScriptTree;
	void CollectAllModules(TArray<FAssetData>& InOutModules);
	void OnModuleSeleted(FAssetData* InSelectedAssetData);
};