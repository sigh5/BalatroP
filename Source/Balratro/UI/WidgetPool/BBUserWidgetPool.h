// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "BBUserWidgetPool.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UBBUserWidgetPool : public UObject
{
	GENERATED_BODY()
	
public:
   UBBUserWidgetBase* AddWidget(UUserWidget* Owner, FName& CurViewName, TSubclassOf<UBBUserWidgetBase> WidgetClass)
    {
        if (!WidgetClass)
        {
            return nullptr;
        }

        if (auto* UseWidget = Pool.Find(CurViewName))
        {
            UBBUserWidgetBase* Widget = *UseWidget;
            return Widget;
        }

        auto* Widget = CreateWidget<UBBUserWidgetBase>(Owner, WidgetClass);
        Pool.Add(CurViewName, Widget);
        return Widget;
    }

   UBBUserWidgetBase* GetWidget(FName& CurViewName)
   {
       if (auto* UseWidget = Pool.Find(CurViewName))
       {
           UBBUserWidgetBase* Widget = *UseWidget;
           return Widget;
       }
       
       return nullptr;
   }


    void SetWidgetVisible(const FName& CurViewName,bool Invalue)
    {
        if (auto* UseWidget = Pool.Find(CurViewName))
        {
            UBBUserWidgetBase* Widget = *UseWidget;
            if (Invalue == true)
            {
                Widget->SetVisibility(ESlateVisibility::Visible);
            }
            else
                Widget->SetVisibility(ESlateVisibility::Collapsed);
        };
    }

    void ClearPool()
    {
        Pool.Empty();
    }

    int32 NumInPool() const
    {
        return Pool.Num();
    }

private:
    UPROPERTY()
    TMap<FName, TObjectPtr<UBBUserWidgetBase>> Pool;

};
