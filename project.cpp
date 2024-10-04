#include <iostream>
#include <string>
using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    bool isIssued;
    string student;
    Book* next; 

    Book(int id, string title, string author)
        : id(id), title(title), author(author), isIssued(false), student(""), next(NULL) {}
};

class Library {
private:
    Book* head;

    Book* getTail(Book* cur);
    Book* partition(Book* head, Book* end, Book** newHead, Book** newEnd);
    Book* quickSortRecur(Book* head, Book* end);

public:
    Library() : head(nullptr) {}

    void addBook(int id, string title, string author);
    Book* searchBookByID(int id);
    Book* searchBookByTitle(const string& title);
    bool issueBook(int id, const string& student);
    bool returnBook(int id);
    void listAllBooks();
    bool deleteBook(int id);
    void quickSort(Book** headRef);
};

void Library::addBook(int id, string title, string author) {
    Book* newBook = new Book(id, title, author);
    if (!head) {
        head = newBook;
    } else {
        Book* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newBook;
    }
}

Book* Library::searchBookByID(int id) {
    Book* temp = head;
    while (temp) {
        if (temp->id == id) return temp;
        temp = temp->next;
    }
    return nullptr;
}

Book* Library::searchBookByTitle(const string& title) {
    Book* temp = head;
    while (temp) {
        if (temp->title == title) return temp;
        temp = temp->next;
    }
    return nullptr;
}

bool Library::issueBook(int id, const string& student) {
    Book* book = searchBookByID(id);
    if (book && !book->isIssued) {
        book->isIssued = true;
        book->student = student; 
        return true;
    }
    return false;
}

bool Library::returnBook(int id) {
    Book* book = searchBookByID(id);
    if (book && book->isIssued) {
        book->isIssued = false;
        book->student = ""; 
        return true;
    }
    return false;
}

Book* Library::getTail(Book* cur) {
    while (cur != nullptr && cur->next != nullptr)
        cur = cur->next;
    return cur;
}

Book* Library::partition(Book* head, Book* end, Book** newHead, Book** newEnd) {
    Book* pivot = end;
    Book* prev = nullptr, * cur = head, * tail = pivot;

    while (cur != pivot) {
        if (cur->id < pivot->id) {
            if ((*newHead) == nullptr)
                (*newHead) = cur;

            prev = cur;  
            cur = cur->next;
        }
        else {
            if (prev)
                prev->next = cur->next;
            Book* temp = cur->next;
            cur->next = nullptr;
            tail->next = cur;
            tail = cur;
            cur = temp;
        }
    }

    if ((*newHead) == nullptr)
        (*newHead) = pivot;

    (*newEnd) = tail;

    return pivot;
}

Book* Library::quickSortRecur(Book* head, Book* end) {
    if (!head || head == end)
        return head;

    Book* newHead = nullptr, * newEnd = nullptr;

    Book* pivot = partition(head, end, &newHead, &newEnd);

    if (newHead != pivot) {
        Book* temp = newHead;
        while (temp->next != pivot)
            temp = temp->next;
        temp->next = nullptr;

        newHead = quickSortRecur(newHead, temp);

        temp = getTail(newHead);
        temp->next = pivot;
    }

    pivot->next = quickSortRecur(pivot->next, newEnd);

    return newHead;
}

void Library::quickSort(Book** headRef) {
    (*headRef) = quickSortRecur(*headRef, getTail(*headRef));
}

void Library::listAllBooks() {
    quickSort(&head);
    Book* temp = head;
    while (temp) {
        std::cout << "ID: " << temp->id << ", Title: " << temp->title << ", Author: " << temp->author << ", Status: " << (temp->isIssued ? "Issued to " + temp->student : "Available") << std::endl;
        temp = temp->next;
    }
}

bool Library::deleteBook(int id) {
    if (!head) return false;
    if (head->id == id) {
        Book* toDelete = head;
        head = head->next;
        delete toDelete;
        return true;
    }

    Book* temp = head;
    while (temp->next && temp->next->id != id) {
        temp = temp->next;
    }

    if (temp->next) {
        Book* toDelete = temp->next;
        temp->next = temp->next->next;
        delete toDelete;
        return true;
    }
    return false;
}


int main() {
    Library library;
    int choice;
    int id;
    string title, author, student;

    while (true) {
        cout << "\nLibrary Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Search Book by ID\n";
        cout << "3. Search Book by Title\n";
        cout << "4. Issue Book\n";
        cout << "5. Return Book\n";
        cout << "6. List All Books\n";
        cout << "7. Delete Book\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter Book ID: ";
            cin >> id;
            cout << "Enter Book Title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Enter Book Author: ";
            getline(cin, author);
            library.addBook(id, title, author);
            break;
        case 2:
            cout << "Enter Book ID to search: ";
            cin >> id;
            if (Book* book = library.searchBookByID(id)) {
                cout << "Book found - ID: " << book->id << ", Title: " << book->title << ", Author: " << book->author << ", Status: " << (book->isIssued ? "Issued to " + book->student : "Available") << endl;
            } else {
                cout << "Book not found.\n";
            }
            break;
        case 3:
            cout << "Enter Book Title to search: ";
            cin.ignore();
            getline(cin, title);
            if (Book* book = library.searchBookByTitle(title)) {
                cout << "Book found - ID: " << book->id << ", Title: " << book->title << ", Author: " << book->author << ", Status: " << (book->isIssued ? "Issued to " + book->student : "Available") << endl;
            } else {
                cout << "Book not found.\n";
            }
            break;
        case 4:
            cout << "Enter Book ID to issue: ";
            cin >> id;
            cout << "Enter Student Name: ";
            cin.ignore();
            getline(std::cin, student);
            if (library.issueBook(id, student)) {
                cout << "Book issued successfully to " << student << ".\n";
            } else {
                cout << "Book not found or already issued.\n";
            }
            break;
        case 5:
            cout << "Enter Book ID to return: ";
            cin >> id;
            if (library.returnBook(id)) {
                cout << "Book returned successfully.\n";
            } else {
                cout << "Book not found or not issued.\n";
            }
            break;
        case 6:
            library.listAllBooks();
            break;
        case 7:
            cout << "Enter Book ID to delete: ";
            cin >> id;
            if (library.deleteBook(id)) {
                cout << "Book deleted successfully.\n";
            } else {
                cout << "Book not found.\n";
            }
            break;
        case 8:
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }

    }
}
