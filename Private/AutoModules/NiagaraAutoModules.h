// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ToolMenus.h"

#include "NiagaraEditorModule.h"
#include "NiagaraEditor\Private\AssetTypeActions\AssetTypeActions_NiagaraSystem.h"
#include "Toolkits/SelectModuleToolkit.h"

class FNiagaraAutoModules : public FAssetTypeActions_NiagaraSystem
{
public:
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	virtual bool HasActions(const TArray<UObject*>& InObjects ) const override { return true; }
	
private:
	void ExtensionButtonClicked();
	void AddMoudules(TSharedPtr<FNiagaraSystemViewModel> SystemViewModel);
};
