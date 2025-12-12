#include "MainWindow.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QApplication>
#include <QFrame>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    applyStyles();
    
    // Load data
    bookList.loadFromFile("books.txt");
    
    // Initial State
    stackedWidget->setCurrentIndex(0); // Add Book Page
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUi() {
    setWindowTitle("Library Management System");
    resize(1000, 650);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Sidebar
    mainLayout->addWidget(createSidebar(), 1);

    // Main Content
    stackedWidget = new QStackedWidget();
    mainLayout->addWidget(stackedWidget, 4);

    // Add Pages (Index mapping matches buttons)
    // 0: Add Book
    // 1: Delete
    // 2: Update
    // 3: Search ID
    // 4: Search Text
    // 5: Display All
    // 6: Count
    
    stackedWidget->addWidget(createAddBookPage());
    stackedWidget->addWidget(createDeletePage());
    stackedWidget->addWidget(createUpdatePage());
    stackedWidget->addWidget(createSearchIdPage());
    stackedWidget->addWidget(createSearchTextPage());
    stackedWidget->addWidget(createDisplayAllPage());
    stackedWidget->addWidget(createCountPage());
}

QWidget* MainWindow::createSidebar() {
    QFrame* sidebar = new QFrame();
    sidebar->setObjectName("Sidebar");
    
    QVBoxLayout* layout = new QVBoxLayout(sidebar);
    layout->setContentsMargins(0, 20, 0, 20);
    layout->setSpacing(5);

    QLabel* title = new QLabel("LMS");
    title->setObjectName("SidebarTitle");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    layout->addSpacing(30);

    auto addSidebarBtn = [&](QString text, int index, QString objName = "") {
        QPushButton* btn = new QPushButton(text);
        btn->setCheckable(true);
        btn->setAutoExclusive(true);
        btn->setFixedHeight(50);
        btn->setCursor(Qt::PointingHandCursor);
        if(!objName.isEmpty()) btn->setObjectName(objName);
        
        connect(btn, &QPushButton::clicked, [=]() { 
            showPage(index); 
        });
        
        layout->addWidget(btn);
        return btn;
    };

    QPushButton* btnAdd = addSidebarBtn("Add Book", 0, "BtnAddBookGreen");
    addSidebarBtn("Delete Book", 1);
    addSidebarBtn("Update Book", 2);
    addSidebarBtn("Search ID", 3);
    addSidebarBtn("Search Text", 4);
    addSidebarBtn("Display All", 5);
    addSidebarBtn("Count Books", 6);

    layout->addStretch();

    QPushButton* btnExit = new QPushButton("Save & Exit");
    btnExit->setObjectName("BtnExitRed");
    btnExit->setCursor(Qt::PointingHandCursor);
    connect(btnExit, &QPushButton::clicked, this, &MainWindow::saveAndExit);
    layout->addWidget(btnExit);

    btnAdd->setChecked(true); // Default
    return sidebar;
}

void MainWindow::applyStyles() {
    this->setStyleSheet(R"(
        /* Global Reset */
        * { font-family: 'Segoe UI', sans-serif; }

        /* Main Window */
        QMainWindow { background-color: #ECF0F1; } /* Light Gray Main Area */

        /* Sidebar Styling */
        QFrame#Sidebar {
            background-color: #2C3E50; /* Dark Gray Sidebar */
            border: none;
        }

        /* Sidebar Title */
        QLabel#SidebarTitle {
            color: white;
            font-size: 28px;
            font-weight: bold;
            letter-spacing: 2px;
        }

        /* Standard Sidebar Buttons */
        QFrame#Sidebar QPushButton {
            background-color: transparent;
            color: #BDC3C7;
            text-align: left;
            padding-left: 30px;
            border: none;
            font-size: 16px;
            border-radius: 0px;
        }
        
        QFrame#Sidebar QPushButton:hover {
            background-color: #34495E;
            color: white;
        }

        /* Selected State (Teal/Green) */
        QFrame#Sidebar QPushButton:checked {
            background-color: #16A085; /* Teal/Green */
            color: white;
            border-left: 5px solid #1ABC9C;
        }


        QPushButton#BtnAddBookGreen {
            color: #2ECC71; /* Green Text */
        }
        QPushButton#BtnAddBookGreen:checked {
            color: white; /* White on Teal background */
        }

        /* Exit Button (Red) */
        QPushButton#BtnExitRed {
            background-color: #C0392B; /* Red */
            color: white;
            margin: 20px;
            border-radius: 5px;
            padding: 10px;
            text-align: center;
            padding-left: 0;
            height: 40px;
        }
        QPushButton#BtnExitRed:hover {
            background-color: #E74C3C;
        }

        /* Content Area Headers */
        QLabel.Header {
            font-size: 24px;
            font-weight: bold;
            color: #2C3E50;
            margin-bottom: 20px;
        }

        /* Input Fields */
        QLineEdit {
            background-color: white;
            color: black;
            border: 1px solid #BDC3C7;
            border-radius: 5px;
            padding: 10px;
            font-size: 14px;
            margin-bottom: 10px;
        }
        QLineEdit:focus {
            border: 2px solid #3498DB;
        }

        /* Labels */
        QLabel {
            font-size: 14px;
            color: #34495E;
            font-weight: bold;
        }

        /* Main Action Button (Blue - Add Book Page) */
        QPushButton#BtnMainAction {
            background-color: #2980B9; /* Blue */
            color: white;
            border-radius: 5px;
            padding: 12px 20px;
            font-size: 16px;
            font-weight: bold;
            min-width: 150px;
        }
        QPushButton#BtnMainAction:hover {
            background-color: #3498DB;
        }
        QPushButton#BtnMainAction:pressed {
            background-color: #2573A7;
        }

        /* Table Widget */
        QTableWidget {
            border: 1px solid #BDC3C7;
            background-color: white;
            color: black;
            gridline-color: #ECF0F1;
        }
        QHeaderView::section {
            background-color: #34495E;
            color: white;
            padding: 8px;
            border: none;
            font-weight: bold;
        }
    )");
}

// --- Page Creators ---

QWidget* MainWindow::createAddBookPage() {
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(50, 50, 50, 50);
    layout->setSpacing(15);

    QLabel* header = new QLabel("Add New Book");
    header->setProperty("class", "Header"); // We can use qproperty-class if handled in Qt, but CSS selector by property is slightly diff. 
    // Easier: Just style specific widget or use object name.
    header->setStyleSheet("font-size: 24px; font-weight: bold; color: #2C3E50; margin-bottom: 20px;");
    layout->addWidget(header);

    // Form Container (Visual grouping not strictly required by prompt but looks nice)
    // Prompt says inputs directly.

    layout->addWidget(new QLabel("Book ID"));
    idInput = new QLineEdit(); 
    layout->addWidget(idInput);

    layout->addWidget(new QLabel("Title"));
    titleInput = new QLineEdit();
    layout->addWidget(titleInput);

    layout->addWidget(new QLabel("Author"));
    authorInput = new QLineEdit();
    layout->addWidget(authorInput);

    layout->addWidget(new QLabel("Year"));
    yearInput = new QLineEdit();
    layout->addWidget(yearInput);

    layout->addSpacing(20);

    QPushButton* btn = new QPushButton("Add Book");
    btn->setObjectName("BtnMainAction");
    btn->setCursor(Qt::PointingHandCursor);
    connect(btn, &QPushButton::clicked, this, &MainWindow::onAddBookClicked);
    
    // Left align or center? "A 'Add Book' button in blue below the input fields." default stretch is usually full width or left.
    // Let's make it fixed width left aligned for clean look.
    btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addWidget(btn);

    layout->addStretch();
    return page;
}

QWidget* MainWindow::createDisplayAllPage() {
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);

    QLabel* header = new QLabel("All Books");
    header->setStyleSheet("font-size: 24px; font-weight: bold; color: #2C3E50; margin-bottom: 20px;");
    layout->addWidget(header);

    bookTable = new QTableWidget();
    bookTable->setColumnCount(4);
    bookTable->setHorizontalHeaderLabels({"ID", "Title", "Author", "Year"});
    bookTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    bookTable->setAlternatingRowColors(true);
    bookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bookTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookTable->verticalHeader()->setVisible(false);
    layout->addWidget(bookTable);

    return page;
}

// Minimal implementations for other pages to complete the Sidebar
QWidget* MainWindow::createDeletePage() {
    QWidget* page = new QWidget();
    QVBoxLayout* l = new QVBoxLayout(page); l->setContentsMargins(50,50,50,50);
    QLabel* h = new QLabel("Delete Book"); h->setStyleSheet("font-size: 24px; font-weight: bold; color: #2C3E50;");
    l->addWidget(h);
    l->addWidget(new QLabel("Book ID to delete:"));
    deleteIdInput = new QLineEdit(); l->addWidget(deleteIdInput);
    QPushButton* btn = new QPushButton("Delete"); btn->setObjectName("BtnMainAction");
    btn->setStyleSheet("background-color: #C0392B;"); // Red for delete action
    connect(btn, &QPushButton::clicked, this, &MainWindow::onDeleteBookClicked);
    l->addWidget(btn); l->addStretch();
    return page;
}

QWidget* MainWindow::createUpdatePage() {
    QWidget* page = new QWidget();
    QVBoxLayout* l = new QVBoxLayout(page); l->setContentsMargins(50,50,50,50);
    l->addWidget(new QLabel("Update Book (Enter ID to load first)"));
    updateIdInput = new QLineEdit(); updateIdInput->setPlaceholderText("ID"); l->addWidget(updateIdInput);
    
    updateForm = new QWidget();
    QVBoxLayout* fl = new QVBoxLayout(updateForm); fl->setContentsMargins(0,0,0,0);
    updateTitleInput = new QLineEdit(); fl->addWidget(new QLabel("Title")); fl->addWidget(updateTitleInput);
    updateAuthorInput = new QLineEdit(); fl->addWidget(new QLabel("Author")); fl->addWidget(updateAuthorInput);
    updateYearInput = new QLineEdit(); fl->addWidget(new QLabel("Year")); fl->addWidget(updateYearInput);
    QPushButton* btn = new QPushButton("Save Changes"); btn->setObjectName("BtnMainAction");
    connect(btn, &QPushButton::clicked, this, &MainWindow::onUpdateBookClicked);
    fl->addWidget(btn);
    // updateForm->setVisible(false); // Can logic later

    l->addWidget(updateForm); l->addStretch();
    return page;
}

QWidget* MainWindow::createSearchIdPage() {
    QWidget* page = new QWidget();
    QVBoxLayout* l = new QVBoxLayout(page); l->setContentsMargins(50,50,50,50);
    l->addWidget(new QLabel("Search by ID"));
    searchIdInput = new QLineEdit(); l->addWidget(searchIdInput);
    QPushButton* btn = new QPushButton("Search"); btn->setObjectName("BtnMainAction");
    connect(btn, &QPushButton::clicked, this, &MainWindow::onSearchIdClicked);
    l->addWidget(btn);
    searchIdResult = new QLabel("Result will appear here.");
    l->addWidget(searchIdResult); l->addStretch();
    return page;
}

QWidget* MainWindow::createSearchTextPage() {
    QWidget* page = new QWidget();
    QVBoxLayout* l = new QVBoxLayout(page); l->setContentsMargins(50,50,50,50);
    l->addWidget(new QLabel("Search by Text"));
    searchTextInput = new QLineEdit(); l->addWidget(searchTextInput);
    QPushButton* btn = new QPushButton("Search"); btn->setObjectName("BtnMainAction");
    connect(btn, &QPushButton::clicked, this, &MainWindow::onSearchTextClicked);
    l->addWidget(btn);
    searchTable = new QTableWidget(); searchTable->setColumnCount(4);
    searchTable->setHorizontalHeaderLabels({"ID", "Title", "Author", "Year"});
    searchTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    l->addWidget(searchTable);
    return page;
}

QWidget* MainWindow::createCountPage() {
    QWidget* page = new QWidget();
    QVBoxLayout* l = new QVBoxLayout(page); l->setAlignment(Qt::AlignCenter);
    QLabel* h = new QLabel("Total Books");
    h->setStyleSheet("font-size: 32px; color: #7f8c8d;");
    countLabel = new QLabel("0");
    countLabel->setStyleSheet("font-size: 80px; font-weight: bold; color: #2C3E50;");
    l->addWidget(h); l->addWidget(countLabel);
    return page;
}

// --- Logic ---

void MainWindow::showPage(int index) {
    stackedWidget->setCurrentIndex(index);
    if(index == 5) refreshBookList();
    if(index == 6) countLabel->setText(QString::number(bookList.countBooks()));
}

void MainWindow::refreshBookList() {
    bookTable->setRowCount(0);
    auto books = bookList.getAllBooks();
    for(auto b : books) {
        int r = bookTable->rowCount();
        bookTable->insertRow(r);
        bookTable->setItem(r, 0, new QTableWidgetItem(QString::number(b->id)));
        bookTable->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(b->title)));
        bookTable->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(b->author)));
        bookTable->setItem(r, 3, new QTableWidgetItem(QString::number(b->year)));
    }
}

void MainWindow::onAddBookClicked() {
    bool okId, okYear;
    int id = idInput->text().toInt(&okId);
    int year = yearInput->text().toInt(&okYear);
    
    if(!okId || !okYear || titleInput->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Invalid Input");
        return;
    }
    
    if(bookList.addBook(id, titleInput->text().toStdString(), authorInput->text().toStdString(), year)) {
        QMessageBox::information(this, "Success", "Book Added");
        idInput->clear(); titleInput->clear(); authorInput->clear(); yearInput->clear();
        refreshBookList();
    } else {
        QMessageBox::warning(this, "Error", "ID already exists");
    }
}

void MainWindow::onDeleteBookClicked() {
    int id = deleteIdInput->text().toInt();
    if(bookList.deleteBook(id)) {
        QMessageBox::information(this,"Success","Deleted");
        refreshBookList();
    }
    else QMessageBox::warning(this,"Error","Not Found");
}

void MainWindow::onUpdateBookClicked() {
    bool okId, okYear;
    int id = updateIdInput->text().toInt(&okId);
    int year = updateYearInput->text().toInt(&okYear);
    
    if(!okId) {
        QMessageBox::warning(this, "Error", "Invalid Book ID");
        return;
    }

    if(updateTitleInput->text().isEmpty()) { // Minimal validation
         // If fields are empty, maybe we shouldn't update? 
         // Assuming user fills all fields for now as per simple CRUD
    }

    if(bookList.updateBook(id, updateTitleInput->text().toStdString(), updateAuthorInput->text().toStdString(), year)) {
        QMessageBox::information(this, "Success", "Book Updated");
        refreshBookList();
    } else {
        QMessageBox::warning(this, "Error", "Book ID not found");
    }
}
void MainWindow::onSearchIdClicked() {
     Book* b = bookList.findBook(searchIdInput->text().toInt());
     if(b) searchIdResult->setText("Found: " + QString::fromStdString(b->title));
     else searchIdResult->setText("Not Found");
}
void MainWindow::onSearchTextClicked() {
    searchTable->setRowCount(0);
    auto books = bookList.searchByText(searchTextInput->text().toStdString());
    for(auto b : books) {
        int r = searchTable->rowCount();
        searchTable->insertRow(r);
        searchTable->setItem(r, 0, new QTableWidgetItem(QString::number(b->id)));
        searchTable->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(b->title)));
        searchTable->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(b->author)));
        searchTable->setItem(r, 3, new QTableWidgetItem(QString::number(b->year)));
    }
}

void MainWindow::saveAndExit() {
    bookList.saveToFile("books.txt");
    QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    bookList.saveToFile("books.txt");
    event->accept();
}
