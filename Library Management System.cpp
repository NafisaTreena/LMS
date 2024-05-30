#include <iostream>
#include <string>
#include <stdexcept>
#include <conio.h>
#include <windows.h>
#include <unordered_map>
#include <queue>
#include <vector>
#include <climits>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

class LoginSystem
{
private:
    string username;
    string password;

public:
    void showWelcomeScreen()
    {
        system("cls");
        cout << "\n\n";
        cout << "\t\t\t=====================================================";
        cout << "\n\n\t\t\t\tWELCOME TO LIBRARY MANAGEMENT SYSTEM";
        cout << "\n\n\t\t\t=====================================================";
        getch();
    }

    void authenticateUser()
    {
        const string validUsernames[] = {"treena", "takia", "arjina"};
        const string validPasswords[] = {"123456", "123456", "123456"};
        const int numUsers = sizeof(validUsernames) / sizeof(validUsernames[0]);

        while (true)
        {
            try
            {
                system("cls");
                cout << "\n\n";
                cout << "\t\t\t============================";
                cout << "\n\n\t\t\t\tLOGIN PANEL";
                cout << "\n\n\t\t\t============================";
                cout << "\n\n\n ENTER USER NAME: ";
                cin >> username;
                cout << "\n\n ENTER PASSWORD: ";
                password = "";
                for (int i = 1; i <= 6; i++)
                {
                    password += getch();
                    cout << "*";
                }
                bool authenticated = false;
                for (int i = 0; i < numUsers; i++)
                {
                    if (username == validUsernames[i] && password == validPasswords[i])
                    {
                        authenticated = true;
                        break;
                    }
                }

                if (authenticated)
                {
                    cout << "\n\n\n\t\t\tCONGRATULATIONS! LOGIN SUCCESSFUL";
                    cout << "\n\n\n\t\t\t\tLOADING";
                    for (int i = 1; i <= 6; i++)
                    {
                        Sleep(100);
                        cout << ".";
                    }
                    Sleep(100);
                    system("cls");
                    cout << "\n\n";
                    cout << "\t\t\t==========================";
                    cout << "\n\n\t\t\t\tMAIN MENU";
                    cout << "\n\n\t\t\t==========================\n";
                    break;
                }
                else
                {
                    throw invalid_argument("Invalid login credentials");
                }
            }
            catch (const invalid_argument& e)
            {
                cout << "\n\n\n\t\t\t" << e.what();
            }
            catch (const exception& e)
            {
                cout << "\n\n\n\t\t\tAn error occurred: " << e.what();
            }
            getch();
            getch();
        }
    }
};

struct Book
{
    int id;
    string title;
    string author;
    bool isBorrowed;
    Book* next;
};

struct Member
{
    int id;
    string name;
    Book* borrowedBooks;
    int borrowedCount;
    Member* next;
};

class BookList
{
private:
    Book* head;
    const string bookDataFile = "books.txt";

public:
    BookList() : head(nullptr) {}

    void addBook(int id, string title, string author)
    {
        try
        {
            Book* newBook = new Book{id, title, author, false, nullptr};
            newBook->next = head;
            head = newBook;

            ofstream outFile(bookDataFile, ios::app);
            if (outFile.is_open())
            {
                outFile << id << "," << title << "," << author << endl;
                outFile.close();
            }
            else
            {
                cerr << "Unable to open file for writing." << endl;
            }
        }
        catch (const bad_alloc& e)
        {
            cerr << "Memory allocation failed: " << e.what() << endl;
        }
    }

    void displayBooks()
    {
        ifstream inFile(bookDataFile);
        if (inFile.is_open())
        {
            string line;
            while (getline(inFile, line))
            {
                stringstream ss(line);
                string idStr, title, author;
                getline(ss, idStr, ',');
                getline(ss, title, ',');
                getline(ss, author, ',');

                int id = stoi(idStr);

                cout << "Name: " << title << " , Author: " << author << " , ID: " << id << endl;
            }
            inFile.close();
        }
        else
        {
            cerr << "Unable to open file for reading." << endl;
        }
    }

    Book* searchBook(int id)
    {
        Book* temp = head;
        while (temp != nullptr)
        {
            if (temp->id == id)
            {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void quickSort(Book* low, Book* high)
    {
        if (low == nullptr || high == nullptr || low == high || low->next == high) return;

        Book* pivot = partition(low, high);
        quickSort(low, pivot);
        quickSort(pivot->next, high);
    }

    Book* partition(Book* low, Book* high)
    {
        string pivot = high->title;
        Book* i = low->next;
        Book* j = low;
        while (i != high)
        {
            if (i->title < pivot)
            {
                j = (j == nullptr) ? low : j->next;
                swap(j->title, i->title);
                swap(j->id, i->id);
                swap(j->author, i->author);
                swap(j->isBorrowed, i->isBorrowed);
            }
            i = i->next;
        }
        j = (j == nullptr) ? low : j->next;
        swap(j->title, high->title);
        swap(j->id, high->id);
        swap(j->author, high->author);
        swap(j->isBorrowed, high->isBorrowed);
        return j;
    }

    void sortBooks()
    {
        if (head == nullptr) return;

        Book* tail = head;
        while (tail->next != nullptr)
        {
            tail = tail->next;
        }

        quickSort(head, tail);
    }


    Book* getHead()
    {
        return head;
    }

    Book* binarySearch(string title)
    {
        sortBooks();
        Book* low = head;
        Book* high = nullptr;

        while (low != high)
        {
            Book* mid = low;
            int count = 0;
            while (mid->next != high)
            {
                mid = mid->next;
                ++count;
            }
            for (int i = 0; i < count / 2; ++i)
            {
                mid = mid->next;
            }
            if (mid->title == title)
            {
                return mid;
            }

            else if (mid->title < title)
            {
                low = mid->next;
            }
            else
            {
                high = mid;
            }
        }
        return nullptr;
    }
};

class MemberList
{
private:
    Member* head;
    const string memberDataFile = "members.txt";

public:
    MemberList() : head(nullptr) {}

    void addMember(int id, string name)
    {
        try
        {
            Member* newMember = new Member{id, name, nullptr, 0, nullptr};
            newMember->next = head;
            head = newMember;

            ofstream outFile(memberDataFile, ios::app);
            if (outFile.is_open())
            {
                outFile << id << "," << name << endl;
                outFile.close();
            }
            else
            {
                cerr << "Unable to open file for writing." << endl;
            }
        }
        catch (const bad_alloc& e)
        {
            cerr << "Memory allocation failed: " << e.what() << endl;
        }
    }
    void displayMembers()
    {
        ifstream inFile(memberDataFile);
        if (inFile.is_open())
        {
            string line;
            while (getline(inFile, line))
            {
                stringstream ss(line);
                string idStr, name;
                getline(ss, idStr, ',');
                getline(ss, name, ',');

                int id = stoi(idStr);

                cout << "Name: " << name << " , ID: " << id << " , Borrowed books: ";

                Member* member = searchMember(id);
                if (member)
                {
                    cout << member->borrowedCount;
                }
                else
                {
                    cout << "0";
                }
                cout << endl;
            }
            inFile.close();
        }
        else
        {
            cerr << "Unable to open file for reading." << endl;
        }
    }


    Member* searchMember(int id)
    {
        Member* temp = head;
        while (temp != nullptr)
        {
            if (temp->id == id)
            {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void borrowBook(int memberId, Book* book)
    {
        Member* member = searchMember(memberId);
        if (member && book && !book->isBorrowed)
        {
            book->isBorrowed = true;
            Book* borrowedBook = new Book{book->id, book->title, book->author, book->isBorrowed, member->borrowedBooks};
            member->borrowedBooks = borrowedBook;
            member->borrowedCount++;
            cout << "Book borrowed successfully." << endl;
        }
        else
        {
            cout << "Error: Member not found or book already borrowed." << endl;
        }
    }

    void returnBook(int memberId, int bookId)
    {
        Member* member = searchMember(memberId);
        if (!member)
        {
            cout << "Member not found." << endl;
            return;
        }

        Book* prev = nullptr;
        Book* curr = member->borrowedBooks;
        while (curr != nullptr && curr->id != bookId)
        {
            prev = curr;
            curr = curr->next;
        }

        if (curr != nullptr)
        {
            if (prev != nullptr)
            {
                prev->next = curr->next;
            }
            else
            {
                member->borrowedBooks = curr->next;
            }

            delete curr;
            member->borrowedCount--;
            cout << "Book returned successfully." << endl;
        }
        else
        {
            cout << "Book not found in member's borrowed books." << endl;
        }
    }

    Member* bfsSearchByID(int id)
    {
        if (!head)
            return nullptr;

        queue<Member*> q;
        unordered_map<int, Member*> visited;

        q.push(head);
        visited[head->id] = head;

        while (!q.empty())
        {
            Member* current = q.front();
            q.pop();

            if (current->id == id)
                return current;

            Member* neighbor = current->next;
            while (neighbor)
            {
                if (visited.find(neighbor->id) == visited.end())
                {
                    q.push(neighbor);
                    visited[neighbor->id] = neighbor;
                }
                neighbor = neighbor->next;
            }
        }

        return nullptr;
    }
};

class LibraryGraph
{
private:
    unordered_map<int, unordered_map<int, int>> adjacencyList;

public:

    void addEdge(int memberId1, int memberId2, int weight)
    {
        adjacencyList[memberId1][memberId2] = weight;
        adjacencyList[memberId2][memberId1] = weight;
    }

    vector<int> findShortestPath(int memberId1, int memberId2)
    {
        unordered_map<int, int> distance;
        unordered_map<int, int> parent;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        for (auto it = adjacencyList.begin(); it != adjacencyList.end(); ++it)
        {
            int member = it->first;
            distance[member] = INT_MAX;
            parent[member] = -1;
        }

        distance[memberId1] = 0;
        pq.push({0, memberId1});

        while (!pq.empty())
        {
            int u = pq.top().second;
            pq.pop();

            for (auto it = adjacencyList[u].begin(); it != adjacencyList[u].end(); ++it)
            {
                int v = it->first;
                int weight = it->second;

                if (distance[v] > distance[u] + weight)
                {
                    distance[v] = distance[u] + weight;
                    parent[v] = u;
                    pq.push({distance[v], v});
                }
            }
        }

        vector<int> shortestPath;
        int current = memberId2;
        while (current != -1)
        {
            shortestPath.push_back(current);
            current = parent[current];
        }
        reverse(shortestPath.begin(), shortestPath.end());

        return shortestPath;
    }
};

void printMenu()
{
    cout << "\n1. Add Book" << endl;
    cout << "2. Add Member" << endl;
    cout << "3. Display Books" << endl;
    cout << "4. Display Members" << endl;
    cout << "5. Borrow Book" << endl;
    cout << "6. Return Book" << endl;
    cout << "7. Search Book by Title" << endl;
    cout << "8. Search Member by ID" << endl;
    cout << "9. Find Shortest Path Between Members" << endl;
    cout << "10. Exit" << endl;
    cout << "Enter your choice: ";
}

int main()
{
    LoginSystem loginSystem;
    loginSystem.showWelcomeScreen();
    loginSystem.authenticateUser();

    BookList books;
    MemberList members;
    LibraryGraph libraryGraph;

    libraryGraph.addEdge(1, 2, 1);
    libraryGraph.addEdge(1, 3, 1);
    libraryGraph.addEdge(2, 4, 1);
    libraryGraph.addEdge(3, 4, 1);
    libraryGraph.addEdge(4, 5, 1);

    int choice;
    do
    {
        printMenu();
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            int id;
            string title, author;
            cout << "\nEnter Book ID: ";
            cin >> id;
            cout << "Enter Book Title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Enter Book Author: ";
            getline(cin, author);
            books.addBook(id, title, author);
            cout << "Book added successfully.\n" << endl;
            break;
        }
        case 2:
        {
            int id;
            string name;
            cout << "\nEnter Member ID: ";
            cin >> id;
            cout << "Enter Member Name: ";
            cin.ignore();
            getline(cin, name);
            members.addMember(id, name);
            cout << "Member added successfully.\n" << endl;
            break;
        }
        case 3:
        {
            cout << "\nList of Books:\n" << endl;
            books.displayBooks();
            cout << "\n\n";
            break;
        }
        case 4:
        {
            cout << "\nMembers of the library:\n" << endl;
            members.displayMembers();
            cout << "\n\n";
            break;
        }
        case 5:
        {
            int memberId, bookId;
            cout << "\nEnter Member ID: ";
            cin >> memberId;
            cout << "Enter Book ID: ";
            cin >> bookId;
            Book* book = books.searchBook(bookId);
            if (book)
            {
                members.borrowBook(memberId, book);
            }
            else
            {
                cout << "Book not found.\n" << endl;
            }
            break;
        }
        case 6:
        {
            int memberId, bookId;
            cout << "\nEnter Member ID: ";
            cin >> memberId;
            cout << "Enter Book ID: ";
            cin >> bookId;
            members.returnBook(memberId, bookId);
            break;
        }
        case 7:
        {
            string title;
            cout << "\nEnter Book Title: ";
            cin.ignore();
            getline(cin, title);
            Book* book = books.binarySearch(title);
            if (book)
            {
                cout << "Book found: ID=" << book->id << ", Title=" << book->title << ", Author=" << book->author << endl;
            }
            else
            {
                cout << "Book not found.\n" << endl;
            }
            break;
        }
        case 8:
        {
            int id;
            cout << "\nEnter Member ID to search: ";
            cin >> id;
            Member* member = members.searchMember(id);
            if (member)
            {
                cout << "Member found: ID=" << member->id << ", Name=" << member->name << ", Borrowed Books=" << member->borrowedCount << endl;
            }
            else
            {
                cout << "Member not found.\n" << endl;
            }
            break;
        }
        case 9:
        {
            int memberId1, memberId2;
            cout << "\nEnter ID of first member: ";
            cin >> memberId1;
            cout << "Enter ID of second member: ";
            cin >> memberId2;

            vector<int> shortestPath = libraryGraph.findShortestPath(memberId1, memberId2);

            if (shortestPath.empty())
            {
                cout << "There is no path between the given members." << endl;
            }
            else
            {
                cout << "Shortest path between members: ";
                for (int memberId : shortestPath)
                {
                    cout << memberId << " ";
                }
                cout << endl;
            }
            break;
        }
        case 10:
        {
            cout << "Exiting program.\n";
            break;
        }
        default:
        {
            cout << "Invalid choice. Please enter a number between 1 and 10.\n";
            break;
        }
        }
    }
    while (choice != 10);

    return 0;
}
