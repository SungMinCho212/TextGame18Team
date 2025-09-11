#pragma once

class GameManager {
public:
    void Run();

private:
    int stageLevel = 1; // ���������� GameManager������ ����
    bool endlessMode = false;

    void CreateCharacter();
    void ShowStatus();
    bool BattleOnce(bool bossFight);    // ���� 1ȸ(����= true, ���= false)
    void ShowRewards(int gold, int exp);
    void VisitShop();

    int  RandInt(int a, int b) const;
    enum class PostBossChoice { Endless, Quit };
    PostBossChoice PromptAfterStage5Boss();
    void ShowGameEndAndExit();
};
