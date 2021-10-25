// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS() class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASTUBaseWeapon();

    FOnClipEmptySignature OnClipEmpty;

    virtual void StartFire();
    virtual void StopFire();

    void ChangeClip(); //�����������
    bool CanReload() const;

    bool TryToAddAmmo(int32 ClipsAmount);

    FWeaponUIData GetUIData() const { return UIData; }
    FAmmoData GetAmmoData() const { return CurrentAmmo; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f; // 1500 �����. 1 ���� 1 ��(15 �����)

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* MuzzleFX;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void MakeShot();

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd, FVector& ShootDirection) const;

    APlayerController* GetPlayerController() const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    bool IsAmmoFull() const;

    FVector GetMuzzleWorldLocation() const;

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    void ScaleDamage(float Amount, const FHitResult& HitResult);

    //�����������
    void DecreaseAmmo();      //������� ������� �������� �� 1 ��� ������
    bool IsAmmoEmpty() const; //������� trye ���� �� �������� �� ����� ����� ����
    bool IsClipEmpty() const; //������� true ���� ������� ������ �����
    void LogAmmo();           //��������� ��� ��������� ������� ��������
    //

    UNiagaraComponent* SpawnMazzleFX();

private:
    FAmmoData CurrentAmmo;
};
