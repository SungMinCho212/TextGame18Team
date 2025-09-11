#include <iostream>
#include <random>
#include <algorithm>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <limits>
using namespace std;

#include "Item.cpp"                
#include "Item2.h"
#include "HPPotion.h"
#include "HPPotion.cpp"
#include "MPPotion.h"
#include "MPPotion.cpp"
#include "TeamProJect Monster.cpp"  
#include "GameManager.h"

//화면 비우기
static inline void ClearScreen() 
{
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
}

//타자기 효과
static inline void Typewrite(const std::string& s, int per_char_ms = 12) 
{
    for (char c : s) 
    {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(per_char_ms));
    }
    std::cout << '\n';
}
static inline void TypewriteLines(const std::vector<std::string>& lines,
    int per_char_ms = 12, int per_line_ms = 60) 
{
    for (const auto& l : lines) 
    {
        Typewrite(l, per_char_ms);
        std::this_thread::sleep_for(std::chrono::milliseconds(per_line_ms));
    }
}

//엔터 기다리기
static inline void WaitEnter(const std::string& prompt = "계속하려면 Enter 키를 누르세요...") 
{
    std::cout << '\n' << prompt << std::flush;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string dummy;
    std::getline(std::cin, dummy);
}


static std::vector<std::string> g_log;
static inline void LogClear() { g_log.clear(); }
static inline void LogPush(const std::string& s) { g_log.push_back(s); }

//HUD
static void PrintHUD(const Character& ch, const Monster* m, const Inventory& inv, int stageLevel) 
{
    cout << "----------------------------------------\n";
    cout << "[Stage " << stageLevel << "]\n";
    cout << "[PLAYER] HP " << ch.getHP() << "/" << ch.getMaxHP()
        << "  ATK " << ch.getATK()
        << "  SPD " << ch.getSPD() << "\n";
    if (m) 
    {
        cout << "[MONSTER: " << m->GetName() << "] "
            << "HP " << m->GetHP()
            << "  ATK " << m->GetAttackPower()
            << "  SPD " << m->GetAttackSpeed() << "\n";
    }

    cout << "[INV] Gold " << inv.GetGold() << "\n";
}


static inline void ShowFrame(const Character& ch, const Monster* m, const Inventory& inv,
    int stageLevel, int per_char_ms = 12, int per_line_ms = 60)
{
    ClearScreen();
    PrintHUD(ch, m, inv, stageLevel);

    if (!g_log.empty())
    {
        cout << "-------------- LOG --------------\n";
        TypewriteLines(g_log, per_char_ms, per_line_ms);
        cout << "---------------------------------\n";
    }
    LogClear();
}

//전투 결과
static inline void ShowResultScreen(const Character& ch, const Inventory& inv, int stageLevel,
    const std::vector<std::string>& resultLines,
    int per_char_ms = 12, int per_line_ms = 60) 
{
    ClearScreen();
    cout << "=========== 전투 결과 ===========" << '\n';

    TypewriteLines(resultLines, per_char_ms, per_line_ms);

    cout << "---------------------------------" << '\n';

    cout << "[현재 상태]\n";

    PrintHUD(ch, nullptr, inv, stageLevel);

    ch.printStatus();

    WaitEnter();
}


static std::mt19937& RNG() { static std::mt19937 r{ std::random_device{}() }; return r; }

int GameManager::RandInt(int a, int b) const { std::uniform_int_distribution<int> d(a, b); return d(RNG()); }


static Character g_player;
static Inventory g_inv; 

// 메인 루프
void GameManager::Run() 
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1) 캐릭터 생성
    CreateCharacter();

    // 2) 상태창 확인 
    ShowStatus();

    // 3~7 반복
    while (true) 
    {
        // 3) 전투: 5,10,15… 스테이지는 보스
        const int stageBefore = stageLevel;
        bool bossFight = (stageLevel >= 5) && (stageLevel % 5 == 0);

        bool alive = BattleOnce(bossFight);
        if (!alive) 
        {
            std::vector<std::string> endLines = { "플레이어가 쓰러졌습니다. 게임 종료." };
            ShowResultScreen(g_player, g_inv, stageLevel, endLines);
            break;
        }

        if (!endlessMode && bossFight && stageBefore == 5)
        {
            auto choice = PromptAfterStage5Boss();
            if (choice == PostBossChoice::Quit)
            {
                ShowGameEndAndExit();
                return; 
            }
            else 
            {
                endlessMode = true;  
                
            }
        }
        // 5) 상점
        VisitShop();

        // 6) 상태
        ShowStatus();
        // 7) 다음 전투로 자동 반복
    }
}

// 캐릭터 생성
void GameManager::CreateCharacter() 
{
    ShowFrame(g_player, nullptr, g_inv, stageLevel);
    cout << "직업을 선택하세요 (1:Warrior 2:Mage 3:Assassin): ";
    int sel; cin >> sel;
    if (sel < 1 || sel > 3) sel = 1;

    g_player.chooseJob(static_cast<Job>(sel));
    g_player.recomputeDerived();

    
    g_inv.Clear();
    g_inv.AddGold(30);
    g_inv.AddItem(new HPPotion(1));
    g_inv.AddItem(new MPPotion(1));
    LogPush("초기 아이템: HP 포션 x1, MP 포션 x1 / Gold 30");

    
    ShowFrame(g_player, nullptr, g_inv, stageLevel);
    g_player.printStatus();
}

//스테이터스 보여주기
void GameManager::ShowStatus() 
{
    ShowFrame(g_player, nullptr, g_inv, stageLevel);
    g_player.printStatus();
    cout << "\n[인벤토리]\n";
    g_inv.PrintInventory();
    WaitEnter();
}

//전투
bool GameManager::BattleOnce(bool bossFight) 
{
    auto mon = bossFight ? SummonBoss(stageLevel) : SummonMonster(stageLevel);
    LogPush(std::string("전투 시작! [") + mon->GetName() + "] (Stage " + std::to_string(stageLevel) + (bossFight ? ", Boss)" : ")"));

    while (!mon->IsDead() && g_player.getHP() > 0) 
    {

        
        const int pSPD = g_player.getSPD();
        const int mSPD = mon->GetAttackSpeed();
        bool playerFirst;

        if (pSPD == mSPD) 
        {
            playerFirst = (RandInt(0, 1) == 0);
            LogPush(std::string("선공 판정: SPD 동률(") + std::to_string(pSPD) + ") → "
                + (playerFirst ? "플레이어" : std::string("[") + mon->GetName() + "]"));
        }
        else if (pSPD > mSPD) 
        {
            playerFirst = true;
            LogPush(std::string("선공: 플레이어 (") + std::to_string(pSPD) + " > " + std::to_string(mSPD) + ")");
        }
        else 
        {
            playerFirst = false;
            LogPush(std::string("선공: [") + mon->GetName() + "] (" + std::to_string(mSPD) + " > " + std::to_string(pSPD) + ")");
        }

        
        ShowFrame(g_player, mon.get(), g_inv, stageLevel);

        std::cout << "(1)공격 (2)아이템 사용 (3)도망: ";
        int cmd; std::cin >> cmd;

        auto useOneItem = [&]() 
            {
            ShowFrame(g_player, mon.get(), g_inv, stageLevel);
            g_inv.PrintInventory();
            std::cout << "사용할 아이템 번호(-1: 취소, 1부터 시작): ";
            int idx;
            std::cin >> idx;
            if (idx < 0) return;
            idx -= 1;
            if (!g_inv.UseByIndex(idx, g_player)) 
            {
                LogPush("아이템 사용 실패(인덱스/조건 확인).");
            }
            else 
            {
                LogPush("아이템 사용 완료.");
            }
            };

        auto playerAct = [&]() -> bool 
            {
            if (cmd == 1) 
            {
                mon->ApplyDamage(g_player.getATK());
                LogPush(std::string("[플레이어] 공격 → ") + std::to_string(g_player.getATK()) + " 피해");
            }
            else if (cmd == 2) 
            {
                useOneItem();
            }
            else if (cmd == 3) 
            {
                if (bossFight) 
                { LogPush("보스 전투에서는 도망칠 수 없습니다."); return true; }

                LogPush("도망쳤습니다.");
                
                ShowResultScreen(g_player, g_inv, stageLevel, g_log);
                LogClear();
                return false; 
            }
            return true;
            };

        auto monsterAct = [&]() 
            {
            int dmg = mon->RollDamage();
            g_player.takeDamageFrom(dmg);
            LogPush(std::string("[") + mon->GetName() + "] 공격 → " + std::to_string(dmg) + " 피해");
            };

        if (playerFirst) 
        {
            if (!playerAct()) return true;       // 도주
            if (mon->IsDead()) break;
            monsterAct();
        }
        else 
        {
            monsterAct();
            if (g_player.getHP() <= 0) break;
            if (!playerAct()) return true;
        }
    }

    if (g_player.getHP() <= 0) return false; 

    // 승리 보상
    int gold = bossFight ? (100 + stageLevel * 5) : (12 + stageLevel * 2);

    int exp = bossFight ? (60 + stageLevel * 5) : (50 + stageLevel * 2);

    g_inv.AddGold(gold);
    g_player.gainExp(exp);
    g_player.recomputeDerived();

    // 결과/보상/드랍 로그 구성
    LogPush("전투 승리! Gold +" + std::to_string(gold) + ", EXP +" + std::to_string(exp));

    auto drops = mon->TakeDrops();
    if (!drops.empty()) 
    {
        LogPush("[드랍]");
        for (auto* it : drops)
        {
            if (!it) continue;
            g_inv.AddItem(it);
            LogPush(std::string(" - ") + it->GetName() + " x" + std::to_string(it->GetCount()));
        }
    }

    
    ShowResultScreen(g_player, g_inv, stageLevel, g_log);
    LogClear();

    
    stageLevel += 1;
    return true;
}

// 보상 출력
void GameManager::ShowRewards(int gold, int exp) 
{
    LogPush("[보상] Gold +" + std::to_string(gold) + ", EXP +" + std::to_string(exp));
}

// 상점 
void GameManager::VisitShop()
{
    while (true)
    {
        ShowFrame(g_player, nullptr, g_inv, stageLevel);
        cout << "\n===== 상점 (Gold: " << g_inv.GetGold() << ") =====\n";
        cout << "0) 나가기\n";
        cout << "1) HP 포션(HP+30) 10 Gold\n";
        cout << "2) MP 포션(MP+30) 12 Gold\n";
        cout << "선택: ";
        int c; cin >> c;
        if (c == 0) break;

        Item* toBuy = nullptr;

        int price = 0;

        if (c == 1)
        {
            toBuy = new HPPotion(1); price = 10;
        }

        else if (c == 2)
        {
            toBuy = new MPPotion(1); price = 12;
        }

        if (!toBuy)
        {
            LogPush("해당 품목이 없습니다."); continue;
        }

        if (g_inv.SpendGold(price))
        {
            g_inv.AddItem(toBuy);
            LogPush(std::string("구매 완료: ") + toBuy->GetName());
        }
        else
        {
            LogPush("골드가 부족합니다.");
            delete toBuy; // 실패 시 메모리 정리
        }

    }
}
    GameManager::PostBossChoice GameManager::PromptAfterStage5Boss()
    {
        while (true)
        {
            ClearScreen();
            cout << "===== 선택지 =====\n";
            cout << "1) 무한 모드로 계속 싸운다 (죽을 때까지)\n";
            cout << "2) 게임을 종료한다\n";
            cout << "선택: ";

            int sel;
            if (!(cin >> sel)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (sel == 1) return PostBossChoice::Endless;
            if (sel == 2) return PostBossChoice::Quit;

            // 잘못 입력 시 다시
        }
    }
    void GameManager::ShowGameEndAndExit()
    {
        ClearScreen();
        cout << "====================\n";
        cout << "      게임 끝       \n";
        cout << "====================\n";
        // Enter 입력을 한 번 더 받으면 일반적으로 콘솔이 종료(더블클릭 실행 기준)
        WaitEnter("엔터를 누르면 종료됩니다...");
        // 여기서 단순히 Run()에서 return하면 main 종료와 함께 창 닫힘
    }
