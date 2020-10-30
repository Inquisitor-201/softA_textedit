#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>
#include <QColorDialog>
#include "mainwindow.h"

SettingsDialog::SettingsDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setFixedSize(size());
    ui->color_icon->setStyleSheet("border: 1px solid black");
    ui->bg_icon->setStyleSheet("border: 1px solid black");
    ui->selected_icon->setStyleSheet("border: 1px solid black");
    ui->editor_cur_icon->setStyleSheet("border: 1px solid black");
    ui->global_color_icon->setStyleSheet("border: 1px solid black");

    connect(ui->spinbox_fontsize, QOverload<int>::of(&QSpinBox::valueChanged), [=](int v){
        s_font.setPixelSize(v);
        ui->edit->setFont(s_font);
    });

    connect(ui->fontComboBox,
            QOverload<const QFont&>::of(&QFontComboBox::currentFontChanged),
            [=](const QFont& f) {
               s_font.setFamily(f.family());
               ui->edit->setFont(s_font);
    });

    connect(ui->c_bold, &QCheckBox::stateChanged, [=](){
        p_currentFormat->setFontWeight(ui->c_bold->isChecked() ? QFont::Bold : QFont::Normal);
        ui->edit->setCurrentCharFormat(*p_currentFormat);
        ui->edit->setPlainText(ui->edit->toPlainText());
       // qDebug() << ui->c_bold->isChecked();
    });

    connect(ui->c_italic, &QCheckBox::stateChanged, [=](){
        p_currentFormat->setFontItalic(ui->c_italic->isChecked());
        ui->edit->setCurrentCharFormat(*p_currentFormat);
        ui->edit->setPlainText(ui->edit->toPlainText());
    });

    connect(ui->c_underline, &QCheckBox::stateChanged, [=](){
        p_currentFormat->setFontUnderline(ui->c_underline->isChecked());
        ui->edit->setCurrentCharFormat(*p_currentFormat);
        ui->edit->setPlainText(ui->edit->toPlainText());
    });

    connect(ui->button_select_color, &QPushButton::clicked, [=](){
        QColor c = QColorDialog::getColor(p_currentFormat->foreground().color(), this, "Choose color...");
        if (!c.isValid())
            return;
        p_currentFormat->setForeground(QBrush(c));
        QPixmap pix(16, 16);
        pix.fill(c);
        ui->color_icon->setPixmap(pix);
        ui->edit->setCurrentCharFormat(*p_currentFormat);
        ui->edit->setPlainText(ui->edit->toPlainText());
    });

    connect(ui->button_global_color, &QPushButton::clicked, [=](){
        QColor c = QColorDialog::getColor(s_global_text_color, this, "Choose color...");
        if (!c.isValid())
            return;
        s_global_text_color = c;
        QPixmap pix(16, 16);
        pix.fill(c);
        ui->global_color_icon->setPixmap(pix);

        QTextCharFormat fmt;
        fmt.setForeground(QBrush(c));
        ui->edit->setCurrentCharFormat(fmt);
        ui->edit->mergeCurrentCharFormat(*p_currentFormat);
        ui->edit->setPlainText(ui->edit->toPlainText());
    });

    connect(ui->bg_button, &QPushButton::clicked, [=](){
        QColor c = QColorDialog::getColor(s_background_color, this, "Choose color...");
        if (!c.isValid())
            return;
        s_background_color = c;
        QPixmap pix(16, 16);
        pix.fill(c);
        ui->bg_icon->setPixmap(pix);
    });

    connect(ui->editor_cur_button, &QPushButton::clicked, [=](){
        QColor c = QColorDialog::getColor(s_current_line_color, this, "Choose color...");
        if (!c.isValid())
            return;
        s_current_line_color = c;
        QPixmap pix(16, 16);
        pix.fill(c);
        ui->editor_cur_icon->setPixmap(pix);
    });

    connect(ui->selected_button, &QPushButton::clicked, [=](){
        QColor c = QColorDialog::getColor(s_selected_bg_color, this, "Choose color...");
        if (!c.isValid())
            return;
        s_selected_bg_color = c;
        QPixmap pix(16, 16);
        pix.fill(c);
        ui->selected_icon->setPixmap(pix);
    });

    connect(ui->button_apply, &QPushButton::clicked, [=]()   //应用到全局变量，apply
    {
        keywordFormat =  s_keywordFormat;
        numberFormat = s_numberFormat;
        operatorFormat = s_operatorFormat;
        quotationFormat = s_quotationFormat;
        precomplingInstructionFormat = s_precomplingInstructionFormat;
        singleLineCommentFormat = s_singleLineCommentFormat;
        multiLineCommentFormat = s_multiLineCommentFormat;
        background_color = s_background_color;
        current_line_color = s_current_line_color;
        selected_bg_color = s_selected_bg_color;
        global_text_color = s_global_text_color;
        *globalFont = s_font;

        editor->highlighter()->resetRules();
        editor->resetAppearance();
        editor->highlightCurrentLine();

        auto cvt = [](QColor _rgb)->QString {
            QString redStr = QString("%1").arg(_rgb.red(),2,16,QChar('0'));
            QString greenStr = QString("%1").arg(_rgb.green(),2,16,QChar('0'));
            QString blueStr = QString("%1").arg(_rgb.blue(),2,16,QChar('0'));
            QString hexStr = "#" + redStr + greenStr+ blueStr;
            return hexStr;
        };
        main_window->filetree()->setStyleSheet("background-color:" + cvt(background_color)
                                               + "; color: " + cvt(global_text_color));
        main_window->upperwidget()->setStyleSheet("background-color:" + cvt(background_color));
        main_window->upperwidget()->resetBackground();
    });

    connect(ui->button_cancel, &QPushButton::clicked, [=](){
        close();
    });

    connect(ui->combobox_pattern, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        //qDebug() << "index=" << index;
        switch (index)
        {
            case Keyword:
                s_example = "Keyword";
                p_currentFormat = &s_keywordFormat;
                break;
            case Number:
                s_example = "0123456789";
                p_currentFormat = &s_numberFormat;
                break;
            case Operator:
                s_example = "+-*/()";
                p_currentFormat = &s_operatorFormat;
                break;
            case Quotation:
                s_example = "\"Example...\"";
                p_currentFormat = &s_quotationFormat;
                break;
            case PrecomplingInstructions:
                s_example = "#define false true";
                p_currentFormat = &s_precomplingInstructionFormat;
                break;
            case SingleLineComment:
                s_example = "//Comment here...";
                p_currentFormat = &s_singleLineCommentFormat;
                break;
            case MultiLineComment:
                s_example = "/* Comment here... */";
                p_currentFormat = &s_multiLineCommentFormat;
                break;
        };
        showCurrentFormat();
    });

    connect(ui->button_comfirm, &QPushButton::clicked, this, [=](){
        if (ui->combobox_theme->currentText() == "Default")
        {
            s_keywordFormat.setFontWeight(QFont::Bold);
            s_keywordFormat.setFontItalic(true);
            s_keywordFormat.setFontUnderline(false);
            s_keywordFormat.clearProperty(QTextFormat::ForegroundBrush);

            s_numberFormat.setFontWeight(QFont::Normal);
            s_numberFormat.setFontItalic(false);
            s_numberFormat.setFontUnderline(false);
            s_numberFormat.setForeground(QBrush(QColor(128, 0, 128)));

            s_operatorFormat.setFontWeight(QFont::Normal);
            s_operatorFormat.setFontItalic(false);
            s_operatorFormat.setFontUnderline(false);
            s_operatorFormat.setForeground(QBrush(QColor(255, 0, 0)));

            s_quotationFormat.setFontWeight(QFont::Normal);
            s_quotationFormat.setFontItalic(false);
            s_quotationFormat.setFontUnderline(false);
            s_quotationFormat.setForeground(QBrush(QColor(0, 0, 255)));

            s_precomplingInstructionFormat.setFontWeight(QFont::Normal);
            s_precomplingInstructionFormat.setFontItalic(false);
            s_precomplingInstructionFormat.setFontUnderline(false);
            s_precomplingInstructionFormat.setForeground(QBrush(QColor(0, 128, 0)));

            s_singleLineCommentFormat.setFontWeight(QFont::Normal);
            s_singleLineCommentFormat.setFontItalic(true);
            s_singleLineCommentFormat.setFontUnderline(false);
            s_singleLineCommentFormat.setForeground(QBrush(QColor(0, 120, 222)));

            s_multiLineCommentFormat = s_singleLineCommentFormat;

            s_global_text_color = QColor(0, 0, 0);
            s_current_line_color = QColor(204, 255, 255);
            s_background_color = QColor(255, 255, 255);
            s_selected_bg_color = QColor(0, 120, 215);
        } else
        if (ui->combobox_theme->currentText() == "Dark") {
            s_keywordFormat.setFontWeight(QFont::Bold);
            s_keywordFormat.setFontItalic(true);
            s_keywordFormat.setFontUnderline(false);
            s_keywordFormat.setForeground(QBrush(QColor(50, 120, 210)));

            s_numberFormat.setFontWeight(QFont::Normal);
            s_numberFormat.setFontItalic(false);
            s_numberFormat.setFontUnderline(false);
            s_numberFormat.setForeground(QBrush(QColor(181, 200, 140)));

            s_operatorFormat.setFontWeight(QFont::Normal);
            s_operatorFormat.setFontItalic(false);
            s_operatorFormat.setFontUnderline(false);
            s_operatorFormat.setForeground(QBrush(QColor(237, 90, 12)));

            s_quotationFormat.setFontWeight(QFont::Normal);
            s_quotationFormat.setFontItalic(false);
            s_quotationFormat.setFontUnderline(false);
            s_quotationFormat.setForeground(QBrush(QColor(206, 145, 120)));

            s_precomplingInstructionFormat.setFontWeight(QFont::Normal);
            s_precomplingInstructionFormat.setFontItalic(false);
            s_precomplingInstructionFormat.setFontUnderline(false);
            s_precomplingInstructionFormat.setForeground(QBrush(QColor(50, 120, 210)));

            s_singleLineCommentFormat.setFontWeight(QFont::Normal);
            s_singleLineCommentFormat.setFontItalic(true);
            s_singleLineCommentFormat.setFontUnderline(false);
            s_singleLineCommentFormat.setForeground(QBrush(QColor(100, 150, 75)));

            s_multiLineCommentFormat = s_singleLineCommentFormat;

            s_global_text_color = QColor(220, 220, 220);
            s_current_line_color = QColor(100, 100, 100);
            s_background_color = QColor(35, 35, 35);
            s_selected_bg_color = QColor(42, 42, 42);
        }
        showCurrentFormat();
        showCurrentBackground();
    });
}

void SettingsDialog::setEditor(customEdit* edit){
    editor = edit;
}

void SettingsDialog::setMainWindow(MainWindow* mw){
    main_window = mw;
}

void SettingsDialog::initialize()
{
    ui->fontComboBox->setCurrentFont(*globalFont);
    ui->spinbox_fontsize->setMinimum(1);
    ui->spinbox_fontsize->setValue(globalFont->pixelSize());

//s全部是settingsdialog内部的存储变量
//tab-1
    s_keywordFormat = keywordFormat;
    s_numberFormat = numberFormat;
    s_operatorFormat = operatorFormat;
    s_quotationFormat = quotationFormat;
    s_precomplingInstructionFormat = precomplingInstructionFormat;
    s_singleLineCommentFormat = singleLineCommentFormat;
    s_multiLineCommentFormat = multiLineCommentFormat;

    ui->combobox_pattern->setCurrentIndex(Keyword);
    p_currentFormat = &s_keywordFormat;
    s_example = "Keyword";
    s_font = *globalFont;
    showCurrentFormat();

//tab-2
    s_current_line_color = current_line_color;
    s_background_color = background_color;
    s_selected_bg_color = selected_bg_color;
    showCurrentBackground();

    exec();
}

void SettingsDialog::showCurrentFormat()
{
    ui->c_bold->setChecked(p_currentFormat->fontWeight() == QFont::Bold);
    ui->c_italic->setChecked(p_currentFormat->fontItalic());
    ui->c_underline->setChecked(p_currentFormat->underlineStyle() != QTextCharFormat::NoUnderline);

    QPixmap pix(16, 16);
    pix.fill(p_currentFormat->foreground().color());
    ui->color_icon->setPixmap(pix);
    pix.fill(s_global_text_color);
    ui->global_color_icon->setPixmap(pix);

    ui->edit->setCurrentCharFormat(*p_currentFormat);
    ui->edit->setFont(s_font);
    ui->edit->setPlainText(s_example);
}

void SettingsDialog::showCurrentBackground()
{
    QPixmap pix(16, 16);
    pix.fill(s_background_color);
    ui->bg_icon->setPixmap(pix);
    pix.fill(s_current_line_color);
    ui->editor_cur_icon->setPixmap(pix);
    pix.fill(s_selected_bg_color);
    ui->selected_icon->setPixmap(pix);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}


