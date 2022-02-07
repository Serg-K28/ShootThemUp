// Shoot Them Up Game. All Rights Reserved

#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Menu/UI/STULevelItemWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);

void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
    }

    InitLevelItems();
}

void USTUMenuWidget::OnStartGame()
{
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance)
    {
        return;
    }

    UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevel().LevelName);
}

void USTUMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USTUMenuWidget::InitLevelItems()
{
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance)
    {
        return;
    }

    checkf(STUGameInstance->GetLevelsData().Num() != 0, TEXT ("Levels data must not be empty!"));
    
    if (!LevelItemsBox)
    {
        return;
    }
    LevelItemsBox->ClearChildren(); //очистка тестовиг плиток з двіжка

    for (auto LevelData : STUGameInstance->GetLevelsData())
    {
        const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);   //Створюємо віджет
        if (!LevelItemWidget)
        {
            continue;
        }
        LevelItemWidget->SetLevelData(LevelData);   //записуємо інформацію в створений віджет
        LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);   //Підписуємось на делегат з STUCoreTypes

        LevelItemsBox->AddChild(LevelItemWidget);   //Додаємо дочірній елемент в HorizontalBox
        LevelItemWidgets.Add(LevelItemWidget);  //Додаємо вказівник до нашого масива вказівників
    }

    if (STUGameInstance->GetStartupLevel().LevelName.IsNone())
    {
        OnLevelSelected(STUGameInstance->GetLevelsData()[0]); //Якщо рівень ще не обрано і ми точно знаємо що вони створені - підсвічуємо за замовченням 1-й рівень
    }
    else
    {
        OnLevelSelected(STUGameInstance->GetStartupLevel());    //Підсвічую рівень з якого вийшли до меню
    }
}

void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)
{
    const auto STUGameInstance = GetSTUGameInstance();
    if (!STUGameInstance)
    {
        return;
    }
    
    STUGameInstance->SetStartupLevel(Data);

    for (auto LevelItemWidget : LevelItemWidgets)
    {
        if (LevelItemWidget)
        {
            const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;    //Прийматиме значеннтя true якщо ім'я рівня яке прийшло в колбекі дорівнює поточному елементу масива наших віджетів
            LevelItemWidget->SetSelected(IsSelected);
        }
    }
}


USTUGameInstance* USTUMenuWidget::GetSTUGameInstance() const
{
    if (!GetWorld())
    {
        return nullptr;
    }
    return GetWorld()->GetGameInstance<USTUGameInstance>();
}
