// Shoot Them Up Game. All Rights Reserved

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Dev/STUFireDamageType.h"
#include "Dev/STUIceDamageType.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"

DEFINE_LOG_CATEGORY_STATIC(STUHealthComponent, All, All);

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);

    SetHealth(MaxHealth);

    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandle);
    }
}

void USTUHealthComponent::OnTakeAnyDamageHandle(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead())
    {
        return;
    }

    SetHealth(Health - Damage); //Математична дія яка обмежена двома рамками 0 та MaxHealth нижче і вище цих
                                //рамок значення не виходить завдяки методу Clamp

    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandler); // Зупиняємо таймер при отриманні урона

    if (IsDead()) //Перевірка чи наш герой живий
    {
        OnDeath.Broadcast(); //Поширення стану смерті
    }
    else if (AutoHeal &&
             GetWorld()) //Перевірка чи вімкнуто флажок автовідновлення і чи є світ, якщо так - вмикається таймер відновлення здоров'я
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandler, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }

    PlayCameraShake();
}

void USTUHealthComponent::HealUpdate() //Відновлення здоровя при отриманні урона
{
    SetHealth(Health + HealModifier); //Додає здоров'я. Міn метод не дає вийти за межі максимального значення MaxHealth

    if (IsHealthFull() && GetWorld()) //Зупиняє таймер відновлення якщо наше здоров'я повне
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandler);
    }
}

void USTUHealthComponent::SetHealth(float NewHealth) //метод зміни та встановлення здоров'я
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;   //Damage/Healing
    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta); //Виклик делегата FOnHealthChanged (OnHealthChanged) який поширює стан здоров'я
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
    if (IsHealthFull())
    {
        return false;
    }
    SetHealth(Health + HealthAmount);
    return true;
}

bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::PlayCameraShake()
{
    if (IsDead())
    {
        return;
    }

    const auto Player = Cast<APawn>(GetOwner());
    if (!Player)
    {
        return;
    }

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager)
    {
        return;
    }

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}