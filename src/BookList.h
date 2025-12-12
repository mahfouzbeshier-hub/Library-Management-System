#ifndef BOOKLIST_H
#define BOOKLIST_H

#include <string>
#include <vector>

struct Book {
    int id;
    std::string title;
    std::string author;
    int year;
    Book* next;

    Book(int id, std::string title, std::string author, int year)
        : id(id), title(title), author(author), year(year), next(nullptr) {}
};

class BookList {
public:
    BookList();
    ~BookList();

    bool addBook(int id, const std::string& title, const std::string& author, int year);
    bool deleteBook(int id);
    bool updateBook(int id, const std::string& title, const std::string& author, int year);
    Book* findBook(int id);
    std::vector<Book*> searchByText(const std::string& query);
    std::vector<Book*> getAllBooks() const;
    int countBooks() const;
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

private:
    Book* head;
    void clear();
};

#endif // BOOKLIST_H
