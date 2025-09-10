#include <iostream>
#include <vector>
#include <random>
#include <string>

using namespace std;

class Monster 
{                                           //추상 클래스 참조:https://musket-ade.tistory.com/entry/C-%EA%B0%80%EC%83%81-%ED%95%A8%EC%88%98-virtual-Function-%EC%88%9C%EC%88%98-%EA%B0%80%EC%83%81%ED%95%A8%EC%88%98-Pure-Virtual-Function-%EC%B6%94%EC%83%81-%ED%81%B4%EB%9E%98%EC%8A%A4-Abstract-Class
protected:                                                //외부 수정 X
	int Player;
	string Name;
	int Hp;                                               //체력
	int AttackPower;                                      //공격력
	int AttackSpeed;                                      //공격 속도
	int CriticalHit;                                      //치명타
	int DropGold;
	vector<string> Items;                                //드랍 아이템
	int StageLevel;
public:
	Monster(int Health, int AttackPower, int AttackSpeed, int CriticalHit, int DropGold, vector<string> DropItems)  //몬스터 생성 시 체력 값을 바로 Hp멤버에 넣음. 상속 클래스에서 자동 호출 방지.   {}은 생성자 본문. 비어있는 이유: 멤버 초기화 리스트에서 Hp를 초기화 했기 때문에 작업 필요X
		: Hp(Health + StageLevel * 10), AttackPower(AttackPower + StageLevel * 10), AttackSpeed(AttackSpeed + StageLevel * 0.1), CriticalHit(CriticalHit + StageLevel * 1), Items(DropItems) //스테이지 레벨에 비례해서 증가
	{}
	// get함수 참조: https://clairdeluneblog.tistory.com/entry/33-C%EC%9D%98-GET-SET
	string getName() const { return Name; }
	int getHp() const {return Hp;}                                                     //Hp 값을 읽기 전용으로 반환 + const > 수정X
	int getAttackPower() const { return AttackPower; }
	int getAttackSpeed() const { return AttackSpeed; }
	int getCriticalHit() const { return CriticalHit; }
	vector<string> getItems() const { return Items; }

	//set 함수 참조
	//void setName(string newName) { Name = newName; }
	//void setHp(int newHp) { Hp = newHp; }
	//void setAttackPower(int newAttackPower) { AttackPower = newAttackPower; }
	//void setAttackSpeed(int newAttackSpeed) { AttackSpeed = newAttackSpeed; }
	//void setCriticalHit(int newCriticalHit) { CriticalHit = newCriticalHit; }
	//void setItems(vector<string> newItems) { Items = newItems; }


};

class Goblin : public Monster 
{
public:
	Goblin() : Monster(100, 10, 1, 1, 10, { "고블린의 뼈", "고블린의 활" }) 
	{
		cout << "고블린 출현! 체력:100, 공격력:10" << endl;
	}                                                                           // {}은 생성자 본문. 추가로 실행할 코드가 있다면 여기에 작성.
};

class Orc : public Monster
{
public:
	Orc() : Monster(100, 10, 1, 1, 10, {"오크의 어금니", "오크의 창"}) 
	{
		cout << "오크 출현! 체력:100, 공격력:10" << endl;
	}
};

class Troll : public Monster 
{
public:
	Troll() : Monster(100, 10, 1, 1, 10, { "트롤의 심장", "트롤의 몽둥이" }) 
	{
		cout << "트롤 출현! 체력:100, 공격력:10" << endl;
	}
};

class Slime : public Monster
{
public:
	Slime() : Monster(100, 10, 1, 1, 10, { "슬라임의 액체", "슬라임의 핵" }) 
	{
		cout << "슬라임 출현! 체력:100, 공격력:10" << endl;
	}
};

class Dragon : public Monster
{
public:
	Dragon() : Monster(5000, 100, 10, 20, 500, { "드래곤의 이빨", "드래곤의 꼬리" })
	{
		cout << "보스 몬스터 드래곤 출현!! 체력: 5000, 공격력 100" << endl;	
	}
};


std::unique_ptr<Monster> SummonMonster()            //참조:https://modoocode.com/304
{ 
	std::random_device rd;                          //random device 생성
	std::mt19937 gen(rd());                         //난수 생성 엔진 초기화
	std::uniform_int_distribution<int> dis(0, 3);   //0~3까지 균등하게 난수열 분포

	switch (dis(gen))
	{
	case 0: return std::make_unique<Goblin>();
	case 1: return std::make_unique<Orc>();
	case 2: return std::make_unique<Troll>();
	case 3: return std::make_unique<Slime>();
	}
}

//몬스터의 플레이어 공격
void Monster::Attack(Player* player)
{
	if (!player) return;                                             //플레이어 확인
	bool alive = Player->setHp(Player->getHp() - AttackPower);       //플레이어 체력 감소
	if (alive)
	{
		cout << "플레이어 HP: " << player->getHp() << endl;          //생존
	}
	else
	{
		cout << "플레이어가 쓰러졌습니다." << endl;                  //죽음
	}
}

//플레이어의 몬스터 공격
void Player::Attack(Monster* monster) 
{
	if (!Monster) { cout << "공격 대상이 없습니다." << endl; return; }           //몬스터 확인
	cout << Monster->getName() << "에게 " << AttackPower << "의 피해!" << endl;;  //피해 
	bool alive = Monster->setHp(Monster->getHp() - AttackPower);           //피해받은 몬스터 Hp감소 후 생존 여부 확인
	if (alive) cout << "몬스터 HP: " << Monster->getHP() << endl;
	else       cout << "몬스터가 쓰러졌습니다! 승리!" <<endl;
}