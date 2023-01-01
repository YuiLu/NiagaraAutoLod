// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FNiagaraAutoLodModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedRef<FExtender> OnExtendCBAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets);
	void NiagaraAssetMenuExtension(FMenuBuilder& MenuBuilder, const TArray<FAssetData> SelectedAssets);
	void ExtensionButtonClicked(TArray<FAssetData> SelectedAssets);
	
	FDelegateHandle ContentBrowserAssetExtenderDelegateHandle;
};