// Shoot Them Up Game. All Rights Reserved

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Sound/SoundCue.h"

// Sets default values for this component's properties
USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    auto ImpactData = DefaultImpactData;
    if (Hit.PhysMaterial.IsValid()) //????????? ?? ? ????? ????????
    {
        const auto PhysMat = Hit.PhysMaterial.Get(); //????????? ????????? ?? ????????
        if (ImpactDataMap.Contains(PhysMat))         //??????? ????? ???? ? ??????
        {
            ImpactData = ImpactDataMap[PhysMat]; //?????????? ??? ????????
        }
    }
    // Niagara spawn effect
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.NiagaraEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

    // Decal spawn effect
   auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation //
        (                                  //
            GetWorld(),                    // ????????? ?? ????
            ImpactData.DecalData.Material, // ???????? Decal
            ImpactData.DecalData.Size,     // ??????
            Hit.ImpactPoint,               // ̳??? ?????????? (ǳ??????? ? ??'?????)
            Hit.ImpactNormal.Rotation()    // ?????????
        );
   if (DecalComponent)
   {
       DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
   }
   UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactData.Sound, Hit.ImpactPoint);
}
