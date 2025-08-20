#include "Character/MyPawn.h"
#include "Character/Component/HandRankingComponent.h"
#include "Character/Component/PlayerInfoComponent.h"
#include "Character/Component/CardAndDeckComponent.h"
#include "Character/Component/CalculatorHandRankingComponent.h"
#include "Character/Component/BlindComponent.h"
#include "Character/Component/JokerCardComponent.h"
#include "Character/Component/StoreComponent.h"
#include "Character/Component/RewardComponent.h"
#include "Character/Component/ItemSelectComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
	const auto HandRankingComponent = CreateDefaultSubobject<UHandRankingComponent>("HandRanking");
	const auto PlayerInfoComponent = CreateDefaultSubobject<UPlayerInfoComponent>("PlayerInfo");
	const auto CardAndDeckComponent = CreateDefaultSubobject<UCardAndDeckComponent>("CardDeck");
	const auto CalculatorHandPlayComponent = CreateDefaultSubobject<UCalculatorHandRankingComponent>("CalculatorHandPlay");
	const auto BlindComponent = CreateDefaultSubobject<UBlindComponent>("BlindComponent");
	const auto JokerCardComponent = CreateDefaultSubobject<UJokerCardComponent>("JokerCardComponent");
	const auto StoreComponent = CreateDefaultSubobject<UStoreComponent>("StoreComponent");
	const auto RewardComponent = CreateDefaultSubobject<URewardComponent>("RewardComponent");
	const auto ItemSelectComponent = CreateDefaultSubobject<UItemSelectComponent>("ItemSelectComponent");
}
