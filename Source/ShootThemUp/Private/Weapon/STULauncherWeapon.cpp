// Shoot Them Up Game. All Rights Reserved

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"

void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty())
    {
        return;
    }
    FVector TraceStart, TraceEnd, ShootDirection;
    if (!GetTraceData(TraceStart, TraceEnd, ShootDirection))
    {
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);    //Даний метод власний, слад створювати
        Projectile->SetOwner(GetOwner());   //вказуєо власникок снаряда нашого героя так як власником зброї є ссилка на героя
        Projectile->FinishSpawning(SpawnTransform);
    }
    DecreaseAmmo();
    SpawnMazzleFX();
}