#pragma once

class STUUtils
{
public:
    template <typename T> static T* GetSTUPlayerComponent(APawn* PlayerPawn)
    {
        if (!PlayerPawn)
        {
            return nullptr;
        }
        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());//Отримуємо вказівник на наш компонент який ми вкажемо в PlayerHUDWidget. Оскільки ми маємо лише С++ клас то
                                             //можемо скористатися методом StaticClass
        return Cast<T>(Component);
    }
};