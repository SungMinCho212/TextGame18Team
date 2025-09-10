#include <iostream>
#include <string>
using namespace std;

template <typename T>
class Inventory {
protected:
    T* Items;        // 아이템 배열
    int capacity;    // 가능한 아이템 수
    int size;        // 현재 아이템 수
    int gold;        // 소지금

public:
	// Getter 메서드들
    int GetSize() const { return size; } // 현재 아이템 수 반환
    int GetCapacity() const { return capacity; } // 최대 용량 반환
    int GetGold() const { return gold; } // 소지금 반환


	// 생성자
    Inventory(int cap = 0, int initGold = 0)
        : capacity(cap <= 0 ? 1 : cap), size(0), gold(initGold) 
        // 참이면 1, 거짓이면 cap (삼항 연산자), size는 0으로 초기화, gold는 initGold로 초기화
    {
        Items = new T[capacity]; // 아이템 배열 동적 할당
    }


	// 소멸자
    ~Inventory() {
        delete[] Items;  // 동적 할당 해제
        Items = nullptr;
    }

	// 아이템 추가, 인벤토리가 꽉 찼으면 메시지 출력
	void AddItem(const T& item) { // 참조로 받아서 복사 방지
		if (size < capacity) { // 인벤토리에 공간이 남아있으면
			Items[size++] = item; // 아이템 추가 후 size 증가
        }
        else {
            cout << "인벤토리가 꽉 찼습니다." << endl;
        }
    }


    // 아이템 제거, 인덱스가 유효하지 않으면 메시지 출력
    void Clear() { size = 0; }

    void PrintItems() const {
		if (size == 0) { // 사이즈가 0이면 비어있음 출력
            cout << "인벤토리가 비었습니다." << endl;
        }
        else {
			for (int i = 0; i < size; i++) { // 현재 아이템 수만큼 출력
                cout << i + 1 << ". " << Items[i] << endl;
            }
        }
    }
    
    //추가 지점
    void AddGold(int g) { gold += g; if (gold < 0) gold = 0; }
    bool SpendGold(int g) { if (g < 0) return false; if (gold < g) return false; gold -= g; return true; }
};

//class Potion {
//private:
//    vector<Potion> potions; // 포션 종류를 담을 벡터
//
//public:
//    void AddPotion(const Potion& potion) {
//        potions.push_back(potion); // 포션 추가
//	}
//
//};

int main() {
    Inventory<string> inv(10, 300); // 용량 10, 초기 소지금 300

    inv.AddItem("HP 포션");
    inv.AddItem("MP 포션");
    inv.AddItem("검");
    inv.AddItem("방패");
    
	cout << "==인벤토리==" << endl;
	inv.PrintItems(); // 지금까지 인벤토리에 들어간 아이템 출력

    cout << "=====================" << endl;
    cout << "최대 용량: " << inv.GetCapacity() << endl;
    cout << "현재 소지금: " << inv.GetGold() << "G" << endl;
    cout << "=====================" << endl;
}
