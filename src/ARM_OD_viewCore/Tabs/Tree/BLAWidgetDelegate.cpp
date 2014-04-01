#include "BLAWidgetDelegate.h"
#include <QtGui/QApplication>
#include <QDebug>
#include <QFile>

#include <QLabel>

#include <QCheckBox>
#include <QDir>
#include <QComboBox>
#include <QTextEdit>
#include <QSpinBox>


BLAWidgetDelegate::BLAWidgetDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    _cur_view(-1)
{
    _pm_round_red = new QPixmap(":/images/signals/images/bullet_red.png");
    _pm_round_green = new QPixmap(":/images/signals/images/bullet_green.png");

    QFile file("Styles/stylesheet.qcss");
    file.open(QFile::ReadOnly);
	if(file.isOpen()) {
        _styleSheet = QLatin1String(file.readAll());
        file.close();
    }

    _cb_items_filter.push_back("100");
    _cb_items_filter.push_back("300");
    _cb_items_filter.push_back("500");
    _cb_items_filter.push_back("800");
    _cb_items_filter.push_back("1000");
    _cb_items_filter.push_back("2000");
    _cb_items_filter.push_back("3000");
    _cb_items_filter.push_back("4000");
    _cb_items_filter.push_back("5000");
    _cb_items_filter.push_back("6000");
    _cb_items_filter.push_back("7000");
    _cb_items_filter.push_back("8000");
    _cb_items_filter.push_back("9000");
}

QWidget *BLAWidgetDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant vvar = index.model()->data(index, Qt::UserRole+1);
    if(_cur_view == 2)
    {
        QComboBox* editor = new QComboBox(parent);
        for(unsigned int i = 0; i < _cb_items_filter.size(); ++i)
        {
            editor->addItem(_cb_items_filter[i].toStdString().c_str());
        }
        return editor;
    }
    else
    {
        QSpinBox * editor = new QSpinBox(parent);
        if(_cur_view == 3)
            editor->setRange(-8000, 8000);
//        editor->setValue(22);
        return editor;
    }

    return NULL;
    //return editor;
}

void BLAWidgetDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(_cur_view == 2)
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = index.model()->data(index, Qt::UserRole+1).toString();
        int value = comboBox->findText(text);
        comboBox->setCurrentIndex(value);
        qDebug() << comboBox->currentText();
    }
    if(_cur_view == 1 || _cur_view == 3)
    {
        QSpinBox *sb = static_cast<QSpinBox*>(editor);
        int value = index.model()->data(index, Qt::EditRole).toInt();
        sb->setValue(value);
    }
}

void BLAWidgetDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(_cur_view == 2)
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::UserRole+1);
    }
    if(_cur_view == 1 || _cur_view == 3)
    {
        QSpinBox *sb = static_cast<QSpinBox*>(editor);
        model->setData(index, sb->value(), Qt::EditRole);
    }
}

void BLAWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    QVariant vvar = index.model()->data(index, Qt::UserRole+1);
    if(vvar == 68)
    {//        qDebug() << "USERROLE"  << vvar;
        //        QStyleOptionViewItemV4 myOption = option;
        //        if(!_cb_items.isEmpty())
        //        {
        //            qDebug() << index.row();
        //            QString text = _cb_items[index.row()].toStdString().c_str();

        //            qDebug() << text;
        //            myOption.text = text;
        //        }

//        QComboBox *editor = new QComboBox();
//        editor->addItem("LLL");
//        editor->setGeometry(option.rect);
//        editor->render(painter);
//        return;
    }

    QVariant var = index.model()->data(index, Qt::CheckStateRole);
    QString text = index.model()->data(index, Qt::DisplayRole).toString();

    if(!var.isNull())
    {
        bool checked = index.model()->data(index, Qt::CheckStateRole).toBool();
        QCheckBox renderer;
        renderer.setChecked(checked);
        renderer.setStyleSheet(_styleSheet);
        renderer.setText(text);
        renderer.resize(option.rect.size());
//        painter->save();
        painter->translate(option.rect.topLeft());
        renderer.render(painter);

//        painter->restore();
    }
    else{
        QLabel renderer;
        renderer.setStyleSheet(_styleSheet);
        renderer.setText(text);
        renderer.resize(option.rect.size());
//        painter->save();
        painter->translate(option.rect.topLeft());
        renderer.render(painter);
//        painter->restore();
    }

    painter->restore();


        //    QStyleOptionViewItemV4 opt = option;
        //    opt.state &= ~QStyle::State_On;

//    QStyleOptionButton check_box_style_option;
//    check_box_style_option.state |= QStyle::State_Enabled;
//    if (checked) {
//    check_box_style_option.state |= QStyle::State_On;
////    check_box_style_option.icon = QIcon(*_pm_round_red);
////    painter->save();
////    painter->drawPixmap(option.rect.x()-10, option.rect.y(), *_pm_round_green);
////    painter->restore();
//    } else {
////        painter->save();
////        painter->drawPixmap(option.rect.x(), option.rect.y(), *_pm_round_red);
////        painter->restore();
//    check_box_style_option.state |= QStyle::State_Off;
//    }
//    check_box_style_option.rect = option.rect;

//    check_box_style_option.text= text;

////    QApplication::style()->drawItemPixmap(painter, check_box_style_option.rect, 0, _pm_round_green);
////    QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter);

////    QApplication::style()->drawControl(QStyle::CE_CheckBoxLabel,
////                                     &check_box_style_option,
////                                     painter);

//    QApplication::style()->drawControl(QStyle::CE_CheckBox,
//                                     &check_box_style_option,
//                                     painter);

}

QSize BLAWidgetDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant value = index.model()->data(index, Qt::SizeHintRole);
    return qvariant_cast<QSize>(value);
}

bool BLAWidgetDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QModelIndex ind = index.sibling(index.row(), 0);
    QString name = ind.data(Qt::DisplayRole).toString();
    if(name == tr("Центр f (МГц)"))
        _cur_view = 1;
    else
    if(name == tr("Фильтр (кГц)"))
        _cur_view = 2;
    else
    if(name == tr("Сдвиг (кГц)"))
        _cur_view = 3;
    else
        _cur_view = 1;


    return false;
}
