// Shoot Them Up Game. All Rights Reserved

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

// Sets default values for this component's properties
USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    auto ImpactData = DefaultImpactData;
    if (Hit.PhysMaterial.IsValid()) //перевірка чи є такий матеріал
    {
        const auto PhysMat = Hit.PhysMaterial.Get(); //Отримуємо вказівник на матеріал
        if (ImpactDataMap.Contains(PhysMat))         //Шукаємо даний ключ в списку
        {
            ImpactData = ImpactDataMap[PhysMat]; //Присвоюємо наш матеріал
        }
    }
    // Niagara spawn effect
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.NiagaraEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

    // Decal spawn effect
   auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation //
        (                                  //
            GetWorld(),                    // Вказівник на світ
            ImpactData.DecalData.Material, // Матеріал Decal
            ImpactData.DecalData.Size,     // Розмір
            Hit.ImpactPoint,               // Місце розміщення (Зіткнення з об'єктом)
            Hit.ImpactNormal.Rotation()    // Обертання
        );
   if (DecalComponent)
   {
       DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
   }
}
