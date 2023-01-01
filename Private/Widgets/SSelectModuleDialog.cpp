#include "SSelectModuleDialog.h"

#include "NiagaraSystem.h"
#include "NiagaraEditor/Public/NiagaraEditorModule.h"
#include "NiagaraEditor/Public/ViewModels/NiagaraSystemViewModel.h"
#include "NiagaraEditor/Public/ViewModels/NiagaraEmitterHandleViewModel.h"
#include "NiagaraEditor/Public/ViewModels/Stack/NiagaraStackItemGroup.h"
#include "NiagaraEditor/Public/ViewModels/Stack/NiagaraStackEntry.h"
#include "NiagaraEditor/Public/ViewModels/Stack/NiagaraStackViewModel.h"
#include "NiagaraEditor/Public/NiagaraClipboard.h"

#include "Widgets/Workflow/SWizard.h"
#include "Widgets/Input/STextComboBox.h"

#include "SNiagaraModuleScriptList.h"
#include "EditorScriptingUtilities/Public/EditorAssetLibrary.h"
#include "Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "FSelectModuleToolkit"

SSelectModuleDialog::SSelectModuleDialog()
{
	SelectedModuleAssetData = nullptr;
	SelectedCategory = nullptr;
	SelectedSubCategory = nullptr;
}

SSelectModuleDialog::~SSelectModuleDialog()
{
	SelectedModuleAssetData = nullptr;
}

void SSelectModuleDialog::Construct(const FArguments& InArgs, TArray<FAssetData> InSelectedAssets)
{
	SelectedAssets = InSelectedAssets;

	GetValidCategory();

	TSharedRef<SVerticalBox> RootBox =
	SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	.Padding(10)
	.AutoHeight()
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		  .HAlign(HAlign_Left)
		  .AutoHeight()
		[
			SNew(STextBlock).Text(LOCTEXT("Select", "Target Location: "))
		]

#pragma region	类型选择控件
		+ SVerticalBox::Slot()
		  .HAlign(HAlign_Center)
		  .AutoHeight()
		  .Padding(0, 5, 0, 0)
		[
			SNew(SHorizontalBox)

#pragma region 选择一级生命周期: System/Emitter/Particle
			+ SHorizontalBox::Slot()
			  .VAlign(VAlign_Center)
			  .AutoWidth()
			  .Padding(0, 0, 20, 0)
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				  .VAlign(VAlign_Center)
				  .AutoWidth()
				[
					SNew(STextBlock).Text(LOCTEXT("Category", "Category: "))
				]

				+ SHorizontalBox::Slot()
				  .VAlign(VAlign_Center)
				[
					SAssignNew(CategoryComboBox, STextComboBox)
						.OptionsSource(&CategoryNames) // 设置Stack分类的可选项
						.OnSelectionChanged(this, &SSelectModuleDialog::OnCategorySelectionChanged) // 选中后的回调接口
						.InitiallySelectedItem(CategoryNames[0]) // 设置显示的初始选项
				]
			]
#pragma endregion

#pragma region 选择二级生命周期: Spawn/Update/...
			+ SHorizontalBox::Slot()
			  .VAlign(VAlign_Center)
			  .AutoWidth()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				  .VAlign(VAlign_Center)
				  .AutoWidth()
				[
					SNew(STextBlock).Text(LOCTEXT("SubCategory", "SubCategory: "))
				]

				+ SHorizontalBox::Slot().VAlign(VAlign_Center)
				[
					SAssignNew(SubCategoryComboBox, STextComboBox)
						.OptionsSource(&SubCategorySource) // 设置Stack子分类的可选项
						.OnSelectionChanged(this, &SSelectModuleDialog::OnSubCategorySelectionChanged) // 选中后的回调接口
						.InitiallySelectedItem(SubCategorySource[0]) // 设置显示的初始选项
				]
			]
#pragma endregion
		]
#pragma endregion
		+ SVerticalBox::Slot()
		  .AutoHeight()
		[
			// 列表控件，用于显示自定义的NiagaraScript
			SAssignNew(NiagaraModuleScriptList, SNiagaraModuleScriptList)
				.OnCollectAllModules(this, &SSelectModuleDialog::CollectAllModules)
				.OnModuleSeleted(this, &SSelectModuleDialog::OnModuleSeleted)
				.AutoExpandActionMenu(false)
		]
	];
	

	SWindow::Construct(
		SWindow::FArguments()
		.Title(LOCTEXT("NewSelectModuleWindowTitle", "Pick a module script for your system(s)"))
		.SizingRule(ESizingRule::UserSized)
		.ClientSize(FVector2D(450,640))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		[
			SAssignNew(Wizard, SWizard)
			.OnCanceled(this, &SSelectModuleDialog::OnCancelButtonClicked)
			.OnFinished(this, &SSelectModuleDialog::OnOkButtonClicked)
			.CanFinish(this, &SSelectModuleDialog::IsOkButtonEnabled)
			.ShowPageList(false)
			
			+ SWizard::Page()
			.CanShow(true)
			[
				RootBox
			]
		]
	);
}

bool SSelectModuleDialog::IsOkButtonEnabled() const
{
	return SelectedCategory.IsValid() && SelectedSubCategory.IsValid() && SelectedCategory != NoneCategory && SelectedSubCategory != NoneCategory && SelectedModuleAssetData != nullptr;
}

/* 核心逻辑: 
 * 遍历选中资产的所有Emitter，判断Emitter中是否含有选中的Stack分类，
 * if(有) {
 *   判断 NiagaraScript 能否加入选中的 Stack
 *   if(可以) {
 *     记录所有加入了该 NiagaraScript 的 Emitters
 *     在 MessageDialog 中打印输出 Success 信息
 *   }
 *   else {
 *     在 MessageDialog 中打印输出 Fail 信息
 *   }
 * }
 */
void SSelectModuleDialog::OnOkButtonClicked()
{
	/* NiagaraSystemViewModel层级结构: 
	 * FNiagaraSystemViewModel			资产视图模型，可以理解为特效整体 (Niagara System)
	 * FNiagaraEmitterHandleViewModel	发射器句柄
	 * UNiagaraStackViewModel			模块堆栈，可以理解为真正的发射器
	 * UNiagaraStackItemGroup			Module Group (Emitter Spawn, Particle Update .etc)
	 * UNiagaraStackModuleItem			Module
	 */
	bool bAutoLoadModule = false;
	FString SuccessMessages;
	FString FailedMessages;
	for (auto SelectedAsset : SelectedAssets)
	{
		FString SystemName = SelectedAsset.AssetName.ToString() + "\n";
		SuccessMessages.Append(SystemName);
		FailedMessages.Append(SystemName);
		if (UNiagaraSystem* NS = Cast<UNiagaraSystem>(SelectedAsset.GetAsset()))
		{
			TSharedPtr<FNiagaraSystemViewModel> SystemViewModel = MakeShared<FNiagaraSystemViewModel>();
			// 使用 Option 初始化 ViewModel, 否则会崩溃报错
			FNiagaraSystemViewModelOptions SystemViewModelOptions = FNiagaraSystemViewModelOptions();
			SystemViewModelOptions.bCanAutoCompile = false;
			SystemViewModelOptions.bCanSimulate = false;
			SystemViewModelOptions.EditMode = ENiagaraSystemViewModelEditMode::SystemAsset;
			SystemViewModelOptions.MessageLogGuid = NS->GetAssetGuid();
			SystemViewModel->Initialize(*NS, SystemViewModelOptions);

			// 获取发射器句柄
			const TArray<TSharedRef<FNiagaraEmitterHandleViewModel>>& EmitterHandleViewModels = SystemViewModel->GetEmitterHandleViewModels();
			// 遍历发射器
			for (TSharedRef<FNiagaraEmitterHandleViewModel> EmitterHandleViewModel : EmitterHandleViewModels)
			{
				UNiagaraStackViewModel* StackViewModel = EmitterHandleViewModel->GetEmitterStackViewModel();
				TArray<UNiagaraStackItemGroup*> StackItemGroups;
				StackViewModel->GetRootEntry()->GetUnfilteredChildrenOfType<UNiagaraStackItemGroup>(StackItemGroups);

				for (UNiagaraStackItemGroup* StackItemGroup : StackItemGroups)
				{
					FName const ExecutionCategoryName = StackItemGroup->GetExecutionCategoryName();
					FName const ExecutionSubcategoryName = StackItemGroup->GetExecutionSubcategoryName();

					// 对比Category
					if (ExecutionCategoryName.ToString() == *SelectedCategory && ExecutionSubcategoryName.ToString() == *SelectedSubCategory)
					{
						if (UNiagaraScript* Script = Cast<UNiagaraScript>(SelectedModuleAssetData->GetAsset()))
						{
							UNiagaraClipboardContent* const ClipboardContent = UNiagaraClipboardContent::Create();
							const UNiagaraClipboardFunction* const ClipboardFunction = UNiagaraClipboardFunction::CreateScriptFunction(ClipboardContent, SelectedModuleAssetData->AssetName.ToString(), Script, FGuid());
							ClipboardContent->Functions.Add(ClipboardFunction);

							// 检测 NiagaraScript 能否加入选中的 Stack
							FText OutTestMessage;
							if (StackItemGroup->TestCanPasteWithMessage(ClipboardContent, OutTestMessage))
							{
								bAutoLoadModule = true;
								NS->Modify();
								FText PasetWarning = FText();
								StackItemGroup->Paste(ClipboardContent, PasetWarning);
								SuccessMessages = SuccessMessages + FString::Format(TEXT("Add Module 【{0}】 to 【{1}】 Successfully!\n"), { SelectedModuleAssetData->AssetName.ToString(), EmitterHandleViewModel->GetName().ToString()});
							}
							else
							{
								FailedMessages = FailedMessages + FString::Format(TEXT("Add Module 【{0}】 to 【{1}】 Failed.\n"), { SelectedModuleAssetData->AssetName.ToString(), EmitterHandleViewModel->GetName().ToString()});
							}
						}
					}
				}
			}
			UEditorAssetLibrary::SaveLoadedAsset(NS);
		}
	}
	RequestDestroyWindow();

	// 输出信息
	if (bAutoLoadModule)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("AutoLoadModule_Success", "{0}"), FText::FromString(SuccessMessages)));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("AutoLoadModule_Fail", "{0}"), FText::FromString(FailedMessages)));
	}
}

void SSelectModuleDialog::OnCancelButtonClicked()
{
	RequestDestroyWindow();
}

void SSelectModuleDialog::GetValidCategory()
{
	// 根据选中的NiagaraSystem资产，遍历所有发射器，获取所有的Stack分类和对应的子分类。
	TArray<FString> TempCategoryNames;
	TMap<FString, TArray<FString>> TempSubCategoryNames;
	for (const FAssetData& SelectedAsset : SelectedAssets)
	{
		if (UNiagaraSystem* NS = Cast<UNiagaraSystem>(SelectedAsset.GetAsset()))
		{
			TSharedPtr<FNiagaraSystemViewModel> SystemViewModel = MakeShared<FNiagaraSystemViewModel>();
			FNiagaraSystemViewModelOptions SystemViewModelOptions = FNiagaraSystemViewModelOptions();
			SystemViewModelOptions.bCanAutoCompile = false;
			SystemViewModelOptions.bCanSimulate = false;
			SystemViewModelOptions.EditMode = ENiagaraSystemViewModelEditMode::SystemAsset;
			SystemViewModelOptions.MessageLogGuid = NS->GetAssetGuid();
			SystemViewModel->Initialize(*NS, SystemViewModelOptions);

			// 获取所有的发射器
			const TArray<TSharedRef<FNiagaraEmitterHandleViewModel>>& EmitterHandleViewModels = SystemViewModel->GetEmitterHandleViewModels();
			// 遍历发射器
			for (TSharedRef<FNiagaraEmitterHandleViewModel> EmitterHandleViewModel : EmitterHandleViewModels)
			{
				UNiagaraStackViewModel* StackViewModel = EmitterHandleViewModel->GetEmitterStackViewModel();
				TArray<UNiagaraStackItemGroup*> StackItemGroups;
				StackViewModel->GetRootEntry()->GetUnfilteredChildrenOfType<UNiagaraStackItemGroup>(StackItemGroups);

				for (UNiagaraStackItemGroup* StackItemGroup : StackItemGroups)
				{
					FName const ExecutionCategoryName = StackItemGroup->GetExecutionCategoryName();
					FName const ExecutionSubcategoryName = StackItemGroup->GetExecutionSubcategoryName();
					if (ExecutionSubcategoryName == UNiagaraStackEntry::FExecutionSubcategoryNames::Settings)
					{
						continue;
					}
					
					FString CategoryName = ExecutionCategoryName.ToString();
					TempCategoryNames.AddUnique(CategoryName);
					if (!TempSubCategoryNames.Contains(CategoryName))
					{
						TempSubCategoryNames.Add(CategoryName, {});
					}
					TempSubCategoryNames[CategoryName].AddUnique(ExecutionSubcategoryName.ToString());
				}
			}
		}
	}

	// 构建界面STextComboBox下拉菜单控件的选项。
	CategoryNames.Reset();
	SubCategoryNames.Reset();

	CategoryNames.Add(NoneCategory);
	SubCategoryNames.Add(NoneCategory, { NoneCategory });
	SubCategorySource.Add(NoneCategory);

	for (FString& CategoryName : TempCategoryNames)
	{
		TSharedPtr<FString> CategoryNamePtr = MakeShared<FString>(CategoryName);
		CategoryNames.Add(CategoryNamePtr);
		SubCategoryNames.Add(CategoryNamePtr, {});
		for (FString& SubCategoryName : TempSubCategoryNames[CategoryName])
		{
			SubCategoryNames[CategoryNamePtr].Add(MakeShared<FString>(SubCategoryName));
		}
	}
}

void SSelectModuleDialog::OnCategorySelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	// 当Category选择后，更新子分类的可选项。
	SubCategorySource.Empty();

	for (auto SubCategory : SubCategoryNames[NewSelection])
	{
		SubCategorySource.Add(SubCategory);
	}

	SelectedCategory = NewSelection;
	SelectedSubCategory = SubCategorySource[0];
	SubCategoryComboBox->RefreshOptions();
	SubCategoryComboBox->SetSelectedItem(SubCategorySource[0]);
}

void SSelectModuleDialog::OnSubCategorySelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	SelectedSubCategory = NewSelection;
}

void SSelectModuleDialog::CollectAllModules(TArray<FAssetData>& InOutModules)
{
	// 筛选并获取Content目录下的NiagaraScript
	InOutModules.Empty();

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	TArray<FAssetData> AssetDataList;
	AssetRegistryModule.Get().GetAssetsByClass(UNiagaraScript::StaticClass()->GetFName(), AssetDataList);
	for (const FAssetData& AssetData : AssetDataList)
	{
		if (AssetData.ObjectPath.ToString().StartsWith("/Game/"))
		{
			InOutModules.Add(AssetData);
		}
	}
}

void SSelectModuleDialog::OnModuleSeleted(FAssetData* InSelectedAssetData)
{
	SelectedModuleAssetData = InSelectedAssetData;
}

#undef LOCTEXT_NAMESPACE