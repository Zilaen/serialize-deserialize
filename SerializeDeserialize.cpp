#include <iostream>
#include <fstream>
#include <unordered_map>

class ListNode {
public:
    ListNode* Prev;
    ListNode* Next;
    ListNode* Rand;
    std::string Data;

    ListNode(const std::string& data)
        : Prev(nullptr), Next(nullptr), Rand(nullptr), Data(data)
    {
    }
};

class ListRand {
public:
    ListNode* Head;
    ListNode* Tail;
    int Count;

    void Serialize(std::ofstream& s) {
        std::unordered_map<ListNode*, int> nodeIndices;
        int index = 0;
        ListNode* currentNode = Head;

        while (currentNode != nullptr) {
            nodeIndices[currentNode] = index;
            index++;

            currentNode = currentNode->Next;
        }

        s << Count << "\n";

        currentNode = Head;
        while (currentNode != nullptr) {
            s << currentNode->Data << "\n";
            if (currentNode->Rand != nullptr) {
                int randIndex = nodeIndices[currentNode->Rand];
                s << randIndex << "\n";
            }
            else {
                s << -1 << "\n";
            }

            currentNode = currentNode->Next;
        }
    }

    void Deserialize(std::ifstream& s) {
        std::unordered_map<int, ListNode*> nodeIndices;
        int index = 0;
        int randIndex = 0;

        s >> Count;

        for (int i = 0; i < Count; i++) {
            std::string data;
            s >> data;

            ListNode* newNode = new ListNode(data);
            nodeIndices[index] = newNode;
            index++;

            s >> randIndex;

            if (randIndex != -1) {
                newNode->Rand = nodeIndices[randIndex];
            }

            if (Head == nullptr) {
                Head = newNode;
                Tail = newNode;
            }
            else {
                Tail->Next = newNode;
                newNode->Prev = Tail;
                Tail = newNode;
            }
        }
    }
};

int main() {
    ListRand list;
    list.Count = 3;

    ListNode* node1 = new ListNode("Node 1");
    ListNode* node2 = new ListNode("Node 2");
    ListNode* node3 = new ListNode("Node 3");

    node1->Next = node2;
    node1->Rand = node3;
    node2->Next = node3;
    node2->Rand = node1;
    node3->Prev = node2;
    node3->Rand = node2;

    list.Head = node1;
    list.Tail = node3;

    std::ofstream outFile("list.txt");
    list.Serialize(outFile);
    outFile.close();

    ListRand deserializedList;
    std::ifstream inFile("list.txt");
    deserializedList.Deserialize(inFile);
    inFile.close();

    // проверяем, что структура списка десериализована правильно

    ListNode* current = deserializedList.Head;
    while (current != nullptr) {
        std::cout << "Data: " << current->Data << std::endl;
        if (current->Rand != nullptr) {
            std::cout << "Rand: " << current->Rand->Data << std::endl;
        }
        else {
            std::cout << "Rand: nullptr" << std::endl;
        }

        current = current->Next;
    }

    // очищаем память

    current = deserializedList.Head;
    while (current != nullptr) {
        ListNode* next = current->Next;
        delete current;
        current = next;
    }

    return 0;
}