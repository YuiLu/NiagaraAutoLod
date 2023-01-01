#pragma once

static const FName TabId("Select Module");
static const FName TestTab("Test Tab");

class FSelectModuleToolkit
{
public:
	// Constructor &. Destructor
	FSelectModuleToolkit() {}
	~FSelectModuleToolkit() {}
	
	void Initialize(TArray<FAssetData>& InNiagaraAssets);
};