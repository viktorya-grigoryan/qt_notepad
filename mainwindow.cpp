#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    status = new QStatusBar(this);
    setStatusBar(status);
    setWindowIcon(QIcon(":/rsc/MainIcon.png"));

    MyTabWidget = new QTabWidget(this);
    MyTabWidget->setTabsClosable(true);
    MyTabWidget->setMovable(true);
    MyTabWidget->setDocumentMode(true);
    MyTabWidget->setUsesScrollButtons(true);
    MyTabWidget->setTabShape(QTabWidget::Triangular);
    connect(MyTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(slot_close()));
    vec_of_notes = new QVector<Editor*>;

    ExplorerModel = new QFileSystemModel(this);
    ExplorerModel->setRootPath(QDir::rootPath());
    FileExplorerView = new QTreeView(this);
    FileExplorerView->setModel(ExplorerModel);
    connect(FileExplorerView, SIGNAL(doubleClicked(QModelIndex)), SLOT(slot_open_file_exp(QModelIndex)));

    MyExplorer = new QDockWidget(tr("Проводник"),this);
    MyExplorer->setWidget(FileExplorerView);
    MyExplorer->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    MyExplorer->hide();
    addDockWidget(Qt::LeftDockWidgetArea, MyExplorer);

    MyViewer = new QDockWidget(tr("Обозреватель открытых документов"),this);
    MyViewer->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    MyViewer->hide();
    addDockWidget(Qt::RightDockWidgetArea, MyViewer);

    setWindowTitle(tr("My text editor"));
    CreateActions();
    CreateMenu();

    MenuView = new QMenu(tr("Вид"));
    explorer_action = MenuView->addAction(tr("Показать проводник"),this,SLOT(SlotForExplorer(bool)));
    explorer_action->setIcon(QIcon(":/rsc/Explorer_doc.png"));
    explorer_action->setCheckable(true);
    connect(explorer_action, SIGNAL(setChecked(bool)), this, SLOT(SlotForExplorer(bool)));
    connect(MyExplorer, SIGNAL(visibilityChanged(bool)), explorer_action, SLOT(setChecked(bool)));

    viewer_action = MenuView->addAction(tr("Показать обозреватель открытых документов"), this, SLOT(SlotForViewer(bool)));
    viewer_action->setIcon(QIcon(":/rsc/Viewer_doc.png"));
    viewer_action->setCheckable(true);
    connect(viewer_action, SIGNAL(setChecked(bool)), this, SLOT(SlotForViewer(bool)));
    connect(MyViewer, SIGNAL(visibilityChanged(bool)), viewer_action, SLOT(setChecked(bool)));
    menuBar()->addMenu(MenuView);

    setCentralWidget(MyTabWidget);
}

MainWindow::~MainWindow()
{
    vec_of_notes->clear();
    delete vec_of_notes;
}

void MainWindow::CreateActions()
{
    QToolBar *bar = addToolBar("Toolbar");
    new_action = new QAction(tr("&Новый"), this);
    new_action->setShortcut(QKeySequence::New);
    new_action->setIcon(QIcon(":/rsc/New_doc.png"));
    bar->addAction(new_action);
    connect(new_action, SIGNAL(triggered()), this, SLOT(slot_new_file()));

    open_action = new QAction(tr("&Открыть"), this);
    open_action->setShortcut(QKeySequence::Open);
    open_action->setIcon(QIcon(":/rsc/Open_doc.png"));
    bar->addAction(open_action);
    connect(open_action, SIGNAL(triggered()), this, SLOT(slot_open_file()));

    save_action = new QAction(tr("&Сохранить"), this);
    save_action->setShortcut(QKeySequence::Save);
    save_action->setIcon(QIcon(":/rsc/Save_doc.png"));
    bar->addAction(save_action);
    connect(save_action, SIGNAL(triggered()),this , SLOT(slot_save_file()));

    save_as_action = new QAction(tr("&Сохранить как..."), this);
    save_as_action->setShortcut(QKeySequence("Ctrl+Shift+S"));
    save_as_action->setIcon(QIcon(":/rsc/Save_as_doc.png"));
    bar->addAction(save_as_action);
    connect(save_as_action, SIGNAL(triggered()),this , SLOT(slot_save_as()));

    close_action = new QAction(tr("&Закрыть"), this);
    close_action->setShortcut(QKeySequence::Close);
    close_action->setIcon(QIcon(":/rsc/Close_doc.png"));
    connect(close_action, SIGNAL(triggered()),this , SLOT(slot_close()));

    cut_action = new QAction(tr("&Вырезать"), this);
    cut_action->setShortcut(QKeySequence::Cut);
    cut_action->setIcon(QIcon(":/rsc/Cut_doc.png"));
    connect(cut_action, SIGNAL(triggered()),this , SLOT(slot_cut()));

    copy_action = new QAction(tr("&Копировать"), this);
    copy_action->setShortcut(QKeySequence::Copy);
    copy_action->setIcon(QIcon(":/rsc/Copy_doc.png"));
    connect(copy_action, SIGNAL(triggered()),this , SLOT(slot_copy()));

    paste_action = new QAction(tr("&Вставить"), this);
    paste_action->setShortcut(QKeySequence::Paste);
    paste_action->setIcon(QIcon(":/rsc/Paste_doc.png"));
    connect(paste_action, SIGNAL(triggered()),this , SLOT(slot_paste()));

    clear_action = new QAction(tr("&Удалить"), this);
    clear_action->setShortcut(QKeySequence::Delete);
    clear_action->setIcon(QIcon(":/rsc/Clear_doc.png"));
    connect(clear_action, SIGNAL(triggered()),this , SLOT(slot_clear()));

    select_all_action = new QAction(tr("&Выделить всё"), this);
    select_all_action->setShortcut(QKeySequence::SelectAll);
    select_all_action->setIcon(QIcon(":/rsc/Select_all_doc.png"));
    connect(select_all_action, SIGNAL(triggered()),this , SLOT(slot_select_all()));

    save_all_action = new QAction(tr("&Сохранить всё"));
    save_all_action->setShortcut(QKeySequence("Ctrl+Shift+S"));
    save_all_action->setIcon(QIcon(":/rsc/Save_all_doc.png"));
    bar->addAction(save_all_action);
    connect(save_all_action, SIGNAL(triggered()),this , SLOT(slot_save_all()));

    close_all_action = new QAction(tr("Закрыть всё"));
    close_all_action->setShortcut(QKeySequence("Ctrl+Shift+W"));
    close_all_action->setIcon(QIcon(":/rsc/Close_all_doc.png"));
    connect(close_all_action, SIGNAL(triggered()),this , SLOT(slot_close_all()));
}

void MainWindow::CreateMenu()
{
    MenuFile = menuBar()->addMenu(tr("&Файл"));
    MenuFile->addAction(new_action);
    MenuFile->addAction(open_action);
    MenuFile->addAction(save_action);
    MenuFile->addAction(save_as_action);
    MenuFile->addAction(save_all_action);
    MenuFile->addAction(close_action);
    MenuFile->addAction(close_all_action);
    MenuFile->addSeparator();
    MenuFile->addAction("&Выход", qApp, SLOT(closeAllWindows()), QKeySequence("CTRL+Q"));

    MenuEdit = menuBar()->addMenu(tr("&Правка"));
    MenuEdit->addAction(cut_action);
    MenuEdit->addAction(copy_action);
    MenuEdit->addAction(paste_action);
    MenuEdit->addAction(clear_action);
    MenuEdit->addAction(select_all_action);
}


void MainWindow::slot_select_document(const QModelIndex& p_index)
{
    QString name = ViewerModel->data(p_index).toString();
    MyTabWidget->setCurrentIndex(list_of_notes_names.indexOf(name));
}

void::MainWindow::slot_note_was_update()
{
    if (!MyTabWidget->tabText(MyTabWidget->currentIndex()).contains("*"))
        MyTabWidget->setTabText(MyTabWidget->currentIndex(), "*" + MyTabWidget->tabText(MyTabWidget->currentIndex()));
    statusbar_change();
}

void MainWindow::slot_new_file()
{
    ++doc_version;
    Editor *new_note = new Editor(this);
    QString filename = "Документ " + QString::number(doc_version);
    auto index = MyTabWidget->addTab(new_note, filename);

    vec_of_notes->push_back(new_note);
    MyTabWidget->addTab(vec_of_notes->at(index), filename);
    MyTabWidget->setCurrentIndex(vec_of_notes->indexOf(new_note));
    connect(new_note, SIGNAL(textChanged()), SLOT(slot_note_was_update()));
    list_of_notes_names << MyTabWidget->tabText(MyTabWidget->currentIndex());
    SlotForViewer(MyViewer->isVisible());
}

void MainWindow::slot_open_file()
{
    QString file_path = QFileDialog::getOpenFileName(this, "Open file", "../Lab_3-4_notepad/");
    if (file_path != "")
    {
        QString file_name = file_path.section('/', -1);
        bool flag = true;
        int index=0;
        for (int i = 0; i < vec_of_notes->size(); ++i)
        {
            if (MyTabWidget->tabText(i) == file_name)
            {
                flag = false;
                index = i;
            }
        }
        if (flag)
        {
            QFile file(file_path);
            if ( !file.open(QFile::ReadOnly|QFile::Text))
            {
                QMessageBox::warning(this, tr("Ошибка, "), tr("невозможно открыть файл!"));
                return;
            }
            QTextStream TextStream(&file);
            Editor *new_note = new Editor(this);
           
            Highlighter *highlighter = new Highlighter(tr("cpp"), new_note->document());

            new_note->setPlainText(file.readAll());
            vec_of_notes->push_back(new_note);
            MyTabWidget->addTab(vec_of_notes->at(vec_of_notes->indexOf(new_note)), file_path.section('/', -1));
            MyTabWidget->setCurrentIndex(vec_of_notes->indexOf(new_note));
            connect(new_note, SIGNAL(textChanged()), SLOT(slot_note_was_update()));
            file.flush();
            file.close();
            list_of_notes_names << MyTabWidget->tabText(MyTabWidget->currentIndex());
            SlotForViewer(MyViewer->isVisible());
            status->showMessage(tr(QString("Opened file " + file_name).toStdString().c_str()), 5000);
        } else
        {
            MyTabWidget->setCurrentIndex(index);
        }
    }
}

void MainWindow::slot_save_file()
{
    QString filename = MyTabWidget->tabText(MyTabWidget->currentIndex()).section('*', -1);
    if (filename.contains("Документ "))
    {
        slot_save_as();
        return;
    }
    QFile File (filename);
    if (!File.open(QFile::WriteOnly|QFile::Text))
    {
        QMessageBox::warning(this, tr("Ошибка "), ("невозможно сохранить файл!"));
        return;
    }
    QTextStream TextStream(&File);
    TextStream << qobject_cast<Editor*>(MyTabWidget->widget(MyTabWidget->currentIndex()))->toPlainText();
    MyTabWidget->setTabText(MyTabWidget->currentIndex(), filename);
    list_of_notes_names.replace(MyTabWidget->currentIndex(), filename);
    SlotForViewer(MyViewer->isVisible());
    File.close();
}

void MainWindow::slot_save_as()
{
    QString FilePath = QFileDialog::getSaveFileName(this, tr("Сохранить как"), "", "*.txt");
    QFile File(FilePath);
    if (!File.open(QFile::WriteOnly|QFile::Text))
    {
        if (FilePath!="")
        {
            QMessageBox::warning(this, tr("Ошибка "), ("невозможно сохранить файл!"));
        }
        return;
    }
    QTextStream TextStream(&File);
    TextStream << qobject_cast<Editor*>(MyTabWidget->widget(MyTabWidget->currentIndex()))->toPlainText();
    MyTabWidget->setTabText(MyTabWidget->currentIndex(), FilePath.section('/', -1));
    list_of_notes_names.replace(MyTabWidget->currentIndex(), FilePath.section('/', -1));
    SlotForViewer(MyViewer->isVisible());
    File.close();
}

void MainWindow::slot_save_all()
{
    for (int i=0; i < vec_of_notes->size(); ++i)
    {
        MyTabWidget->setCurrentIndex(i);
        slot_save_file();
    }
}

void MainWindow::slot_close()
{
    int index = MyTabWidget->currentIndex();
    if (MyTabWidget->tabText(index).contains("*"))
    {
        int answer = QMessageBox::question(this, tr("Закрыть файл"), tr("Сохранить изменения в ") +
                                           MyTabWidget->tabText(index).section('*', -1) + "?", tr("Да"), tr("Нет"), tr("Отмена"));
        if (answer == 0)
        {
            MyTabWidget->setCurrentIndex(index);
            slot_save_file();
        } else if (answer == 2)
        {
            return;
        }
    }
    vec_of_notes->remove(index);
    MyTabWidget->removeTab(index);
    list_of_notes_names.removeAt(index);
    SlotForViewer(MyViewer->isVisible());
}


void MainWindow::slot_close_all()
{
    bool flag = false;
    for (int i = 0; i < vec_of_notes->size(); ++i)
    {
        if (MyTabWidget->tabText(i).contains("*"))
        {
            flag = true;
            MyTabWidget->setTabText(i, MyTabWidget->tabText(i).section("*", -1));
        }
    }
    if (flag)
    {
        int answer = QMessageBox::question(this, tr("Закрыть все"), tr("Сохранить изменения?"), tr("Да"), tr("Нет"), tr("Отмена"));
        if (answer == 0)
            slot_save_all();
        else if (answer == 2)
            return;
    }
    vec_of_notes->clear();
    MyTabWidget->clear();
    list_of_notes_names.clear();
    SlotForViewer(MyViewer->isVisible());
}


void MainWindow::slot_exit(QCloseEvent *event)
{
    MayBeSave *p_dialog = new MayBeSave(this);
    p_dialog->setWindowTitle(tr("Сохранить изменения?"));
    p_dialog->question_table->setColumnCount(2);
    p_dialog->setFixedSize(500, 400);
    p_dialog->question_table->setHorizontalHeaderLabels(QStringList() << tr("Файл") << tr("Путь"));
    p_dialog->question_table->setFixedSize(500, 300);
    p_dialog->question_table->horizontalHeader()->setStretchLastSection(true);
    p_dialog->question_table->verticalHeader()->hide();
    connect(p_dialog->push_button_yes, SIGNAL(clicked()), this, SLOT(clicked_yes()));
    connect(p_dialog->push_button_no, SIGNAL(clicked()), this, SLOT(clicked_no()));
    bool p_flag = false;
    int row = 0;
    QString path;
    for (int i = 0; i<vec_of_notes->size(); ++i)
    {
        if (MyTabWidget->tabText(i).contains("*"))
        {
            path ="../Lab_3-4_notepad/" + MyTabWidget->tabText(i).section("*",-1);
            QTableWidgetItem *item = new QTableWidgetItem(MyTabWidget->tabText(i).section("*",-1));
            QTableWidgetItem *itm = new QTableWidgetItem(path);
            p_dialog->question_table->insertRow(row);
            p_dialog->question_table->setItem(row, 0, item);
            p_dialog->question_table->setItem(row, 1, itm);
            p_flag = true;
            ++row;
        }
    }
    if (p_flag)
    {
        p_dialog->setModal(true);
        if (p_dialog->exec() == QDialog::Rejected)
        {
            delete p_dialog;
            event->ignore();
            return;
        }
        delete p_dialog;
        event->accept();
    }
}

void MainWindow::slot_cut()
{
    qobject_cast<Editor*>(MyTabWidget->currentWidget())->cut();
}

void MainWindow::slot_copy()
{
    qobject_cast<Editor*>(MyTabWidget->currentWidget())->copy();
}

void MainWindow::slot_paste()
{
    qobject_cast<Editor*>(MyTabWidget->currentWidget())->paste();
}

void MainWindow::slot_clear()
{
    qobject_cast<Editor*>(MyTabWidget->currentWidget())->clear();
}

void MainWindow::slot_select_all()
{
    qobject_cast<Editor*>(MyTabWidget->currentWidget())->selectAll();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    slot_exit(event);
}

void MainWindow::SlotForExplorer(bool p_flag)
{
    //!!! Память под виджеты нужно выделять при инициализации, а не каждый раз при вызове слота.
    //!!! Тут только hide или show

    if (p_flag)
        MyExplorer->show();
    else
        MyExplorer->hide();
}

void MainWindow::slot_open_file_exp(QModelIndex p_index)
{
    QModelIndex myind = p_index;
    if (!ExplorerModel->hasChildren(p_index))
    {
        QFile file(ExplorerModel->filePath(p_index));
        QString file_name = ExplorerModel->filePath(p_index).section('/', -1);
        bool p_flag = true;
        int index = 0;
        for (int i = 0; i < vec_of_notes->size(); ++i)
        {
            if (MyTabWidget->tabText(i) == file_name)
            {
                p_flag = false;
                index = i;
            }
        }
        if (p_flag)
        {
            if (!file.open(QFile::ReadOnly|QFile::Text))
            {
                QMessageBox::warning(this, tr("Ошибка, "), tr("невозможно открыть файл!"));
                return;
            }
            QTextStream TextStream(&file);
            Editor *new_note = new Editor(this);
            //QString extension = QFileInfo(file_name).suffix();
            //Highlighter *highlighter = new Highlighter (extension, new_note->document()); - должно быть так, но так глючит приложение, поэтому оставлю как было
            Highlighter *highlighter = new Highlighter(tr("cpp"), new_note->document());


            //!!! Где вывод текста в new_note и настройка расцветки, как в MainWindow::slot_open_file ????


            new_note->setPlainText(file.readAll());
            vec_of_notes->push_back(new_note);
            MyTabWidget->addTab(vec_of_notes->at(vec_of_notes->indexOf(new_note)), ExplorerModel->fileName(myind));
            MyTabWidget->setCurrentIndex(vec_of_notes->indexOf(new_note));
            connect(new_note, SIGNAL(textChanged()), SLOT(slot_note_was_update()));
            file.flush();
            file.close();
            list_of_notes_names << MyTabWidget->tabText(MyTabWidget->currentIndex());
            SlotForViewer(MyViewer->isVisible());
            status->showMessage(tr(QString("Opened file " + file_name).toStdString().c_str()), 5000);
        }
        else
            MyTabWidget->setCurrentIndex(index);
    }
}

void MainWindow::SlotForViewer(bool flag)
{
    ViewerModel = new QStringListModel(MyViewer);
    QListView *FileViewerView = new QListView(MyViewer);
    ViewerModel->setStringList(list_of_notes_names);
    FileViewerView->setModel(ViewerModel);
    FileViewerView->setEditTriggers(QAbstractItemView::NoEditTriggers); // добавила запрет на изменение имен открытых файлов
    MyViewer->setWidget(FileViewerView);
    status->showMessage(QString("Строка: 1 Столбец: 1"));
    connect(FileViewerView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slot_select_document(QModelIndex)));
    if (flag)
        MyViewer->show();
    else
        MyViewer->hide();
}

MayBeSave::MayBeSave(QWidget *parent)
{
    question_table = new QTableWidget;
    question_label = new QLabel(tr("Сохранить измененные документы?"));
    push_button_yes = new QPushButton(tr("Да"));
    push_button_no = new QPushButton(tr("Нет"));
    push_button_cancel = new QPushButton(tr("Отмена"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(question_label, 0, 0);
    layout->addWidget(question_table, 1, 0, 1, 3);
    layout->addWidget(push_button_yes, 2, 0);
    layout->addWidget(push_button_no, 2, 1);
    layout->addWidget(push_button_cancel, 2, 2);
    setLayout(layout);
    connect(push_button_yes, SIGNAL(clicked()), this, SLOT(accept()));
    connect(push_button_no, SIGNAL(clicked()), this, SLOT(accept()));
    connect(push_button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void MainWindow::clicked_yes()
{
    slot_save_all();
    vec_of_notes->clear();
    MyTabWidget->clear();
    list_of_notes_names.clear();
}

void MainWindow::clicked_no()
{
    vec_of_notes->clear();
    MyTabWidget->clear();
    list_of_notes_names.clear();
}

void MainWindow::statusbar_change() //задание на защиту
{
    Editor* cur=qobject_cast<Editor*>(MyTabWidget->currentWidget());
    status->showMessage(QString("Строка: %1 Столбец: %2 ").arg((cur->cursorRect().bottom()-18)/16+1).arg((cur->cursorRect().left()-3)/4+1));

}
