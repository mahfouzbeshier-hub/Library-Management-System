#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QLineEdit>
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "BookList.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Navigation
    void showPage(int index);
    void saveAndExit();

    // Actions
    void onAddBookClicked();
    void onDeleteBookClicked();
    void onUpdateBookClicked();
    void onSearchIdClicked();
    void onSearchTextClicked();

private:
    void setupUi();
    void applyStyles();
    
    QWidget* createSidebar();
    
    // Page Creators
    QWidget* createAddBookPage();
    QWidget* createDisplayAllPage();
    QWidget* createDeletePage();
    QWidget* createUpdatePage();
    QWidget* createSearchIdPage();
    QWidget* createSearchTextPage();
    QWidget* createCountPage();

    // Helpers
    void refreshBookList();
    
protected:
    void closeEvent(QCloseEvent *event) override;

    // Data
    BookList bookList;

    // UI Elements
    QStackedWidget* stackedWidget;
    
    // Add Page Inputs
    QLineEdit* idInput;
    QLineEdit* titleInput;
    QLineEdit* authorInput;
    QLineEdit* yearInput;
    
    // Display Page
    QTableWidget* bookTable;
    
    // Delete Page
    QLineEdit* deleteIdInput;
    
    // Update Page
    QLineEdit* updateIdInput;
    QLineEdit* updateTitleInput;
    QLineEdit* updateAuthorInput;
    QLineEdit* updateYearInput;
    QWidget* updateForm;
    
    // Search Pages
    QLineEdit* searchIdInput;
    QLabel* searchIdResult;
    QLineEdit* searchTextInput;
    QTableWidget* searchTable;
    
    // Count
    QLabel* countLabel;
};

#endif // MAINWINDOW_H
