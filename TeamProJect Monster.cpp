#include <iostream>
#include <vector>
#include <random>
#include <string>
using namespace std;

class Monster 
{                                           //추상 클래스 참조:https://musket-ade.tistory.com/entry/C-%EA%B0%80%EC%83%81-%ED%95%A8%EC%88%98-virtual-Function-%EC%88%9C%EC%88%98-%EA%B0%80%EC%83%81%ED%95%A8%EC%88%98-Pure-Virtual-Function-%EC%B6%94%EC%83%81-%ED%81%B4%EB%9E%98%EC%8A%A4-Abstract-Class
protected:                                                //외부 수정 X
	int Hp;                                               //체력
	int AttackPower;                                      //공격력
	int AttackSpeed;                                      //공격 속도
	int CriticalHit;                                      //치명타
	vector<string> Items;                                 //드랍 아이템
public:
	Monster(int Health, int AttackPower, int AttackSpeed, int CriticalHit, vector<string> DropItems)  //몬스터 생성 시 체력 값을 바로 Hp멤버에 넣음. 상속 클래스에서 자동 호출 방지.   {}은 생성자 본문. 비어있는 이유: 멤버 초기화 리스트에서 Hp를 초기화 했기 때문에 작업 필요X
		: Hp(Health + StageLevel * 10), AttackPower(AttackPower + StageLevel * 10), AttackSpeed(AttackSpeed + StageLevel * 0.1), CriticalHit(CriticalHit + StageLevel * 1), Items(DropItems) //스테이지 레벨에 비례해서 증가
	{}
	// get함수 참조: https://clairdeluneblog.tistory.com/entry/33-C%EC%9D%98-GET-SET
	int getHp() const {return Hp;}                                                     //Hp 값을 읽기 전용으로 반환 + const > 수정X
	int getAttackPower() const { return AttackPower; }
	int getAttackSpeed() const { return AttackSpeed; }
	int getCriticalHit() const { return CriticalHit; }
	vector<string> getItems() const { return Items; }

	//set 함수 참조
	//void setHp(int newHp) { Hp = newHp; }
	//void setAttackPower(int newAttackPower) { AttackPower = newAttackPower; }
	//void setAttackSpeed(int newAttackSpeed) { AttackSpeed = newAttackSpeed; }
	//void setCriticalHit(int newCriticalHit) { CriticalHit = newCriticaltHit; }
	//void setItems(vector<string> newItems) { Items = newItems; }


};

class Goblin : public Monster 
{
public:
	Goblin() : Monster(100, 10, 1, 1, { "고블린 아이템1", "고블린 아이템2" }) 
	{
		cout << "고블린 출현! 체력:100, 공격력:10" << endl;
	}                                                                           // {}은 생성자 본문. 추가로 실행할 코드가 있다면 여기에 작성.
};

class Orc : public Monster
{
public:
	Orc() : Monster(100, 10, 1, 1, {"오크 아이템1", "오크 아이템2"}) 
	{
		cout << "오크 출현! 체력:100, 공격력:10" << endl;
	}
};

class Troll : public Monster 
{
public:
	Troll() : Monster(100, 10, 1, 1, { "트롤 아이템1", "트롤 아이템2" }) 
	{
		cout << "트롤 출현! 체력:100, 공격력:10" << endl;
	}
};

class Slime : public Monster
{
public:
	Slime() : Monster(100, 10, 1, 1, { "슬라임 아이템1", "슬라임 아이템2" }) 
	{
		cout << "슬라임 출현! 체력:100, 공격력:10" << endl;
	}
};


unique_ptr<Monster> SummonMonster()                    //unique_ptr > 스마트 포인터, 메모리 자동관리
{ 
	int choice = rand % 4;                               //int choice = rand % 4 > 0~3중 랜덤 선택
	switch (choice)
	{
	 case 0: return make_unique<Goblin>();               //번호 할당
	 case 1: return make_unique<Orc>();
	 case 2: return make_unique<Troll>();
	 case 3: return make_unique<Slime>();
	}
	return nullptr;                                       //컴파일러 경고피하기 위한 장치 (지워도 무관)
}