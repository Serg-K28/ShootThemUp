// Shoot Them Up Game. All Rights Reserved


#include "UI/STUGameDataWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"
#include "Player/STUPlayerController.h"

int32 USTUGameDataWidget::GetTimeRound() const 
{
    return GameMode() ? GameMode()->GetTimeRound() : 0;
}

int32 USTUGameDataWidget::GetCurrentRound() const
{
    return GameMode() ? GameMode()->GetCurrentRound() : 0;
}

int32 USTUGameDataWidget::GetTotalRound() const
{
    return GameMode() ? GameMode()->GetGameData().RoundsNum : 0;
}

ASTUGameModeBase* USTUGameDataWidget::GameMode() const
{
    return GetWorld() ? Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ASTUPlayerState* USTUGameDataWidget::PlayerState() const
{
    return GetOwningPlayer() ? Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}
