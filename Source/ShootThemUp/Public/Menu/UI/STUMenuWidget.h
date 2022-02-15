// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class USTUGameInstance;
class USTULevelItemWidget;

UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public USTUBaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* StartGameButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* LevelItemsBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* HideAnimation;

    virtual void NativeOnInitialized() override;

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation);

private:
    UPROPERTY()
    TArray<USTULevelItemWidget*> LevelItemWidgets; //Масив посилань на наші створені віджети

    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnQuitGame();

    void InitLevelItems();                        //основна логіка створення плиток кнопок
    void OnLevelSelected(const FLevelData& Data); //Функція колбек для нашого створеного делегата
    USTUGameInstance* GetSTUGameInstance() const; //Допоміжна функція. Повертає вказівник на наш конкретний клас
};
