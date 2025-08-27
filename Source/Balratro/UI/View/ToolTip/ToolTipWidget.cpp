// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/ToolTip/ToolTipWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WrapBoxSlot.h"

#include "Animation/WidgetAnimation.h"

void UToolTipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);

	ToolTipDataAsset = LoadObject<UHandRankingToolTip_DataAsset>
		(nullptr,
		TEXT("/Script/Balratro.HandRankingToolTip_DataAsset'/Game/Item/DT_TooltipRanking.DT_TooltipRanking'"));

	if (!ToolTipDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load DataAsset!"));
	}

	FillAnimMap();
}

void UToolTipWidget::SetHandRanking_ToolTipWidgetPos(const FHRButton_Info& _Info, const FVector2D& Pos)
{
	if (Pos.X == 0 && Pos.Y == 0)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Set_ToolTipWidgetImage(_Info);
		
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(this->Slot);
		if(CanvasSlot)
		{
			CanvasSlot->SetPosition(Pos);
		}
		
		SetVisibility(ESlateVisibility::Visible);
	}

}

void UToolTipWidget::SetImageRender(class UImage* CurImage, class UPaperSprite* Sprite)
{
	UWrapBoxSlot* HSlot = Cast<UWrapBoxSlot>(CurImage->Slot);
	if (HSlot)
	{
		FMargin Margin = HSlot->GetPadding();
		Margin.Right += 20.f; 
		HSlot->SetPadding(Margin);
	}

	FSlateBrush SpriteBrush;
	SpriteBrush.SetResourceObject(Sprite);
	SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
	SpriteBrush.SetImageSize(FVector2D(84.f, 128.f));
	CurImage->SetBrush(SpriteBrush);

	//PlayAnimation(HighCard, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);

}

void UToolTipWidget::Set_ToolTipWidgetImage(const FHRButton_Info& _Info)
{
	HaneRankingText->SetText(FText::FromName(_Info._Name));

	for (const auto & Info : ToolTipDataAsset->HandRankings)
	{
		if (Info.Name == _Info._Name)
		{
			for (int32 i = 0; i < Info.Cards.Num(); ++i)
			{
				if (UPaperSprite* Sprite = Info.Cards[i].LoadSynchronous())
				{
					// UImage에 반영 (예: Image1 ~ Image5)
					if (i == 0 && Image1)
						SetImageRender(Image1, Sprite);
					if (i == 1 && Image2)
						SetImageRender(Image2, Sprite);
					if (i == 2 && Image3)
						SetImageRender(Image3, Sprite);
					if (i == 3 && Image4)
						SetImageRender(Image4, Sprite);
					if (i == 4 && Image5)
						SetImageRender(Image5, Sprite);
				}
			}
			
			UWidgetAnimation* Anim = GetAnimationByName(Info.Animmation_Name);
			PlayAnimation(Anim);
		}
	}
}

UWidgetAnimation* UToolTipWidget::GetAnimationByName(const FName& AnimName) const
{
	UWidgetAnimation* const* WidgetAnim = AnimationsMap.Find(AnimName);
	if (WidgetAnim)
		return *WidgetAnim;

	return nullptr;
}

void UToolTipWidget::FillAnimMap()
{
	AnimationsMap.Empty();

	for (TFieldIterator<FObjectProperty> PropIt(GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		FObjectProperty* Prop = *PropIt;
		if (Prop->PropertyClass == UWidgetAnimation::StaticClass())
		{
			UObject* Obj = Prop->GetObjectPropertyValue_InContainer(this);
			UWidgetAnimation* anim = Cast<UWidgetAnimation>(Obj);


			FName animName = FName(*anim->GetName());


			AnimationsMap.Add(animName, anim);
		}
	}

}
