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

    virtual void StartPlay() override;

    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

    //підрахунок убивств ким - кого
    void Killed(AController* KillerController, AController* VictimController);

    //
    int32 GetTimeRound() const { return RoundCountDown; }
    int32 GetCurrentRound() const { return CurrentRound; }
    FGameData GetGameData() const { return GameData; }
    //

    void RespawnRequest(AController* Controller);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

private:
    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;

    void SpawnBots();
    
    void StartRound();  //Запуск раунда з встановленим діапазоном часу
    void GameTimerUpdate(); // Зменьшення таймера, при 0 збільшення раундів. Якщо це останній раунд - завершення гри

    void ResetPlayers();    //Виклик всих контролерів і перезапуск кожного за допомогою ResetOnePlayer
    void ResetOnePlayer(AController* Controller);   //Якщо Pawn не нульовий(живий) - Destrot і створення по новій

    void CreateTeamsInfo();
    FLinearColor DetermineColorByTeamID(int32 TeamID) const;
    void SetPlayerColor(AController* Controller);

    //Виводить інформацію по всим PlayerState
    void LogPlayerInfo();

    void StartRespawn(AController* Controller);

    void GameOver();

};
