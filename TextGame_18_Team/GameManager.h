#pragma once

class GameManager {
public:
    void Run();

private:
    int stageLevel = 1; // 스테이지는 GameManager에서만 관리
    bool endlessMode = false;

    void CreateCharacter();
    void ShowStatus();
    bool BattleOnce(bool bossFight);    // 전투 1회(생존= true, 사망= false)
    void ShowRewards(int gold, int exp);
    void VisitShop();

    int  RandInt(int a, int b) const;
    enum class PostBossChoice { Endless, Quit };
    PostBossChoice PromptAfterStage5Boss();
    void ShowGameEndAndExit();
};
