// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

class AAIController;

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASTUGameModeBase();

    FOnMatchStateChangedSignature OnMatchStateChanged;

    virtual void StartPlay() override;

    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

    //��������� ������� ��� - ����
    void Killed(AController* KillerController, AController* VictimController);

    //
    int32 GetTimeRound() const { return RoundCountDown; }
    int32 GetCurrentRound() const { return CurrentRound; }
    FGameData GetGameData() const { return GameData; }
    //

    void RespawnRequest(AController* Controller);

    virtual bool ClearPause() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

private:
    ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;

    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;

    void SpawnBots();
    
    void StartRound();  //������ ������ � ������������ ��������� ����
    void GameTimerUpdate(); // ���������� �������, ��� 0 ��������� ������. ���� �� �������� ����� - ���������� ���

    void ResetPlayers();    //������ ���� ���������� � ���������� ������� �� ��������� ResetOnePlayer
    void ResetOnePlayer(AController* Controller);   //���� Pawn �� ��������(�����) - Destrot � ��������� �� ����

    void CreateTeamsInfo();
    FLinearColor DetermineColorByTeamID(int32 TeamID) const;
    void SetPlayerColor(AController* Controller);

    //�������� ���������� �� ���� PlayerState
    void LogPlayerInfo();

    void StartRespawn(AController* Controller);

    void GameOver();

    void SetMatchState(ESTUMatchState State);

    void StopAllFire();
};
