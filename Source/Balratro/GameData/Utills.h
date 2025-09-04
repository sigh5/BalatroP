#pragma once

#include "CoreMinimal.h"


struct FRandomUtils
{
    // ���� RandomStream
    inline static FRandomStream RandomSeed;

    // �ʱ�ȭ
    static void Init()
    {
        RandomSeed.Initialize(FDateTime::Now().GetTicks());
    }

    // �迭���� ���� ���� �̰� ����
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
