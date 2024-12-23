#include <iostream>
#include <vector>
#include <limits> // for numeric_limits
#include <stdexcept> // for invalid_argument


class Node {
public:
    int data;
    Node* next;

    Node(int data = 0) : data(data), next(nullptr) {}
};

class LinkedList {
public:
    Node* head;
    Node* tail;

    LinkedList() : head(nullptr), tail(nullptr) {}

    void add(Node* node) {
        if (head == nullptr) {
            head = node;
            tail = node;
            return;
        }
        tail->next = node;
        tail = node;
    }

    static LinkedList* merge_ordered_lists(LinkedList* llist1, LinkedList* llist2) {
        if (llist1->head == nullptr || llist2->head == nullptr) {
            throw std::invalid_argument("Head is None!");
        }

        LinkedList* ordered_llist = new LinkedList();
        Node* n1 = llist1->head;
        Node* n2 = llist2->head;
        Node* tail = new Node();
        ordered_llist->add(tail);

        while (n1 && n2) {
            if (n1->data < n2->data) {
                tail->next = n1;
                n1 = n1->next;
            }
            else {
                tail->next = n2;
                n2 = n2->next;
            }
            tail = tail->next;
        }
        tail->next = (n1 != nullptr) ? n1 : n2;

        Node* temp = ordered_llist->head;
        ordered_llist->head = ordered_llist->head->next;
        delete temp;
        return ordered_llist;
    }

    void swap_max_min() {
        if (head == nullptr) return;

        Node* n = head;
        Node* min_n = n;
        Node* max_n = n;

        while (n->next) {
            n = n->next;
            min_n = (min_n->data < n->data) ? min_n : n;
            max_n = (max_n->data > n->data) ? max_n : n;
        }
        std::swap(min_n->data, max_n->data);
    }

    void insertion_sort() {
        if (head == nullptr || head->next == nullptr) return;

        Node* sorted_head = head;
        Node* current = head->next;
        sorted_head->next = nullptr;

        while (current) {
            Node* next_node = current->next;
            Node* prev = nullptr;
            Node* temp = sorted_head;
            while (temp && current->data > temp->data) {
                prev = temp;
                temp = temp->next;
            }
            current->next = temp;
            if (prev) {
                prev->next = current;
            }
            else {
                sorted_head = current;
            }
            current = next_node;
        }
        head = sorted_head;
    }

    void print_nodes() {
        Node* n = head;
        while (n) {
            std::cout << n->data << (n->next ? " " : "");
            n = n->next;
        }
        std::cout << std::endl;
    }
};


class Stack {
public:
    Node* head;
    Stack() : head(nullptr) {}

    void push(int data) {
        Node* newNode = new Node(data);
        newNode->next = head;
        head = newNode;
    }

    int pop() {
        if (head == nullptr) {
            throw std::runtime_error("Stack underflow");
        }
        int data = head->data;
        Node* temp = head;
        head = head->next;
        delete temp;
        return data;
    }

    bool isEmpty() {
        return head == nullptr;
    }

    void printStack() {
        Node* temp = head;
        while (temp != nullptr) {
            std::cout << temp->data << " ";
            temp = temp->next;
        }
        std::cout << std::endl;
    }
};



LinkedList* createListFromInput() {
    LinkedList* list = new LinkedList();
    int data;
    std::cout << "Enter data for the list (enter " << std::numeric_limits<int>::min() << " to finish):" << std::endl;
    while (std::cin >> data && data != std::numeric_limits<int>::min()) {
        list->add(new Node(data));
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return list;
}

int main() {
    LinkedList* list1, * list2;
    int choice;

    std::cout << "Choose the data input method:" << std::endl;
    std::cout << "1. Enter from keyboard" << std::endl;
    std::cout << "2. Use predefined lists" << std::endl;
    std::cout << "Enter your choice (1 or 2): ";
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Entering data for the first list:" << std::endl;
        list1 = createListFromInput();
        std::cout << "Entering data for the second list:" << std::endl;
        list2 = createListFromInput();
    }
    else {
        list1 = new LinkedList();
        list1->add(new Node(1));
        list1->add(new Node(3));
        list1->add(new Node(5));

        list2 = new LinkedList();
        list2->add(new Node(2));
        list2->add(new Node(4));
        list2->add(new Node(6));
    }

    try {
        LinkedList* mergedList = LinkedList::merge_ordered_lists(list1, list2);
        std::cout << "Merged list: ";
        mergedList->print_nodes();

        delete mergedList;
        delete list1;
        delete list2;

    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    //Part b) Swap max and min
    LinkedList* list3 = new LinkedList();
    list3->add(new Node(5));
    list3->add(new Node(1));
    list3->add(new Node(9));
    list3->add(new Node(2));
    std::cout << "List before swap: ";
    list3->print_nodes();
    list3->swap_max_min();
    std::cout << "List after swap: ";
    list3->print_nodes();
    delete list3; //Remember to free memory


    // Part c) Stack Implementation
    Stack myStack;
    std::vector<int> stackData = { 10, 20, 30, 40, 50 };
    std::cout << "Pushing elements onto the stack..." << std::endl;
    for (int data : stackData) {
        myStack.push(data);
        std::cout << "Stack: ";
        myStack.printStack();
    }
    std::cout << "Popping elements from the stack..." << std::endl;
    while (!myStack.isEmpty()) {
        std::cout << "Popped: " << myStack.pop() << std::endl;
        std::cout << "Stack: ";
        myStack.printStack();

    }


    // Part d) Insertion Sort
    LinkedList* list4 = new LinkedList();
    list4->add(new Node(5));
    list4->add(new Node(2));
    list4->add(new Node(8));
    list4->add(new Node(1));
    list4->add(new Node(9));
    std::cout << "List before sorting: ";
    list4->print_nodes();
    list4->insertion_sort();
    std::cout << "List after sorting: ";
    list4->print_nodes();
    delete list4; //Remember to free memory

    return 0;
}
