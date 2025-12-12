#include "BookList.h"
#include <fstream>
#include <algorithm>
#include <sstream>

BookList::BookList() : head(nullptr) {}

BookList::~BookList() {
    clear();
}

void BookList::clear() {
    Book* current = head;
    while (current != nullptr) {
        Book* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}

bool BookList::addBook(int id, const std::string& title, const std::string& author, int year) {
    if (findBook(id) != nullptr) {
        return false; // ID already exists
    }

    Book* newBook = new Book(id, title, author, year);
    
    if (head == nullptr) {
        head = newBook;
    } else {
        Book* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newBook;
    }
    return true;
}

bool BookList::deleteBook(int id) {
    if (head == nullptr) return false;

    if (head->id == id) {
        Book* temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    Book* current = head;
    while (current->next != nullptr) {
        if (current->next->id == id) {
            Book* temp = current->next;
            current->next = current->next->next;
            delete temp;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool BookList::updateBook(int id, const std::string& title, const std::string& author, int year) {
    Book* book = findBook(id);
    if (book) {
        book->title = title;
        book->author = author;
        book->year = year;
        return true;
    }
    return false;
}

Book* BookList::findBook(int id) {
    Book* current = head;
    while (current != nullptr) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

std::vector<Book*> BookList::searchByText(const std::string& query) {
    std::vector<Book*> results;
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    Book* current = head;
    while (current != nullptr) {
        std::string lowerTitle = current->title;
        std::string lowerAuthor = current->author;
        std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        std::transform(lowerAuthor.begin(), lowerAuthor.end(), lowerAuthor.begin(), ::tolower);

        if (lowerTitle.find(lowerQuery) != std::string::npos || 
            lowerAuthor.find(lowerQuery) != std::string::npos) {
            results.push_back(current);
        }
        current = current->next;
    }
    return results;
}

std::vector<Book*> BookList::getAllBooks() const {
    std::vector<Book*> books;
    Book* current = head;
    while (current != nullptr) {
        books.push_back(current);
        current = current->next;
    }
    return books;
}

int BookList::countBooks() const {
    int count = 0;
    Book* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

bool BookList::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    Book* current = head;
    while (current != nullptr) {
        file << current->id << "|" << current->title << "|" << current->author << "|" << current->year << "\n";
        current = current->next;
    }
    file.close();
    return true;
}

bool BookList::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    clear(); // Clear existing list before loading

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> parts;

        while (std::getline(ss, segment, '|')) {
            parts.push_back(segment);
        }

        if (parts.size() >= 4) {
            try {
                int id = std::stoi(parts[0]);
                std::string title = parts[1];
                std::string author = parts[2];
                int year = std::stoi(parts[3]);
                addBook(id, title, author, year);
            } catch (...) {
                // Ignore malformed lines
            }
        }
    }
    file.close();
    return true;
}
