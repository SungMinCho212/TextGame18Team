#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <random>

static std::mt19937 rng((std::random_device{})());
static int randint(int a, int b) { std::uniform_int_distribution<int> d(a, b); return d(rng); }
static double rand01() { std::uniform_real_distribution<double> d(0.0, 1.0); return d(rng); }

using namespace std;

// ===== ���� =====
enum class Job { Warrior = 1, Mage, Assassin };

struct Derived {
    int ATK = 0;       // ���ݷ� = STR * 1.5
    int SPD = 0;       // �ӵ�   = AGI % 10
    int CRITStat = 0;  // ġ��Ÿ = AGI * 2
    int MANA = 0;      // ����   = INT * 10  (MaxMP�� ���)
    int CritChance = 0; // ġ��Ÿ Ȯ��(%) = clamp(AGI*2 + �������ʽ�, 0~100)
};

struct Base {
    int LV = 1, EXP = 0;
    int STR = 10, AGI = 10, INTL = 10; // �⺻
    int HP = 100, MaxHP = 100;
    int MP = 0, MaxMP = 0;
};

class Character {
public:
    void chooseJob(Job j) {
        job = j;
        base = {}; // �⺻������ �ʱ�ȭ
        base.STR = 10; base.AGI = 10; base.INTL = 10;

        // --- ���� ���� ---
		// ����: HP/����
        // ������: ����/���ɡ�
        // �ϻ���: ��ø��, ġ��Ÿ Ȯ�� ���ʽ�
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
        case Job::Assassin:// AGI +50%, STR -10%, ġȮ +15%
            applyPercent(base.STR, -10);
            applyPercent(base.AGI, +50);
            applyPercent(base.INTL, 0);
            critBonusFlat = 15;
            break;
        }

        // --- �ִ�ġ ���� ---
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
        // ġȮ(%) = AGI*2 + ���� ���ʽ� (0~100 clamp)
        int cc = base.AGI * 2 + critBonusFlat;
        derived.CritChance = max(0, min(100, cc));
    }

    void printStatus() const {
        cout << "\n===== ĳ���� ���� =====\n";
        cout << "����: " << jobName() << "   ����: " << base.LV << "   ����ġ: " << base.EXP << "/100\n";
        cout << "HP: " << base.HP << "/" << base.MaxHP << "   MP: " << base.MP << "/" << base.MaxMP << "\n";
        cout << "STR: " << base.STR << "   AGI: " << base.AGI << "   INT: " << base.INTL << "\n";
        cout << "\n[�Ļ� ����]\n";
        cout << "���ݷ�(ATK) = ��*1.5 : " << derived.ATK << "\n";
        cout << "�ӵ�(SPD)   = ��ø%10 : " << derived.SPD << "\n";
        cout << "ġ��Ÿ(����)= ��ø*2  : " << derived.CRITStat << "\n";
        cout << "ġ��Ÿ Ȯ��  : " << derived.CritChance << "%\n";
        cout << "����(MANA)  = ����*10 : " << derived.MANA << " (MaxMP)\n";
        cout << "======================\n";
    }

    // ===== ���۵� =====
    void healHP(int amount, int& hpPotion) {
        if (hpPotion <= 0) { cout << "HP ������ �����մϴ�.\n"; return; }
        hpPotion--;
        base.HP = min(base.HP + amount, base.MaxHP);
        cout << "* HP +" << amount << " (���� HP����: " << hpPotion << ")  "
            << base.HP << "/" << base.MaxHP << "\n";
    }
    void healMP(int amount, int& mpPotion) {
        if (mpPotion <= 0) { cout << "MP ������ �����մϴ�.\n"; return; }
        mpPotion--;
        base.MP = min(base.MP + amount, base.MaxMP);
        cout << "* MP +" << amount << " (���� MP����: " << mpPotion << ")  "
            << base.MP << "/" << base.MaxMP << "\n";
    }
    void powerUpHP() { // MaxHP 2��, ���� HP ���� ����
        int oldMax = base.MaxHP;
        base.MaxHP *= 2;
        base.HP = base.HP * base.MaxHP / oldMax;
        cout << "* HP�� 2�谡 �Ǿ����ϴ�: " << base.HP << "/" << base.MaxHP << "\n";
    }
    void powerUpMP() {
        int oldMax = base.MaxMP;
        base.MaxMP *= 2;
        base.MP = base.MP * base.MaxMP / oldMax;
        cout << "* MP�� 2�谡 �Ǿ����ϴ�: " << base.MP << "/" << base.MaxMP << "\n";
    }
    void useSkill() { // ��ų: MP 50 �Ҹ�, ����� = ATK * 2 (ġȮ ���� ����)
        if (base.MP < 50) { cout << "��ų ��� �Ұ� (MP ����)\n"; return; }
        base.MP -= 50;
        int dmg = derived.ATK * 2;
        bool crit = (derived.CritChance >= 100) || (rand() % 100 < derived.CritChance);
        if (crit) dmg = int(dmg * 1.5); // ������ ũ�� ����
        cout << "* ��ų! MP-50  " << (crit ? "[ġ��Ÿ!] " : "")
            << "���� �����: " << dmg << "   MP: " << base.MP << "/" << base.MaxMP << "\n";
    }
    void useUltimate() { // �ʻ��: MP ���� �Ҹ�, ����� = ATK * 3 (+ġȮ)
        int cost = base.MP / 2;
        if (cost == 0) { cout << "�ʻ�� ��� �Ұ� (MP ����)\n"; return; }
        base.MP -= cost;
        int dmg = derived.ATK * 3;
        bool crit = (derived.CritChance >= 100) || (rand() % 100 < derived.CritChance);
        if (crit) dmg = int(dmg * 1.8);
        cout << "* �ʻ��! MP-" << cost << "  " << (crit ? "[ġ��Ÿ!] " : "")
            << "���� �����: " << dmg << "   MP: " << base.MP << "/" << base.MaxMP << "\n";
    }
    void gainExp(int amount) {
        base.EXP += amount;
        cout << "* ����ġ +" << amount << " (" << base.EXP << "/100)\n";
        while (base.EXP >= 100) { base.EXP -= 100; levelUp(); }
    }
    void trainAdd(const string& which, int val) {
        if (which == "STR") base.STR += val;
        else if (which == "AGI") base.AGI += val;
        else if (which == "INT") base.INTL += val;
        recomputeDerived();
        // Max ����
        int oldMaxHP = base.MaxHP, oldMaxMP = base.MaxMP;
        base.MaxHP = 100 + base.STR * 10;
        base.MaxHP = base.MaxHP + base.MaxHP * hpBonusPercent / 100;
        base.HP = min(base.HP, base.MaxHP);
        base.MaxMP = derived.MANA;
        base.MaxMP = base.MaxMP + base.MaxMP * mpBonusPercent / 100;
        base.MP = min(base.MP, base.MaxMP);
        cout << "* " << which << " +" << val << " ����. (HP "
            << oldMaxHP << "->" << base.MaxHP << ", MP "
            << oldMaxMP << "->" << base.MaxMP << ")\n";
    }

    void trainRandom(int rolls = 3);

    string jobName() const {
        switch (job) {
        case Job::Warrior: return "����";
        case Job::Mage:    return "������";
        case Job::Assassin:return "�ϻ���";
        }
        return "����";
    }
    //�߰� ����
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
        cout << "== ������! Lv." << base.LV << " ==\n";
        // ������ ���� �� ���� �ٸ���
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
        cout << "������ �����ϰ� HP/MP�� ���� ȸ���Ǿ����ϴ�.\n";
        printStatus();
    }

    int rollStatByJob();


};

int Character::rollStatByJob() {
    // weights: [STR, AGI, INT]
    int w0 = 0, w1 = 0, w2 = 0;
    switch (job) {
    case Job::Warrior:  w0 = 5; w1 = 2; w2 = 1; break; // ����: �� �켱
    case Job::Mage:     w0 = 1; w1 = 2; w2 = 5; break; // ������: ���� �켱
    case Job::Assassin: w0 = 2; w1 = 5; w2 = 1; break; // �ϻ���: ��ø �켱
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
        // �⺻ +1, 50% Ȯ���� +2
        int inc = 1 + (rand01() < 0.5 ? 1 : 0);
        // 10% Ȯ�� �뼺��: +2 �߰�
        if (rand01() < 0.10) inc += 2;

        if (which == 0) { base.STR += inc; addSTR += inc; }
        else if (which == 1) { base.AGI += inc; addAGI += inc; }
        else { base.INTL += inc; addINT += inc; }
    }

    // �Ļ�/�ִ�ġ ����
    recomputeDerived();
    int oldMaxHP = base.MaxHP, oldMaxMP = base.MaxMP;

    base.MaxHP = 100 + base.STR * 10;
    base.MaxHP = base.MaxHP + base.MaxHP * hpBonusPercent / 100;
    base.HP = min(base.HP, base.MaxHP);

    base.MaxMP = derived.MANA;
    base.MaxMP = base.MaxMP + base.MaxMP * mpBonusPercent / 100;
    base.MP = min(base.MP, base.MaxMP);

    cout << "* �Ʒ� ��� �� STR +" << addSTR << ", AGI +" << addAGI << ", INT +" << addINT << "\n";
    cout << "  MaxHP " << oldMaxHP << " �� " << base.MaxHP
        << ", MaxMP " << oldMaxMP << " �� " << base.MaxMP << "\n";
}


// ===== ��ƿ =====
void setPotion(int count, int* p_HPPotion, int* p_MPPotion) {
    *p_HPPotion = count; *p_MPPotion = count;
}
void printMenu() {
    cout << "\n<���� ���� �ý���>\n"
        << "0. ���� ����\n"
        << "1. HP ȸ��(+20)\n"
        << "2. MP ȸ��(+20)\n"
        << "3. HP ��ȭ(2��)\n"
        << "4. MP ��ȭ(2��)\n"
        << "5. ���� ��ų ���(MP 50)\n"
        << "6. �ʻ�� ���(MP ����)\n"
        << "7. ����ġ ���(+40)\n"
        << "8. �Ʒ�(STR/AGI/INT +1)\n"
        << "9. ������\n"
        << "��ȣ�� �������ּ���: ";
}

