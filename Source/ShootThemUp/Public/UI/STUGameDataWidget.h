// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGameDataWidget.generated.h"

class ASTUGameModeBase;
class ASTUPlayerState;

UCLASS()
class SHOOTTHEMUP_API USTUGameDataWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    //GameMode
    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetTimeRound() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetCurrentRound() const;

    //GameMode->GameData
    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetTotalRound() const;
private:
    ASTUGameModeBase* GameMode() const;
    ASTUPlayerState* PlayerState() const;
};
