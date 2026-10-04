#include <iostream>


using namespace std;

struct Node {
    int data;
    Node* next;

     Node(int x) {
         data = x;
         next = nullptr;
     }
};

void printList(Node* curr) {
    while (curr != nullptr) {
        cout << curr->data << " ";
        curr = curr->next;
    }

    cout << endl;
}

void removeCycle(Node* head) {
    if (head == nullptr || head->next == nullptr)
        return;

    Node* slow = head, *fast = head;

    slow = slow->next;
    fast = fast->next->next;

    while (fast && fast->next) {
        if (slow == fast)
            break;

        slow = slow->next;
        fast = fast->next->next;
    }

    if (slow == fast) {
        slow = head;

        if (slow == fast) {
            while (fast->next != slow) {
                fast = fast->next;
            }
        }  else {
            while (slow->next != fast->next) {
                slow = slow->next;
                fast = fast->next;
            }
        }

        fast->next = nullptr;
    }
}

int main() {
    Node* head = new Node(1);
    head->next = new Node(3);
    head->next->next = new Node(4);

    head->next->next->next = head->next;

    removeCycle(head);
    printList(head);

    return 0;
}
