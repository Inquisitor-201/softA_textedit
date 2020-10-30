#include "mainwindow.h"
#include "replacedialog.h"
#include "game2048.h"
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QScrollBar>
#include <cctype>
#include <QFileIconProvider>
#include <QProcess>
#include <QPushButton>
#include <QStatusBar>
#include <QLabel>
#include <QApplication>
#include <QVBoxLayout>

//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(640, 512);
    setWindowIcon(QIcon(":/resources/icons/window.jpg"));

    setupTextEdit();
    setupDialogs();
    createUI();
    createMenu();
    createToolBar();
}

void MainWindow::createMenu()
{
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);                   //新建菜单栏

    //!-----[filemenu]-----
    fileMenu = new QMenu(this);            //新建文件菜单

    ac_newfile = new QAction(QIcon(":/resources/icons/filenew.png"), "新建(&N)");
    ac_openfile = new QAction(QIcon(":/resources/icons/fileopen.png"), "打开文件(&O)");
    ac_open_project = new QAction(QIcon(":/resources/icons/openproject.png"), "打开项目");
    ac_savefile = new QAction(QIcon(":/resources/icons/filesave.png"), "保存(&S)");
    ac_save_as = new QAction("另存为(&A)...");
    ac_quit = new QAction(QIcon(":/resources/icons/fileclose.png"), "关闭(&Q)");

    connect(ac_newfile, &QAction::triggered, this, &MainWindow::newFile);
    connect(ac_openfile, &QAction::triggered, this, &MainWindow::openFile);
    connect(ac_open_project, &QAction::triggered, this, &MainWindow::openProject);
    connect(ac_savefile, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ac_save_as, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ac_quit, &QAction::triggered, this, &MainWindow::closeFile);

    ac_newfile->setShortcut(QKeySequence::New);
    ac_openfile->setShortcut(QKeySequence::Open);
    ac_savefile->setShortcut(QKeySequence::Save);
    ac_save_as->setShortcut(QKeySequence::SaveAs);
    ac_quit->setShortcut(QKeySequence::Quit);

    fileMenu->setTitle("文件(&F)");
    fileMenu->addAction(ac_newfile);
    fileMenu->addAction(ac_openfile);
    fileMenu->addAction(ac_open_project);
    fileMenu->addAction(ac_savefile);
    fileMenu->addAction(ac_save_as);
    fileMenu->addSeparator();
    fileMenu->addAction(ac_quit);

    menuBar->addMenu(fileMenu);
    //!-----[filemenu]-----

    //!-----[editmenu]-----
    editMenu = new QMenu(this);
    editMenu->setTitle("编辑(&E)");             //新建编辑菜单

    ac_undo = new QAction(QIcon(":/resources/icons/editundo.png"), "撤销(&U)");
    ac_redo = new QAction(QIcon(":/resources/icons/editredo.png"), "恢复(&R)");
    ac_find = new QAction(QIcon(":/resources/icons/editfind.png"), "查找(&F)...");
    ac_replace = new QAction(QIcon(":/resources/icons/editreplace.png"), "替换(&L)...");
    ac_cut = new QAction(QIcon(":/resources/icons/editcut.png"), "剪切(&T)");
    ac_copy = new QAction(QIcon(":/resources/icons/editcopy.png"), "复制(&C)");
    ac_paste = new QAction(QIcon(":/resources/icons/editpaste.png"), "粘贴(&P)");
    ac_select_all = new QAction("全选(&L)");

    connect(ac_undo, &QAction::triggered, textEdit, &customEdit::undo);
    connect(ac_redo, &QAction::triggered, textEdit, &customEdit::redo);
    connect(ac_find, &QAction::triggered, [=](){
        replace_dialog->changeIndex(0);
        replace_dialog->show();
        highlightFindContent();
    });
    connect(ac_replace, &QAction::triggered, [=](){
        replace_dialog->changeIndex(1);
        replace_dialog->show();
        highlightFindContent();
    });
    connect(ac_cut, &QAction::triggered, textEdit, &customEdit::cut);
    connect(ac_copy, &QAction::triggered, textEdit, &customEdit::copy);
    connect(ac_paste, &QAction::triggered, textEdit, &customEdit::paste);
    connect(ac_select_all, &QAction::triggered, textEdit, &customEdit::selectAll);

    ac_find->setShortcut(QKeySequence::Find);

    editMenu->addAction(ac_undo);
    editMenu->addAction(ac_redo);
    editMenu->addSeparator();
    editMenu->addAction(ac_cut);
    editMenu->addAction(ac_copy);
    editMenu->addAction(ac_paste);
    editMenu->addAction(ac_select_all);
    editMenu->addSeparator();
    editMenu->addAction(ac_find);
    editMenu->addAction(ac_replace);

    menuBar->addMenu(editMenu);
    //!-----[editmenu]-----

    //!-----[viewmenu]-----
    viewMenu = new QMenu(this);
    viewMenu->setTitle("视图(&V)");             //新建插件菜单

    ac_zoomin = new QAction(QIcon(":/resources/icons/zoomin.png"), "放大(&I)");
    ac_zoomout = new QAction(QIcon(":/resources/icons/zoomout.png"), "缩小(&O)");
    ac_show_file_tree = new QAction("显示文件树");
    ac_show_upper_widget = new QAction("显示已打开文件");
    ac_show_file_tree->setCheckable(true);
    ac_show_upper_widget->setCheckable(true);
    ac_show_file_tree->setChecked(true);
    ac_show_upper_widget->setChecked(true);

    connect(ac_zoomin, &QAction::triggered, [=](){
        globalFont->setPixelSize(globalFont->pixelSize() + 1);
        textEdit->setFont(*globalFont);
        //binEdit->resetFont();
        if (editArea->currentIndex() == 1)
            binEdit->viewport()->update();
    });

    connect(ac_zoomout, &QAction::triggered, [=](){
        if (globalFont->pixelSize() > 1)
            globalFont->setPixelSize(globalFont->pixelSize() - 1);
        textEdit->setFont(*globalFont);
        //binEdit->resetFont();
        if (editArea->currentIndex() == 1)
            binEdit->viewport()->update();
    });                                        //给二进制文本编辑器feed数据

    connect(ac_show_file_tree, &QAction::toggled, [=](){
        fileTree->setVisible(ac_show_file_tree->isChecked());
    });

    connect(ac_show_upper_widget, &QAction::toggled, [=](){
        upper_widget->setVisible(ac_show_upper_widget->isChecked());
    });

    viewMenu->addAction(ac_zoomin);
    viewMenu->addAction(ac_zoomout);
    viewMenu->addSeparator();
    viewMenu->addAction(ac_show_file_tree);
    viewMenu->addAction(ac_show_upper_widget);
    menuBar->addMenu(viewMenu);
    //!-----[viewmenu]-----

    //!-----[settingsmenu]-----
    settingsMenu = new QMenu(this);
    settingsMenu->setTitle("设置(&S)");             //新建设置菜单

    ac_editsettings = new QAction(QIcon(":/resources/icons/settings.png"), "编辑器外观设置...(&E)");

    connect(ac_editsettings, &QAction::triggered, [=](){
        settings_dialog->initialize();
    });                                      //给二进制文本编辑器feed数据

    settingsMenu->addAction(ac_editsettings);
    menuBar->addMenu(settingsMenu);
    //!-----[settingsmenu]-----

    //!-----[pluginmenu]-----
    pluginMenu = new QMenu(this);
    pluginMenu->setTitle("插件(&P)");             //新建插件菜单

    ac_binedit = new QAction(QIcon(":/resources/icons/binedit.png"), "二进制编辑器...(&B)");
    ac_binedit->setCheckable(true);
    ac_binedit->setChecked(false);               //二进制文本编辑器

    ac_game2048 = new QAction("GAME2048...");
    ac_char_video_viewer = new QAction(QIcon(":/resources/icons/videoviewer.png"), "字符动画...(&V)");

    ac_gamesnake = new QAction("GAMEsnake...");



    connect(ac_binedit, &QAction::toggled, [=](){
        if (ac_binedit->isChecked()) {
            binEdit->setTextString(textEdit->toPlainText());
            editArea->setCurrentIndex(1);
        } else
            editArea->setCurrentIndex(0);
    });

    connect(textEdit, &customEdit::textChanged, [=](){
        if (editArea->currentIndex() == 1)
            binEdit->setTextString(textEdit->toPlainText());
    });                                         //给二进制文本编辑器feed数据

    connect(ac_char_video_viewer, &QAction::triggered, [=](){
        QString filepath = QFileDialog::getOpenFileName(this, "打开视频文件", "./", "mp4 File (*.mp4)");
        if (filepath.isEmpty() || QFileInfo(filepath).suffix() != "mp4")
            return;
        charvideo_viewer->initialize(QUrl::fromLocalFile(filepath));
        editArea->setCurrentIndex(2);
    });

    connect(charvideo_viewer, &CharVideoViewer::videoTerminated, [=](){
        editArea->setCurrentIndex(0);
    });

    connect(ac_game2048, &QAction::triggered, [=](){
        qDebug("ok");
        GAME2048 w;
        w.setFixedSize(320,480);
        w.setWindowTitle("2048");
        w.exec();
    });

    connect(ac_gamesnake, &QAction::triggered, [=](){
        qDebug("ok");

        snake w;
        w.exec();

    });

    pluginMenu->addAction(ac_binedit);
    pluginMenu->addAction(ac_char_video_viewer);
    pluginMenu->addAction(ac_game2048);
    pluginMenu->addAction(ac_gamesnake);
    menuBar->addMenu(pluginMenu);
    //!-----[pluginmenu]-----
}

void MainWindow::createToolBar()
{
    QToolBar* tb = addToolBar(tr("File Actions"));
    tb->setMovable(false);
    tb->addAction(ac_newfile);
    tb->addAction(ac_openfile);
    tb->addAction(ac_open_project);
    tb->addAction(ac_savefile);
    tb->addAction(ac_quit);
    tb->addSeparator();
    tb->addAction(ac_undo);
    tb->addAction(ac_redo);
    tb->addAction(ac_cut);
    tb->addAction(ac_copy);
    tb->addAction(ac_paste);
    tb->addSeparator();
    tb->addAction(ac_find);
    tb->addAction(ac_replace);
    tb->addSeparator();
    tb->addAction(ac_zoomin);
    tb->addAction(ac_zoomout);
    tb->addSeparator();
    tb->addAction(ac_binedit);
    tb->addAction(ac_char_video_viewer);
}

void MainWindow::setupDialogs()
{
    replace_dialog = new replaceDialog(this);
    fileproperty_dialog = new FilePropertyDialog(this);
    settings_dialog = new SettingsDialog(this);
    settings_dialog->setEditor(textEdit);
    settings_dialog->setMainWindow(this);
}

void MainWindow::setupTextEdit()
{
    textEdit = new customEdit(this);
    textEdit->setMainWindow(this);
    binEdit = new BinaryEditor(this);
    binEdit->setRelatedTextEdit(textEdit);
}

void MainWindow::createUI()
{
    fileTree = new QTreeWidget(this);
    fileTree->setHeaderHidden(true);
    //TreeWidgetItem双击事件
    connect(fileTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(appendFileToEditor(QTreeWidgetItem*, int)));
    //TreeWidgetItem右键菜单
    fileTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(fileTree, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(popupContextMenu(const QPoint&)));

    QSplitter *HSplitter = new QSplitter(this);
    QVBoxLayout *rightArea_layout = new QVBoxLayout(this);  //设置水平和垂直布局器
    QWidget *rightArea = new QWidget(this);
    rightArea->setLayout(rightArea_layout);

    editArea = new QStackedWidget(this);
    charvideo_viewer = new CharVideoViewer(this);
    upper_widget = new UpperWidget(this);
    connect(upper_widget->comboBox(), SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_comboboxIndexChanged(int)));
    connect(upper_widget->buttonClose(), &QPushButton::clicked, [=](){
        closeFile();
    });

    editArea->addWidget(textEdit);
    editArea->addWidget(binEdit);
    editArea->addWidget(charvideo_viewer);

    rightArea_layout->setMargin(0);
    rightArea_layout->setSpacing(4);
    rightArea_layout->addWidget(upper_widget);
    rightArea_layout->addWidget(editArea);

    HSplitter->addWidget(fileTree);
    HSplitter->addWidget(rightArea);

    HSplitter->setStretchFactor(0, 1);
    HSplitter->setStretchFactor(1, 3);
    setCentralWidget(HSplitter);

    editArea->setCurrentIndex(0);
    stbar = new QStatusBar(this);
    stbar->showMessage("Ready");
    stbar->setSizeGripEnabled(false);
    setStatusBar(stbar);
}

void MainWindow::openInExplorer()
{
    if (selected_filetree_filename.isEmpty())
        return;
    QProcess process;

    selected_filetree_filename.replace("/", "\\");
    QString cmd = QString("explorer /select,\"%1\"").arg(selected_filetree_filename);
    process.startDetached(cmd);
}

void MainWindow::deleteChildren(QTreeWidgetItem* root)
{
    if (!root)
        return;
    int num_child = root->childCount();
    for (int i = 0; i < num_child; i++)
        deleteChildren(root->child(i));
    //qDebug() << "del:" << root->text(0);
    delete(root);
}

void MainWindow::createFileTreeWidget()
{
    QFileDialog fdialog(this);
    fdialog.setFileMode(QFileDialog::DirectoryOnly);
    if (!fdialog.exec()) {
        return;
    }

    QString folderName = fdialog.selectedFiles()[0];


    QFileInfo fi(folderName);
    QTreeWidgetItem* rootItem = new QTreeWidgetItem(fileTree);
    rootItem->setText(0, folderName);
    rootItem->setIcon(0, QFileIconProvider().icon(fi));
    rootItem->setData(0, Qt::UserRole, folderName);
    trace(folderName, rootItem);
}

void MainWindow::trace(QString path, QTreeWidgetItem* root)
{
    QDir dir(path);
    if(!dir.exists() || !dir.makeAbsolute())
    {
        return;
    }

    QFileInfoList list = dir.entryInfoList();
    for(QFileInfo info : list)
    {
        if(info.fileName() == "." || info.fileName() == "..")
        {
            continue;
        }
        if(info.isDir())
        {
            //如果是目录，则进行递归遍历
            //qDebug() << info.fileName();
            QTreeWidgetItem *item = new QTreeWidgetItem(root);
            item->setText(0, info.fileName());
            item->setIcon(0, QFileIconProvider().icon(info));
            item->setData(0, Qt::UserRole, info.filePath());
            trace(info.filePath(), item);
        }
        else
        {
            //普通文件，则直接输出
            QTreeWidgetItem *item = new QTreeWidgetItem(root);
            item->setText(0, info.fileName());
            item->setData(0, Qt::UserRole, info.filePath());
            item->setIcon(0, QFileIconProvider().icon(info));
        }
    }
}

void MainWindow::appendFileToEditor(QTreeWidgetItem* vertex, int column) //从左侧边栏打开文件，加入editor中
{
    QString filepath = vertex->data(column, Qt::UserRole).value<QString>();

    QFile file(filepath);
    QFileInfo fi(file);
    if (fi.isDir())
        return;
    int ret = file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    if (!ret) {
        qWarning("无法打开文件!");
        return;
    }

    QString content;
    if (textMap.find(filepath) == textMap.end()) {   //该文件没有被打开的情况
        QTextStream in(&file);
        content = in.readAll().toUtf8();
        textMap[filepath] = content;
        upper_widget->appendFile(filepath);
    } else {                    //该文件已经被打开的情况
        content = textMap[filepath];
    }
    setCurrentFileName(filepath);
    textEdit->setPlainText(content);
}

void MainWindow::on_comboboxIndexChanged(int index)
{
    QString str = upper_widget->comboBox()->itemData(index).value<QString>();
    if (textMap.find(str) == textMap.end()) return;
    textEdit->setPlainText(textMap[str]);
    setCurrentFileName(str);
}

void MainWindow::popupContextMenu(const QPoint& pos)
{
    FileTreeContextMenu = new QMenu(this);

    QAction* ac_showProperty = FileTreeContextMenu->addAction("显示文件属性...");
    QAction* ac_deleteFile = FileTreeContextMenu->addAction("从磁盘上删除文件...");
    FileTreeContextMenu->addSeparator();
    QAction* ac_openInExplorer =  FileTreeContextMenu->addAction("在Explorer中显示");

    ac_showProperty->deleteLater();
    ac_deleteFile->deleteLater();
    ac_openInExplorer->deleteLater();

    connect(ac_openInExplorer, &QAction::triggered, this, &MainWindow::openInExplorer);
    connect(ac_showProperty, &QAction::triggered, [=](){
        fileproperty_dialog->showProperty(QFileInfo(selected_filetree_filename));
    });
    connect(ac_deleteFile, &QAction::triggered, [=](){
        QMessageBox msgbox(this);
        msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgbox.setText(selected_filetree_filename + "将被删除。");
        msgbox.setInformativeText("确认删除？");
        msgbox.setDefaultButton(QMessageBox::No);
        int ret = msgbox.exec();

        if (ret == QMessageBox::Yes)
        {
            deleteChildren(selected_filetree_widget);
            QFileInfo fi(selected_filetree_filename);
            if (fi.isDir())
                QDir(selected_filetree_filename).removeRecursively();
            else
                QFile(selected_filetree_filename).remove();
        }
    });

    QTreeWidgetItem* item = fileTree->itemAt(pos);
    if (!item) return;
    selected_filetree_filename = item->data(0, Qt::UserRole).value<QString>();  //已选中的文件名
    selected_filetree_widget = item;                                            //已选中的项

    if (!item->parent()) {   //是top-level的item
        FileTreeContextMenu->addSeparator();
        QAction *ac_close_project = FileTreeContextMenu->addAction("关闭该项目");
        ac_close_project->deleteLater();
        connect(ac_close_project, &QAction::triggered, [=](){
            deleteChildren(item);
        });
    }
    FileTreeContextMenu->exec(cursor().pos());
    delete FileTreeContextMenu;
}

bool MainWindow::newFile()
{
    if (!querySave())
        return false;
    setCurrentFileName("");
    textEdit->clear();
    textEdit->document()->setModified(false);
    return true;
}

void MainWindow::openProject() {
    createFileTreeWidget();
}

bool MainWindow::openFile()          //打开文件
{
    if (!querySave())
        return false;
    QString filename = QFileDialog::getOpenFileName(this, "打开文件", "./", "All files (*.*);; text (*.txt)");
    if (filename.isEmpty())
        return false;                //选择要打开的文件，将其文件名存放在filename中

    QHash<QString, QString>::const_iterator i = textMap.find(filename);
    QString content;
    if (i == textMap.end()) {        //该文件没有被打开的情况
        QFile file(filename);
        if (!file.open(QIODevice::ReadWrite)){
            qWarning("无法打开文件！");
            return false;
        }
        content = QTextStream(&file).readAll().toUtf8();
        textMap[filename] = content;
        //openedFileList->addItem(QFileInfo(file).fileName());
    } else {                    //该文件已经被打开的情况
        content = textMap[filename];
    }
    setCurrentFileName(filename);
    textEdit->setPlainText(content);
    textEdit->document()->setModified(false);    //将文件内容输出到文本框中
    return true;
}

bool MainWindow::saveAs()
{
    QString save_filename;
    if (!getSaveFileName(&save_filename))
        return false;
    if (!saveAsFile(save_filename))
        return false;
    current_filename = save_filename;
    return true;
}

bool MainWindow::saveAsFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning("Failed to open the device.");
        return false;
    }
    QTextStream out(&file);
    out << textEdit->document()->toPlainText();
    textEdit->document()->setModified(false);
    return true;
}

bool MainWindow::getSaveFileName(QString* str)
{
    QFileDialog fdialog(this, "另存为..");
    fdialog.setFileMode(QFileDialog::AnyFile);
    fdialog.setNameFilter("text(*.txt)");
    fdialog.setAcceptMode(QFileDialog::AcceptSave);
    if (fdialog.exec() == QFileDialog::Reject) {
        qWarning("保存失败.");
        return false;
    }
    if (fdialog.selectedFiles().empty())
        return false;
    *str = fdialog.selectedFiles()[0];
    return true;
}

bool MainWindow::saveFile()
{
    if (!textEdit->document()->isModified())
        return true;
    if (current_filename.isEmpty())
    {
        if (!getSaveFileName(&current_filename))
            return false;
    }
    bool ret = saveAsFile(current_filename);
    return ret;
}

bool MainWindow::closeFile()
{
    if (!querySave())
        return false;
    if (!upper_widget->comboBox()->count())
        return false;
    QComboBox* cb = upper_widget->comboBox();
    int cur = cb->currentIndex();
    textMap.remove(cb->itemData(cur).value<QString>());
    upper_widget->comboBox()->removeItem(cur);

    if (upper_widget->comboBox()->count()){
        textEdit->setPlainText(textMap[cb->currentData().value<QString>()]);
        setCurrentFileName(cb->currentData().value<QString>());
    } else {
        textEdit->setPlainText("");
        setCurrentFileName("");
    }
    return true;
}

bool MainWindow::querySave()
{
    if (!textEdit->document()->isModified())
        return true;

    QMessageBox msgbox(this);
    msgbox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgbox.setText("文件 " + (current_filename.isEmpty() ? "(Untitled)" : current_filename) + " 已修改。");
    msgbox.setInformativeText("是否保存？");
    msgbox.setDefaultButton(QMessageBox::Save);
    int ret = msgbox.exec();

    switch (ret)
    {
        case QMessageBox::Save:
            saveFile();
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            return false;
    }
    return true;
}

void MainWindow::clearHighlightContent()
{
    textEdit->resetAppearance(); //清空已有高亮
}

void MainWindow::highlightFindContent()
{
    if (replace_dialog->isHidden())
        return;
    clearHighlightContent();

    QString pattern = replace_dialog->findContent();
    QRegularExpression re(pattern);

    if (replace_dialog->checkedCapital()) {
        re.setPattern(pattern);
    } else {
        QString extended_pattern;
        int pattern_len = pattern.length();
        for (int i = 0; i < pattern_len; i++) {
            if (isupper(pattern[i].toLatin1()))
                extended_pattern = extended_pattern + "[" + pattern[i] + tolower(pattern[i].toLatin1()) + "]";
            else if (islower(pattern[i].toLatin1()))
                extended_pattern = extended_pattern + "[" + pattern[i] + toupper(pattern[i].toLatin1()) + "]";
            else
                extended_pattern = extended_pattern + pattern[i];
        }
        re.setPattern(extended_pattern);
    }
    QTextCursor cs = textEdit->textCursor();

    QRegularExpressionMatchIterator i = re.globalMatch(textEdit->toPlainText());
    QTextCharFormat background_format;
    background_format.setBackground(QBrush(QColor(255, 239, 11)));
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        int startPos = match.capturedStart(), endPos = match.capturedEnd();
        cs.setPosition(startPos, QTextCursor::MoveAnchor);
        cs.setPosition(endPos, QTextCursor::KeepAnchor);
        cs.setCharFormat(background_format);
    }                                           //设置当前的高亮
}

void MainWindow::findNext() // 找下一个匹配字符串
{
    QString pattern = replace_dialog->findContent();
    QRegularExpression re(pattern);

    if (replace_dialog->checkedCapital()) {
        re.setPattern(pattern);
    } else {
        QString extended_pattern;
        int pattern_len = pattern.length();
        for (int i = 0; i < pattern_len; i++) {
            if (isupper(pattern[i].toLatin1()))
                extended_pattern = extended_pattern + "[" + pattern[i] + tolower(pattern[i].toLatin1()) + "]";
            else if (islower(pattern[i].toLatin1()))
                extended_pattern = extended_pattern + "[" + pattern[i] + toupper(pattern[i].toLatin1()) + "]";
            else
                extended_pattern = extended_pattern + pattern[i];
        }
        re.setPattern(extended_pattern);
    }

    QTextCursor cursor = textEdit->textCursor();
    QRegularExpressionMatch match = re.match(textEdit->toPlainText(), cursor.position());
    bool found = 0;

    if (match.capturedStart() >= 0) {
        found = 1;
    } else if (replace_dialog->checkedRepeat()) {
        match = re.match(textEdit->toPlainText());
        if (match.capturedStart() >= 0)
            found = 1;
    }

    if (found) {
        int startPos = match.capturedStart(), endPos = match.capturedEnd();
        cursor.setPosition(startPos, QTextCursor::MoveAnchor);
        cursor.setPosition(endPos, QTextCursor::KeepAnchor);
        textEdit->setTextCursor(cursor);
    } else {
        QMessageBox msgbox;
        msgbox.setWindowTitle("Hint");
        msgbox.setText("Cannot find \"" + pattern + "\"");
        msgbox.exec();
    }
}

void MainWindow::replace()
{
    QString pattern = replace_dialog->findContent();
    QRegularExpression re(pattern);

    if (replace_dialog->checkedCapital()) {
        re.setPattern(pattern);
    } else {
        QString extended_pattern;
        int pattern_len = pattern.length();
        for (int i = 0; i < pattern_len; i++) {
            if (isupper(pattern[i].toLatin1()))
                extended_pattern = extended_pattern + "[" + pattern[i] + tolower(pattern[i].toLatin1()) + "]";
            else if (islower(pattern[i].toLatin1()))
                extended_pattern = extended_pattern + "[" + pattern[i] + toupper(pattern[i].toLatin1()) + "]";
            else
                extended_pattern = extended_pattern + pattern[i];
        }
        re.setPattern(extended_pattern);
    }

    QTextCursor cursor = textEdit->textCursor();
    QRegularExpressionMatch match = re.match(textEdit->toPlainText(), cursor.position());
    bool found = 0;

    if (match.capturedStart() >= 0) {
        found = 1;
    } else if (replace_dialog->checkedRepeat()) {
        match = re.match(textEdit->toPlainText());
        if (match.capturedStart() >= 0)
            found = 1;
    }

    if (found) {
        int startPos = match.capturedStart(), endPos = match.capturedEnd();
        cursor.setPosition(startPos, QTextCursor::MoveAnchor);
        cursor.setPosition(endPos, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        cursor.insertText(replace_dialog->targetText());
        textEdit->setTextCursor(cursor);
    } else {
        QMessageBox msgbox;
        msgbox.setWindowTitle("Hint");
        msgbox.setText("Cannot find \"" + pattern + "\"");
        msgbox.exec();
    }
}

void MainWindow::replaceAll()
{
    QString pattern = replace_dialog->findContent();
    QRegularExpression re(pattern);

    if (replace_dialog->checkedCapital()) {
        re.setPattern(pattern);
    } else {
        QString extended_pattern;
        int pattern_len = pattern.length();
        for (int i = 0; i < pattern_len; i++) {
            if (isupper(pattern[i].toLatin1()))
                extended_pattern = extended_pattern + "[" + pattern[i] + tolower(pattern[i].toLatin1()) + "]";
            else if (islower(pattern[i].toLatin1()))
                extended_pattern = extended_pattern + "[" + pattern[i] + toupper(pattern[i].toLatin1()) + "]";
            else
                extended_pattern = extended_pattern + pattern[i];
        }
        re.setPattern(extended_pattern);
    }
    QTextCursor cs = textEdit->textCursor();

    QRegularExpressionMatch match = re.match(textEdit->toPlainText());
    int count = 0;

    while (match.capturedStart() >= 0) {
        int startPos = match.capturedStart(), endPos = match.capturedEnd();
        cs.setPosition(startPos, QTextCursor::MoveAnchor);
        cs.setPosition(endPos, QTextCursor::KeepAnchor);
        cs.removeSelectedText();
        cs.insertText(replace_dialog->targetText());
        count++;
        match = re.match(textEdit->toPlainText(), cs.position());//全部替换
    }

    QMessageBox msgbox;
    if (count > 0)
        msgbox.setText("已完成" + QString::number(count) + "处替换");
    else
        msgbox.setText(QString("无法替换，因为未找到\"%1\"").arg(replace_dialog->findContent()));
    msgbox.exec();
}

QTreeWidget* MainWindow::filetree(){
    return fileTree;
}

UpperWidget* MainWindow::upperwidget(){
    return upper_widget;
}

void MainWindow::setCurrentFileName(QString Filename)
{
    current_filename = Filename;
    QString fn = QFileInfo(Filename).fileName();
    QString fn_ = (fn.isEmpty() ? "" : fn + " @ TextEdit");
    setWindowTitle(fn_);
}

void MainWindow::zoomin()
{
    ac_zoomin->trigger();
}

void MainWindow::zoomout()
{
    ac_zoomout->trigger();
}

MainWindow::~MainWindow()
{
    //delete ui;
}
