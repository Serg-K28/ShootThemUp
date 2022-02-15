// Shoot Them Up Game. All Rights Reserved

#include "UI/STUHealthBarWidget.h"
#include "Components/ProgressBar.h"

void USTUHealthBarWidget::SetHealthPercent(float Percent)
{

    if (!HealthProgressBar)
    {
        return;
    }

    const auto HealthBarVisibility = (Percent > PercentVisibilityThreshold || FMath::IsNearlyZero(Percent)) //Визначення стану віджета
                                         ? ESlateVisibility::Hidden
                                         : ESlateVisibility::Visible;

    HealthProgressBar->SetVisibility(HealthBarVisibility);  //Встановлення видимості віджета

    const auto HealthBarColor = Percent > PercentColorThreshold ? GoodColor : BadColor; //Визначення кольора віджета
    HealthProgressBar->SetFillColorAndOpacity(HealthBarColor);  //Встановлення кольора віджета

    HealthProgressBar->SetPercent(Percent); //Передаємо здоров'я в прогрес бар
}