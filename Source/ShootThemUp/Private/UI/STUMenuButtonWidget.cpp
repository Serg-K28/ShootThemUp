// Shoot Them Up Game. All Rights Reserved

#include "UI/STUMenuButtonWidget.h"
#include "Components/Button.h"
#include "STUGameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuButtonWidget, All, All);

void USTUMenuButtonWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &USTUMenuButtonWidget::OnMainMenu);
    }
}

void USTUMenuButtonWidget::OnMainMenu() 
{
    if (!GetWorld())
    {
        return;
    }

    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if (!STUGameInstance)
    {
        return;
    }

    if (STUGameInstance->GetMainMenuLevelName().IsNone())
    {
        UE_LOG(LogSTUMenuButtonWidget, Error, TEXT("Menu Level name is NONE"));
        return;
    }

    UGameplayStatics::OpenLevel(this, STUGameInstance->GetMainMenuLevelName());
}
