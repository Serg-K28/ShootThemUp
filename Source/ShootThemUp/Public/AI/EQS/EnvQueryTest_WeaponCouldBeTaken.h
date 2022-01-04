// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_WeaponCouldBeTaken.generated.h"

class ASTUBaseWeapon;

UCLASS()
class SHOOTTHEMUP_API UEnvQueryTest_WeaponCouldBeTaken : public UEnvQueryTest
{
    GENERATED_BODY()

public:
    UEnvQueryTest_WeaponCouldBeTaken(const FObjectInitializer& ObjectInitializer);

    virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TSubclassOf<ASTUBaseWeapon> WeaponType;
};
