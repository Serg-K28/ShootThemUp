// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    ASTURifleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TimeBetweenShots = 0.1f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float BulletSpread = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 10.0f;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd, FVector& ShootDirection) const override;
    virtual void BeginPlay() override;

private:
    FTimerHandle ShotTimerHandle;

    void MakeDamage(const FHitResult& HitResult);

    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;
    
    void InitMuzzleFX();
    void SetMuzzleFXVisibility(bool Visible);
};
