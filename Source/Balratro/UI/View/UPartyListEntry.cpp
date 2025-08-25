// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/UPartyListEntry.h"

#include <Components/TextBlock.h>
#include "UI/MVVM/ViewModel/VM_JockerSlot.h"

void UUPartyListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	

	UpdateVisuals();
}

void UUPartyListEntry::UpdateVisuals()
{

}
