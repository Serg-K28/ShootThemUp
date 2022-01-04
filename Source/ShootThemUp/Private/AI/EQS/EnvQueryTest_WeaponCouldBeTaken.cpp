// Shoot Them Up Game. All Rights Reserved

#include "AI/EQS/EnvQueryTest_WeaponCouldBeTaken.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Pickups/STUAmmoPickup.h"
#include "Weapon/STUBaseWeapon.h"

UEnvQueryTest_WeaponCouldBeTaken::UEnvQueryTest_WeaponCouldBeTaken(const FObjectInitializer& ObjectInitializer) //
    : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_WeaponCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        const auto ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        const auto PickupActor = Cast<ASTUAmmoPickup>(ItemActor);

        const bool WantsBeTakable = BoolValue.GetValue();

        if (!PickupActor || PickupActor->GetWeaponType() != WeaponType)
        {
            continue;
        }

        const auto CouldBeTaken = PickupActor->CouldBeTaken(); 
        It.SetScore(TestPurpose, FilterType, CouldBeTaken, WantsBeTakable);
    }
}