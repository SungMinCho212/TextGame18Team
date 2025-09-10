#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <random>

static std::mt19937 rng((std::random_device{})());
static int randint(int a, int b) { std::uniform_int_distribution<int> d(a, b); return d(rng); }
static double rand01() { std::uniform_real_distribution<double> d(0.0, 1.0); return d(rng); }

using namespace std;

// ===== 직업 =====
enum class Job { Warrior = 1, Mage, Assassin };

struct Derived {
    int ATK = 0;       // 공격력 = STR * 1.5
    int SPD = 0;       // 속도   = AGI % 10
    int CRITStat = 0;  // 치명타 = AGI * 2
    int MANA = 0;      // 마나   = INT * 10  (MaxMP로 사용)
    int CritChance = 0; // 치명타 확률(%) = clamp(AGI*2 + 직업보너스, 0~100)
};

struct Base {
    int LV = 1, EXP = 0;
    int STR = 10, AGI = 10, INTL = 10; // 기본
    int HP = 100, MaxHP = 100;
    int MP = 0, MaxMP = 0;
};

class Character {
public:
    void chooseJob(Job j) {
        job = j;
        base = {}; // 기본값으로 초기화
        base.STR = 10; base.AGI = 10; base.INTL = 10;

        // --- 직업 보정 ---
		// 전사: HP/힘↑
        // 마법사: 마나/지능↑
        // 암살자: 민첩↑, 치명타 확률 보너스
        switch (job) {
        case Job::Warrior: // STR +40%, AGI +10%, INT -10%, MaxHP +30%
            applyPercent(base.STR, +40);
            applyPercent(base.AGI, +10);
            applyPercent(base.INTL, -10);
            hpBonusPercent = 30;
            critBonusFlat = 0;
            break;
        case Job::Mage:    // INT +50%, AGI +10%, STR -20%, MaxMP +30%
            applyPercent(base.STR, -20);
            applyPercent(base.AGI, +10);
            applyPercent(base.INTL, +50);
            mpBonusPercent = 30;
            critBonusFlat = 0;
            break;
        case Job::Assassin:// AGI +50%, STR -10%, 치확 +15%
            applyPercent(base.STR, -10);
            applyPercent(base.AGI, +50);
            applyPercent(base.INTL, 0);
            critBonusFlat = 15;
            break;
        }

        // --- 최대치 재계산 ---
        recomputeDerived();
        base.MaxHP = 100 + base.STR * 10;
        base.MaxHP = base.MaxHP + base.MaxHP * hpBonusPercent / 100;
        base.HP = base.MaxHP;

        base.MaxMP = derived.MANA;
        base.MaxMP = base.MaxMP + base.MaxMP * mpBonusPercent / 100;
        base.MP = base.MaxMP;
    }

    void recomputeDerived() {
        derived.ATK = int(base.STR * 1.5);
        derived.SPD = base.AGI % 10;
        derived.CRITStat = base.AGI * 2;
        derived.MANA = base.INTL * 10;
        // 치확(%) = AGI*2 + 직업 보너스 (0~100 clamp)
        int cc = base.AGI * 2 + critBonusFlat;
        derived.CritChance = max(0, min(100, cc));
    }

    void printStatus() const {
        cout << "\n===== 캐릭터 상태 =====\n";
        cout << "직업: " << jobName() << "   레벨: " << base.LV << "   경험치: " << base.EXP << "/100\n";
        cout << "HP: " << base.HP << "/" << base.MaxHP << "   MP: " << base.MP << "/" << base.MaxMP << "\n";
        cout << "STR: " << base.STR << "   AGI: " << base.AGI << "   INT: " << base.INTL << "\n";
        cout << "\n[파생 스탯]\n";
        cout << "공격력(ATK) = 힘*1.5 : " << derived.ATK << "\n";
        cout << "속도(SPD)   = 민첩%10 : " << derived.SPD << "\n";
        cout << "치명타(스탯)= 민첩*2  : " << derived.CRITStat << "\n";
        cout << "치명타 확률  : " << derived.CritChance << "%\n";
        cout << "마나(MANA)  = 지능*10 : " << derived.MANA << " (MaxMP)\n";
        cout << "======================\n";
    }

    // ===== 동작들 =====
    void healHP(int amount, int& hpPotion) {
        if (hpPotion <= 0) { cout << "HP 포션이 부족합니다.\n"; return; }
        hpPotion--;
        base.HP = min(base.HP + amount, base.MaxHP);
        cout << "* HP +" << amount << " (남은 HP포션: " << hpPotion << ")  "
            << base.HP << "/" << base.MaxHP << "\n";
    }
    void healMP(int amount, int& mpPotion) {
        if (mpPotion <= 0) { cout << "MP 포션이 부족합니다.\n"; return; }
        mpPotion--;
        base.MP = min(base.MP + amount, base.MaxMP);
        cout << "* MP +" << amount << " (남은 MP포션: " << mpPotion << ")  "
            << base.MP << "/" << base.MaxMP << "\n";
    }
    void powerUpHP() { // MaxHP 2배, 현재 HP 비율 유지
        int oldMax = base.MaxHP;
        base.MaxHP *= 2;
        base.HP = base.HP * base.MaxHP / oldMax;
        cout << "* HP가 2배가 되었습니다: " << base.HP << "/" << base.MaxHP << "\n";
    }
    void powerUpMP() {
        int oldMax = base.MaxMP;
        base.MaxMP *= 2;
        base.MP = base.MP * base.MaxMP / oldMax;
        cout << "* MP가 2배가 되었습니다: " << base.MP << "/" << base.MaxMP << "\n";
    }
    void useSkill() { // 스킬: MP 50 소모, 대미지 = ATK * 2 (치확 적용 예시)
        if (base.MP < 50) { cout << "스킬 사용 불가 (MP 부족)\n"; return; }
        base.MP -= 50;
        int dmg = derived.ATK * 2;
        bool crit = (derived.CritChance >= 100) || (rand() % 100 < derived.CritChance);
        if (crit) dmg = int(dmg * 1.5); // 간단한 크리 배율
        cout << "* 스킬! MP-50  " << (crit ? "[치명타!] " : "")
            << "예상 대미지: " << dmg << "   MP: " << base.MP << "/" << base.MaxMP << "\n";
    }
    void useUltimate() { // 필살기: MP 절반 소모, 대미지 = ATK * 3 (+치확)
        int cost = base.MP / 2;
        if (cost == 0) { cout << "필살기 사용 불가 (MP 부족)\n"; return; }
        base.MP -= cost;
        int dmg = derived.ATK * 3;
        bool crit = (derived.CritChance >= 100) || (rand() % 100 < derived.CritChance);
        if (crit) dmg = int(dmg * 1.8);
        cout << "* 필살기! MP-" << cost << "  " << (crit ? "[치명타!] " : "")
            << "예상 대미지: " << dmg << "   MP: " << base.MP << "/" << base.MaxMP << "\n";
    }
    void gainExp(int amount) {
        base.EXP += amount;
        cout << "* 경험치 +" << amount << " (" << base.EXP << "/100)\n";
        while (base.EXP >= 100) { base.EXP -= 100; levelUp(); }
    }
    void trainAdd(const string& which, int val) {
        if (which == "STR") base.STR += val;
        else if (which == "AGI") base.AGI += val;
        else if (which == "INT") base.INTL += val;
        recomputeDerived();
        // Max 갱신
        int oldMaxHP = base.MaxHP, oldMaxMP = base.MaxMP;
        base.MaxHP = 100 + base.STR * 10;
        base.MaxHP = base.MaxHP + base.MaxHP * hpBonusPercent / 100;
        base.HP = min(base.HP, base.MaxHP);
        base.MaxMP = derived.MANA;
        base.MaxMP = base.MaxMP + base.MaxMP * mpBonusPercent / 100;
        base.MP = min(base.MP, base.MaxMP);
        cout << "* " << which << " +" << val << " 적용. (HP "
            << oldMaxHP << "->" << base.MaxHP << ", MP "
            << oldMaxMP << "->" << base.MaxMP << ")\n";
    }

    void trainRandom(int rolls = 3);

    string jobName() const {
        switch (job) {
        case Job::Warrior: return "전사";
        case Job::Mage:    return "마법사";
        case Job::Assassin:return "암살자";
        }
        return "무직";
    }
    //추가 지점
    int getSPD() const { return derived.SPD; }
    int getATK() const { return derived.ATK; }

    int getHP() const { return base.HP; }
    int getMaxHP() const { return base.MaxHP; }
    void setHP(int v) { base.HP = std::max(0, std::min(v, base.MaxHP)); }

    void takeDamageFrom(int rawAtk, int myDef = 0) 
    {
        int real = std::max(1, rawAtk - myDef);
        setHP(base.HP - real);
    }
private:
    Job job = Job::Warrior;
    Base base;
    Derived derived;
    int hpBonusPercent = 0;
    int mpBonusPercent = 0;
    int critBonusFlat = 0;

    static void applyPercent(int& s, int p) { s = max(1, s + s * p / 100); }

    void levelUp() {
        base.LV++;
        cout << "== 레벨업! Lv." << base.LV << " ==\n";
        // 직업별 성장 맛 조금 다르게
        switch (job) {
        case Job::Warrior: base.STR += 3; base.AGI += 1; base.INTL += 1; break;
        case Job::Mage:    base.STR += 1; base.AGI += 1; base.INTL += 3; break;
        case Job::Assassin:base.STR += 1; base.AGI += 3; base.INTL += 1; break;
        }
        recomputeDerived();
        base.MaxHP = 100 + base.STR * 10;
        base.MaxHP = base.MaxHP + base.MaxHP * hpBonusPercent / 100;
        base.HP = base.MaxHP;
        base.MaxMP = derived.MANA;
        base.MaxMP = base.MaxMP + base.MaxMP * mpBonusPercent / 100;
        base.MP = base.MaxMP;
        cout << "스탯이 증가하고 HP/MP가 전부 회복되었습니다.\n";
        printStatus();
    }

    int rollStatByJob();


};

int Character::rollStatByJob() {
    // weights: [STR, AGI, INT]
    int w0 = 0, w1 = 0, w2 = 0;
    switch (job) {
    case Job::Warrior:  w0 = 5; w1 = 2; w2 = 1; break; // 전사: 힘 우선
    case Job::Mage:     w0 = 1; w1 = 2; w2 = 5; break; // 마법사: 지능 우선
    case Job::Assassin: w0 = 2; w1 = 5; w2 = 1; break; // 암살자: 민첩 우선
    }
    int total = w0 + w1 + w2;
    int r = randint(1, total);
    if (r <= w0) return 0;           // STR
    else if (r <= w0 + w1) return 1; // AGI
    else return 2;                   // INT
}

void Character::trainRandom(int rolls) {
    int addSTR = 0, addAGI = 0, addINT = 0;
    for (int i = 0; i < rolls; ++i) {
        int which = rollStatByJob();
        // 기본 +1, 50% 확률로 +2
        int inc = 1 + (rand01() < 0.5 ? 1 : 0);
        // 10% 확률 대성공: +2 추가
        if (rand01() < 0.10) inc += 2;

        if (which == 0) { base.STR += inc; addSTR += inc; }
        else if (which == 1) { base.AGI += inc; addAGI += inc; }
        else { base.INTL += inc; addINT += inc; }
    }

    // 파생/최대치 갱신
    recomputeDerived();
    int oldMaxHP = base.MaxHP, oldMaxMP = base.MaxMP;

    base.MaxHP = 100 + base.STR * 10;
    base.MaxHP = base.MaxHP + base.MaxHP * hpBonusPercent / 100;
    base.HP = min(base.HP, base.MaxHP);

    base.MaxMP = derived.MANA;
    base.MaxMP = base.MaxMP + base.MaxMP * mpBonusPercent / 100;
    base.MP = min(base.MP, base.MaxMP);

    cout << "* 훈련 결과 → STR +" << addSTR << ", AGI +" << addAGI << ", INT +" << addINT << "\n";
    cout << "  MaxHP " << oldMaxHP << " → " << base.MaxHP
        << ", MaxMP " << oldMaxMP << " → " << base.MaxMP << "\n";
}


// ===== 유틸 =====
void setPotion(int count, int* p_HPPotion, int* p_MPPotion) {
    *p_HPPotion = count; *p_MPPotion = count;
}
void printMenu() {
    cout << "\n<스탯 관리 시스템>\n"
        << "0. 상태 보기\n"
        << "1. HP 회복(+20)\n"
        << "2. MP 회복(+20)\n"
        << "3. HP 강화(2배)\n"
        << "4. MP 강화(2배)\n"
        << "5. 공격 스킬 사용(MP 50)\n"
        << "6. 필살기 사용(MP 절반)\n"
        << "7. 경험치 얻기(+40)\n"
        << "8. 훈련(STR/AGI/INT +1)\n"
        << "9. 나가기\n"
        << "번호를 선택해주세요: ";
}

