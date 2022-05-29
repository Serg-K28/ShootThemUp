// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All);

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
};

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFire()
{
    UE_LOG(LogRifleWeapon, Error, TEXT("Fire!"));
    InitFX();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
    SetFXActive(false);
}

void ASTURifleWeapon::MakeShot()
{

    if (!GetWorld())
    {
        return;
    }

    if (IsAmmoEmpty())
    {
        StopFire();
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
        return;
    }

    FVector TraceStart, TraceEnd, ShootDirection;
    if (!GetTraceData(TraceStart, TraceEnd, ShootDirection))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    FVector TraceFXEnd = TraceEnd;  //���� ����� �� ������, ��� �� �����������
    if (HitResult.bBlockingHit)
    {
        TraceFXEnd = HitResult.ImpactPoint;
        //
        const FVector AdjustedDir = (HitResult.ImpactPoint - GetMuzzleWorldLocation()).GetSafeNormal();  //����������� ���� ��
        const float DirectionDot = FMath::RadiansToDegrees(FVector::DotProduct(AdjustedDir, ShootDirection));  //����� � ��������
        //
        if (DirectionDot > 0)  //���� ��� �������(�������) �� �� ��������� ������
        {
            //
            // DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
            // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
            WeaponFXComponent->PlayImpactFX(HitResult);
            UE_LOG(LogRifleWeapon, Error, TEXT("Bone: %s"), *HitResult.BoneName.ToString());

            MakeDamage(HitResult);
        }  //���� ������ ��������� ����� ������� �� ���� ��� � ���糺� � ������� �� ����... �� ������� ���� ��(������ ��� � ����������
           //������� �������)
    }
    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
    DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd, FVector& ShootDirection) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    TraceStart = ViewLocation;  // SocketTransform.GetLocation(); //��������� ����� �������
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    // SocketTransform.GetRotation().GetForwardVector(); //������� ��� FQuat, ����� ��� ������� ��� �� ������� ������ �������� �� ��
    // �
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
    ///////////��������� ����������� ������

    if (HitResult.BoneName == "b_head")
    {
        ScaleDamage(DamageAmount + 90.0f, HitResult);
    }
    else
    {
        ScaleDamage(DamageAmount, HitResult);
    }
    ///////////
}

void ASTURifleWeapon::InitFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMazzleFX();
    }

    if (!FireAudioComponent)
    {
        FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    }

    SetFXActive(true);
}

void ASTURifleWeapon::SetFXActive(bool IsActive)
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetPaused(!IsActive);
        MuzzleFXComponent->SetVisibility(IsActive, true);
    }

    if (FireAudioComponent)
    {
        FireAudioComponent->SetPaused(!IsActive);
    }
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}

void ASTURifleWeapon::Zoom(bool Enable)
{
    const auto Controller = Cast<APlayerController>(GetController());
    if (!Controller || !Controller->PlayerCameraManager)
    {
        return;
    }

    if (Enable)
    {
        DefaultCameraFOV = Controller->PlayerCameraManager->GetFOVAngle();
    }

    Controller->PlayerCameraManager->SetFOV(Enable ? FOVZoomAngle : DefaultCameraFOV);
}