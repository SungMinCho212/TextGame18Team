#include <iostream>
#include <string>
using namespace std;

template <typename T>
class Inventory {
protected:
    T* Items;        // ������ �迭
    int capacity;    // ������ ������ ��
    int size;        // ���� ������ ��
    int gold;        // ������

public:
	// Getter �޼����
    int GetSize() const { return size; } // ���� ������ �� ��ȯ
    int GetCapacity() const { return capacity; } // �ִ� �뷮 ��ȯ
    int GetGold() const { return gold; } // ������ ��ȯ


	// ������
    Inventory(int cap = 0, int initGold = 0)
        : capacity(cap <= 0 ? 1 : cap), size(0), gold(initGold) 
        // ���̸� 1, �����̸� cap (���� ������), size�� 0���� �ʱ�ȭ, gold�� initGold�� �ʱ�ȭ
    {
        Items = new T[capacity]; // ������ �迭 ���� �Ҵ�
    }


	// �Ҹ���
    ~Inventory() {
        delete[] Items;  // ���� �Ҵ� ����
        Items = nullptr;
    }

	// ������ �߰�, �κ��丮�� �� á���� �޽��� ���
	void AddItem(const T& item) { // ������ �޾Ƽ� ���� ����
		if (size < capacity) { // �κ��丮�� ������ ����������
			Items[size++] = item; // ������ �߰� �� size ����
        }
        else {
            cout << "�κ��丮�� �� á���ϴ�." << endl;
        }
    }


    // ������ ����, �ε����� ��ȿ���� ������ �޽��� ���
    void Clear() { size = 0; }

    void PrintItems() const {
		if (size == 0) { // ����� 0�̸� ������� ���
            cout << "�κ��丮�� ������ϴ�." << endl;
        }
        else {
			for (int i = 0; i < size; i++) { // ���� ������ ����ŭ ���
                cout << i + 1 << ". " << Items[i] << endl;
            }
        }
    }
    
    //�߰� ����
    void AddGold(int g) { gold += g; if (gold < 0) gold = 0; }
    bool SpendGold(int g) { if (g < 0) return false; if (gold < g) return false; gold -= g; return true; }
};

//class Potion {
//private:
//    vector<Potion> potions; // ���� ������ ���� ����
//
//public:
//    void AddPotion(const Potion& potion) {
//        potions.push_back(potion); // ���� �߰�
//	}
//
//};

int main() {
    Inventory<string> inv(10, 300); // �뷮 10, �ʱ� ������ 300

    inv.AddItem("HP ����");
    inv.AddItem("MP ����");
    inv.AddItem("��");
    inv.AddItem("����");
    
	cout << "==�κ��丮==" << endl;
	inv.PrintItems(); // ���ݱ��� �κ��丮�� �� ������ ���

    cout << "=====================" << endl;
    cout << "�ִ� �뷮: " << inv.GetCapacity() << endl;
    cout << "���� ������: " << inv.GetGold() << "G" << endl;
    cout << "=====================" << endl;
}
