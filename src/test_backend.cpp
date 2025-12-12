#include "BookList.h"
#include <iostream>
#include <cassert>

void testBackend() {
    BookList list;
    
    // Test Add
    assert(list.addBook(1, "The Hobbit", "Tolkien", 1937));
    assert(list.addBook(2, "1984", "Orwell", 1949));
    assert(!list.addBook(1, "Duplicate", "User", 2020)); // Duplicate ID
    
    // Test Count
    assert(list.countBooks() == 2);
    
    // Test Find
    Book* b = list.findBook(1);
    assert(b != nullptr);
    assert(b->title == "The Hobbit");
    
    // Test Update
    assert(list.updateBook(1, "The Hobbit (Updated)", "J.R.R. Tolkien", 1937));
    b = list.findBook(1);
    assert(b->title == "The Hobbit (Updated)");
    assert(b->author == "J.R.R. Tolkien");
    
    // Test Search Text
    auto results = list.searchByText("Orwell");
    assert(results.size() == 1);
    assert(results[0]->id == 2);
    
    // Test Search Text (Case insensitive partial)
    results = list.searchByText("hobb");
    assert(results.size() == 1);
    
    // Test Delete
    assert(list.deleteBook(1));
    assert(list.countBooks() == 1);
    assert(list.findBook(1) == nullptr);
    
    // Test Save/Load
    list.saveToFile("test_books.txt");
    
    BookList list2;
    list2.loadFromFile("test_books.txt");
    assert(list2.countBooks() == 1);
    assert(list2.findBook(2) != nullptr);
    
    std::cout << "Backend tests passed successfully!" << std::endl;
}

int main() {
    testBackend();
    return 0;
}
