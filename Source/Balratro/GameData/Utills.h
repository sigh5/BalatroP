#pragma once

#include "CoreMinimal.h"


struct FRandomUtils
{
    // 전역 RandomStream
    inline static FRandomStream RandomSeed;

    // 초기화
    static void Init()
    {
        RandomSeed.Initialize(FDateTime::Now().GetTicks());
    }

    // 배열에서 랜덤 원소 뽑고 제거
    template<typename T>
    static T PopRandomElement(TArray<T>& Array)
    {
        if (Array.Num() == 0)
        {
            return T();
        }

        int32 RandomIndex = RandomSeed.RandRange(0, Array.Num() - 1);

        T Element = Array[RandomIndex];
        Array.RemoveAt(RandomIndex);

        return Element;
    }
};
