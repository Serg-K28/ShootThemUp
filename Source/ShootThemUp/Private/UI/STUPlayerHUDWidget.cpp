// Shoot Them Up Game. All Rights Reserved

#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUWeaponComponent.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"
#include "Components/ProgressBar.h"
#include "Player/STUPlayerState.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(
        GetOwningPlayerPawn()); //Приведення типу Component-а UActorComponent до USTUHealthComponent
    if (!HealthComponent)
    {
        return 0.0f;
    }

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetWeaponCurrentUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent)
    {
        return false;
    }

    return WeaponComponent->GetWeaponCurrentUIData(UIData);
}

bool USTUPlayerHUDWidget::GetWeaponCurrentAmmoData(FAmmoData& AmmoData) const //
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(
        GetOwningPlayerPawn()); // GetOwningPlayerPawn() Повертає вказівник на контролюючого його павна/героя
    ;
    if (!WeaponComponent)
    {
        return false;
    }

    return WeaponComponent->GetWeaponCurrentAmmoData(AmmoData);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

void USTUPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer()) //перевірка чи не нульовий контролер
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f)
    {
       // OnTakeDamage();
        if (!IsAnimationPlaying(DamageAnimation))
        {
            PlayAnimation(DamageAnimation);
        }
    }
    UpdateHealthBar();
}

void USTUPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(NewPawn);
    if (HealthComponent && !HealthComponent->OnHealthChanged.IsBoundToObject(this))
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
    }
    UpdateHealthBar();
}

int32 USTUPlayerHUDWidget::GetKillsNum() const
{
    const auto Controller = GetOwningPlayer();
    if (!Controller)
    {
        return 0;
    }

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);

    return PlayerState ? PlayerState->GetKillsNum() : 0;
}

int32 USTUPlayerHUDWidget::GetDeathsNum() const
{
    const auto Controller = GetOwningPlayer();
    if (!Controller)
    {
        return 0;
    }

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);

    return PlayerState ? PlayerState->GetDeathsNum() : 0;
}

void USTUPlayerHUDWidget::UpdateHealthBar()
{
    if (HealthProgressBar)
    {
        HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
    }
}

FString USTUPlayerHUDWidget::FormatBullets(int32 BulletsNum) const
{
    const int32 MaxLen = 3;
    const TCHAR PrefixSymbol = '0';
    
    auto BulletsStr = FString::FromInt(BulletsNum);
    const auto SymbolNumToAdd = MaxLen - BulletsStr.Len();

    if (SymbolNumToAdd > 0)
    {
        BulletsStr = FString::ChrN(SymbolNumToAdd, PrefixSymbol).Append(BulletsStr);
    }
    return BulletsStr;
}