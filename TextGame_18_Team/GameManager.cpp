#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <limits>
#include <iomanip>

struct Status 
{
    int level = 1;
    int hp = 50;
    int maxHp = 50;
    int mp = 20;
    int maxMp = 20;
    int atk = 10;
    int def = 5;
    int spd = 5;
    int exp = 0;
    int nextExp = 20;
};

struct Player 
{
    std::string name;
    Status st;
};

struct Enemy 
{
    std::string name;
    int hp;
    int atk;
    int def;
    int expReward;
};

void PrintDivider() 
{
    std::cout << "----------------------------------------\n";
}

void PrintStatus(const Player& p) 
{
    PrintDivider();
    std::cout << "[상태창] " << p.name << "\n";
    PrintDivider();
    std::cout << "Lv. " << p.st.level << "\n";
    std::cout << "HP  : " << p.st.hp << " / " << p.st.maxHp << "\n";
    std::cout << "MP  : " << p.st.mp << " / " << p.st.maxMp << "\n";
    std::cout << "ATK : " << p.st.atk << "   DEF : " << p.st.def << "   SPD : " << p.st.spd << "\n";
    std::cout << "EXP : " << p.st.exp << " / " << p.st.nextExp << "\n";
    PrintDivider();
}

void PrintRegionBanner(int regionIndex) 
{
    std::cout << "\n=== 다음 지역으로 이동합니다: 지역 #" << regionIndex << " ===\n";
}

std::string ReadValidName() 
{
    while (true) 
    {
        std::string name;
        std::cout << "이름을 입력하세요(2글자 이상): ";
        std::getline(std::cin, name);

       
        auto trim = [](std::string s) 
            {
            size_t a = s.find_first_not_of(" \t\r\n");
            size_t b = s.find_last_not_of(" \t\r\n");
            if (a == std::string::npos) return std::string();
            return s.substr(a, b - a + 1);
            };
        name = trim(name);

        
        if (name.size() >= 2) return name;

        std::cout << "이름은 최소 2글자 이상이어야 합니다.\n";
    }
}

int main() 
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // 1) 이름 입력
    std::string name = ReadValidName();
    Player player;
    player.name = name;

    // 2) 상태창 표시
    PrintStatus(player);
    
}