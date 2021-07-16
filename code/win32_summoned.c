#include <windows.h>
#include <stdio.h>

#define GAME_UPDATE(name) void name(void)
typedef GAME_UPDATE(GameUpdateFunc);
GAME_UPDATE(GameUpdateStub)
{
    printf("GameUpdateStub()\n");
    getc(stdin);
    return;
}

typedef struct Win32GameCode
{
    HMODULE gameCodeDLL;
    FILETIME lastDLLWriteTime;
    GameUpdateFunc* pGameUpdate;
    bool isValid;
} Win32GameCode;

FILETIME
Win32GetLastWriteTime(char* filename)
{
    FILETIME lastWriteTime = {0};

    WIN32_FIND_DATA findData;
    HANDLE findHandle = FindFirstFileA(filename, &findData);
    if(findHandle != INVALID_HANDLE_VALUE)
    {
        lastWriteTime = findData.ftLastWriteTime;
        FindClose(findHandle);
    }

    return(lastWriteTime);
}

Win32GameCode Win32LoadGameCode(char* filename)
{
    Win32GameCode result = {0};
    CopyFile(filename, "summoned_in_use.dll", FALSE);
    result.gameCodeDLL = LoadLibraryA("summoned_in_use.dll");
    result.lastDLLWriteTime = Win32GetLastWriteTime(filename);
    if(result.gameCodeDLL)
    {
        result.pGameUpdate = (GameUpdateFunc *)GetProcAddress(result.gameCodeDLL, "GameUpdate");

        result.isValid = result.pGameUpdate != 0;
    }

    if(!result.isValid)
    {
        result.pGameUpdate = &GameUpdateStub;
    }

    return result;
}

void Win32UnloadGameCode(Win32GameCode* pGameCode)
{
    if(pGameCode->gameCodeDLL)
    {
        FreeLibrary(pGameCode->gameCodeDLL);
    }

    pGameCode->isValid = false;
    pGameCode->pGameUpdate = &GameUpdateStub;
}

int main()
{
    char* filename = "summoned.dll";
    Win32GameCode gameCode = Win32LoadGameCode(filename);
    while(true)
    {
        FILETIME newDLLWriteTime = Win32GetLastWriteTime(filename);
        if(CompareFileTime(&gameCode.lastDLLWriteTime, &newDLLWriteTime))
        {
            printf("Reloading game code.\n");
            Win32UnloadGameCode(&gameCode);
            gameCode = Win32LoadGameCode(filename);
        }
        gameCode.pGameUpdate();
    }
    return 0;
}