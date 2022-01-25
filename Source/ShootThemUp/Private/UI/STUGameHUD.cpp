// Shoot Them Up Game. All Rights Reserved

#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"
#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();
    // DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();
    auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->AddToViewport(); //Також може прийняти параметр порядка відрісовки
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
        }
    }
}

void ASTUGameHUD::DrawCrossHair()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f); //Середина екрана. Canvas визначає цент поточного екрана
    const float HalfLineSize = 10.0f;                                          //Половина довжини лінії приціла
    const float LineThickness = 2.0f;                                          //Товщина лінії
    const FLinearColor LineColor = FLinearColor::Green; //Колір приціла

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor,
        LineThickness); //Відрісовка горизонтальної лінії
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor,
        LineThickness); //Відрісовка вертикальнох лінії
}

void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State)
{
    UE_LOG(LogSTUGameHUD, Display, TEXT("Matsh state changed: %s"), *UEnum::GetValueAsString(State));
}
