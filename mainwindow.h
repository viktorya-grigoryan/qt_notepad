#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMainWindow>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QTabWidget>
#include <QDockWidget>
#include <QDialog>
#include <QTableWidget>
#include <QEvent>
#include <QKeyEvent>
#include <QTreeView>
#include <QFileSystemModel>
#include <QStringListModel>
#include <QListView>
#include <QStyledItemDelegate>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QSyntaxHighlighter>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QStatusBar>
#include "editor.h"
#include "highlighter.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

    virtual void closeEvent(QCloseEvent *event) override;

private:

    QStatusBar *status;
    QMenu *MenuFile;
    QMenu *MenuEdit;
    QMenu *MenuView;
    QToolBar *MyToolBar;
    QTabWidget *MyTabWidget;
    QVector<Editor*> *vec_of_notes;
    QDockWidget *MyExplorer;
    QDockWidget *MyViewer;
    QStringList list_of_notes_names;
    QTreeView *FileExplorerView;

    void CreateActions();
    void CreateMenu();

    QAction *new_action;
    QAction *open_action;
    QAction *save_action;
    QAction *save_as_action;
    QAction *close_action;
    QAction *cut_action;
    QAction *copy_action;
    QAction *paste_action;
    QAction *clear_action;
    QAction *select_all_action;
    QAction *save_all_action;
    QAction *close_all_action;
    QAction *explorer_action;
    QAction *viewer_action;

    int doc_version = 0;
    QFileSystemModel *ExplorerModel;
    QStringListModel *ViewerModel;

public slots:

    void slot_new_file();
    void slot_open_file();
    void slot_open_file_exp( QModelIndex);
    void slot_save_file();
    void slot_save_as();
    void slot_save_all();
    void slot_close();
    void slot_close_all();
    void slot_exit(QCloseEvent*);
    void slot_cut();
    void slot_copy();
    void slot_paste();
    void slot_clear();
    void slot_select_all();
    void SlotForExplorer(bool);
    void SlotForViewer(bool);
    void slot_select_document(const QModelIndex&);
    void slot_note_was_update();
    void clicked_yes();
    void clicked_no();
    void statusbar_change();
};


class MayBeSave: public QDialog
{
    Q_OBJECT

public:

    MayBeSave(QWidget *parent=0);
    QTableWidget *question_table;
    QLabel *question_label;
    QPushButton *push_button_yes;
    QPushButton *push_button_no;
    QPushButton *push_button_cancel;
};

#endif // MAINWINDOW_H
