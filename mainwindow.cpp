#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QTableWidget>

//#include <exiv2/exiv2.hpp>
#include <libkexiv2/kexiv2.h>
// ----------- см. в .pro файле как подключать kexiv через pkgconfig.
// есть ещё сложность в том, что kexiv2 - из KDE4. И смешивать KDE4 с Qt5 довольно проблематично.

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>


#include <iostream>
#include <iomanip>
#include <cassert>

#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),    
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  
    model = new QStandardItemModel(0,0,this);
    ui->tableView->setModel(model);
    loadSettings();
   // connect(ui->tableWidget,SIGNAL(clicked()),this,SLOT(ph_analise())); // в итоге не получилось связал с помощью инструментов графического программирования
    connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(ph_analise_exiv()));
    ph_analise_exiv();
}



MainWindow::~MainWindow()
{
    saveSettings(); // Это лучше делать не в деструкторе - некоторые объекты могут быть уже удалены.
    // Лучше вызывать в обработчике QApplication::aboutToQuit или по onClose главной формы/объекта.
    delete ui;
}

/*void MainWindow::ph_analise_()  // не получилось с интерфейсом аля QT
{
    // ui->lineEdit->setText("Done");
        QLibrary library("/usr/lib/libexiv2.so.12");
        library.load();
        if (library.isLoaded())
        {
            qDebug()<<"library loaded";

            typedef void (*MyPrototype)();
            MyPrototype myFunction = (MyPrototype)library.resolve("KExiv2Iface.KExiv2");
            qDebug()<<myFunction;
            qDebug()<<library.errorString();
            if (myFunction)
            {
               qDebug()<<"function loaded";
               myFunction();
              // myFunction("/media/stub/projects/pd/1_point_detect/star_sky_fullframe/star_fullframe_p2/IMG_0116.jpg");
            }
        }
        if (!library.load())
        {
            qDebug()<<library.errorString();
         // ui->lineEdit->setText("library not loaded");
        }

//     классик doc QLibrary
//        QLibrary myLib("mylib");
//        typedef void (*MyPrototype)();
//        MyPrototype myFunction = (MyPrototype) myLib.resolve("mysymbol");
//        if (myFunction)
//            myFunction();
//      myLib - объект класса QLibrary         - будет переменная library
//      MyPrototype тип сслки на фуункцию      - MyPrototype
//      myFunction  имя функции
//
//

        // ---------------
        // Следующий пример загружает выше-созданную библиотеку, и использует одну из ее функций, для создания и отображения виджета.
    //    typedef QWidget*(*CreateWidgetFunction)(void);
    //    CreateWidgetFunction cwf = (CreateWidgetFunction)library.resolve("KExiv2Iface::KExiv2");
    //    if (cwf) {
    //     QWidget* wid = cwf();
    //     if (wid)
    //      wid->show();
    //    } else {
    //     qDebug() << "Could not show widget from the loaded library";
    //    }
        //library.resolve("KExiv2Iface::KExiv2");
    //QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row+1)*(column+1)));
}

inline bool exists_test1 (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}*/


/*void MainWindow::ph_analise_exiv()
{
    QString fname = ui->lineEdit->text();

    QFileInfo fi(fname);
    //if (exists_test1(fileName))     {
    if (fi.exists()) {
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(fname.toUtf8().constData());
        assert(image.get() != 0);
        image->readMetadata();

        Exiv2::ExifData &exifData = image->exifData();
        if (exifData.empty()) {
            qDebug()<<": No Exif data found in the file";
        }

        int ii=0;
        model->clear();
        model->setColumnCount(2);
        model->setRowCount(0);

        for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != exifData.end(); ++i) {

            QString keyPh = QString::fromUtf8(i->key().c_str());

            QString valuePh = QString::fromUtf8(i->value().toString().c_str());  // преобразование из типа value() в QString

            model->setRowCount(ii+1);
            QModelIndex index = model->index(ii,0,QModelIndex());
            model->setData(index,keyPh);

            QModelIndex index2 = model->index(ii,1,QModelIndex());
            model->setData(index2,valuePh);

            ii++;

            qDebug()   << valuePh;
            qDebug()   << keyPh;
            qDebug() << ii;
        }

        Exiv2::IptcData &iptcData = image->iptcData();
        if (iptcData.empty()) {
            qDebug()<<": No IPTC data found in the file";
        }
        for (Exiv2::IptcData::iterator md = iptcData.begin(); md != iptcData.end(); ++md) {
            QString keyPh = QString::fromUtf8(md->key().c_str());

            QString valuePh = QString::fromUtf8(md->value().toString().c_str());  // преобразование из типа value() в QString

            model->setRowCount(ii+1);
            QModelIndex index = model->index(ii,0,QModelIndex());
            model->setData(index,keyPh);

            QModelIndex index2 = model->index(ii,1,QModelIndex());
            model->setData(index2,valuePh);

            ii++;

            qDebug()   << valuePh;
            qDebug()   << keyPh;
            qDebug() << ii;

        }

        Exiv2::XmpData &xmpData=image->xmpData();
        if (xmpData.empty()) {
            qDebug()<<": No xmpData data found in the file";
        }
        for (Exiv2::XmpData::iterator mdd = xmpData.begin(); mdd != xmpData.end(); ++mdd) {
            QString keyPh = QString::fromUtf8(mdd->key().c_str());

            QString valuePh = QString::fromUtf8(mdd->value().toString().c_str());  // преобразование из типа value() в QString

            model->setRowCount(ii+1);
            QModelIndex index = model->index(ii,0,QModelIndex());
            model->setData(index,keyPh);

            QModelIndex index2 = model->index(ii,1,QModelIndex());
            model->setData(index2,valuePh);

            ii++;

            qDebug()   << valuePh;
            qDebug()   << keyPh;
            qDebug() << ii;

        }

        //  Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
        image->readMetadata();

        // Get a list of preview images available in the image. The list is sorted
        // by the preview image pixel size, starting with the smallest preview.
        Exiv2::PreviewManager loader(*image);
        Exiv2::PreviewPropertiesList list = loader.getPreviewProperties();

        // Some application logic to select one of the previews from the list
        Exiv2::PreviewPropertiesList::const_iterator pos = list.begin();

        // Get the selected preview image
        Exiv2::PreviewImage preview = loader.getPreviewImage(*pos);  // здесь может произойти вылет.*/

        /*
        // Write the preview image to a file
        std::string name = "preview" + preview.extension();
        std::cout << "Writing preview" << " "
                  << preview.width() << "x" << preview.height() << " "
                  << "to file" << " " << name << std::endl;
        preview.writeFile(name);
        QString nameQString=QString::fromUtf8(name.c_str());
        QImage imagePreview(nameQString);
        ui->imageLabel->setPixmap(QPixmap::fromImage(imagePreview));
        */
/*
        QByteArray ba;
        ba.setRawData((char*)preview.pData(),preview.size());
        QImage imagePreview;
        imagePreview.loadFromData(ba);
        if (!imagePreview.isNull())
            ui->imageLabel->setPixmap(QPixmap::fromImage(imagePreview));

        ui->tableView->resizeColumnsToContents();
    }
    else
    {*/
        /*QMessageBox msgBox;
        msgBox.setText("Файл "+ui->lineEdit->text()+" не существует!");
        msgBox.setInformativeText("Файл не существует!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok );
        msgBox.exec();*/
/*        QMessageBox::critical(this,tr("Ошибка"),
                              tr("Файл %1 не существует!").arg(ui->lineEdit->text()));
    }
}*/

// Вариант с kexiv2
void MainWindow::ph_analise_exiv()
{
    QString fname = ui->lineEdit->text();

    QFileInfo fi(fname);
    //if (exists_test1(fileName))     {
    if (fi.exists()) {
        KExiv2Iface::KExiv2 image;
        if (image.load(fname)) {
            int ii=0;
            model->clear();
            model->setColumnCount(2);
            model->setRowCount(0);

            if (!image.hasExif())
                qDebug()<<": No Exif data found in the file";
            else {
                qDebug() << "Exif tags";
                KExiv2Iface::KExiv2::MetaDataMap map = image.getExifTagsDataList();
                for (int i=0;i<map.keys().count();i++) {
                    QString keyPh = map.keys().at(i);
                    QString valuePh = map.value(keyPh);

                    model->setRowCount(ii+1);
                    QModelIndex index = model->index(ii,0,QModelIndex());
                    model->setData(index,keyPh);

                    QModelIndex index2 = model->index(ii,1,QModelIndex());
                    model->setData(index2,valuePh);

                    ii++;

                    qDebug()   << valuePh;
                    qDebug()   << keyPh;
                    qDebug() << ii;
                }
            }
            if (!image.hasIptc())
                qDebug()<<": No IPTC data found in the file";
            else {
                qDebug() << "IPTC tags";
                KExiv2Iface::KExiv2::MetaDataMap map = image.getIptcTagsDataList();
                for (int i=0;i<map.keys().count();i++) {
                    QString keyPh = map.keys().at(i);
                    QString valuePh = map.value(keyPh);

                    model->setRowCount(ii+1);
                    QModelIndex index = model->index(ii,0,QModelIndex());
                    model->setData(index,keyPh);

                    QModelIndex index2 = model->index(ii,1,QModelIndex());
                    model->setData(index2,valuePh);

                    ii++;

                    qDebug()   << valuePh;
                    qDebug()   << keyPh;
                    qDebug() << ii;
                }
            }
            if (!image.hasXmp())
                qDebug()<<": No XMP data found in the file";
            else {
                qDebug() << "XMP tags";
                KExiv2Iface::KExiv2::MetaDataMap map = image.getXmpTagsDataList();
                for (int i=0;i<map.keys().count();i++) {
                    QString keyPh = map.keys().at(i);
                    QString valuePh = map.value(keyPh);

                    model->setRowCount(ii+1);
                    QModelIndex index = model->index(ii,0,QModelIndex());
                    model->setData(index,keyPh);

                    QModelIndex index2 = model->index(ii,1,QModelIndex());
                    model->setData(index2,valuePh);

                    ii++;

                    qDebug()   << valuePh;
                    qDebug()   << keyPh;
                    qDebug() << ii;
                }
            }

            //QImage preview;
            //image.getImagePreview(preview);
            QImage preview = image.getExifThumbnail(true);
            if (preview.isNull())
                qDebug() << "Failed to get preview";
            else
                ui->imageLabel->setPixmap(QPixmap::fromImage(preview));

            ui->tableView->resizeColumnsToContents();
            ui->tableView->resizeRowsToContents();
        }
    }
    else
    {
        QMessageBox::critical(this,tr("Ошибка"),
                              tr("Файл %1 не существует!").arg(ui->lineEdit->text()));
    }
}


void MainWindow::ph_select()
{

    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open Image"), ui->lineEdit->text(), tr("Image Files (*.png *.jpg *.bmp *.CR2)"));
    if (fileName.length() > 1) {
    ui->lineEdit->setText(fileName);
      }
    else
    {
    QMessageBox msgBox;
    msgBox.setText("Файл не выбран");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok );
    msgBox.exec();
    }
    ph_analise_exiv();
}

void MainWindow::saveSettings()
{
    QSettings setting("BVitaly","ph_info");
    setting.beginGroup("MainWindow");
    setting.setValue("position",this->geometry());
    setting.endGroup();
    setting.beginGroup("fileName");
    setting.setValue("fileName",ui->lineEdit->text());
    setting.endGroup();
    setting.beginGroup("exivFilter");
    setting.setValue("PosFilter",ui->comboBox->currentIndex());
    setting.setValue("exivFilter",exivFilter);
    setting.endGroup();

    qDebug() << "Saved settings";
}

void MainWindow::loadSettings()
{
    QSettings setting("BVitaly","ph_info");
    setting.beginGroup("MainWindow");
    QRect myrect = setting.value("position").toRect();
    if (myrect.width()>1)
        setGeometry(myrect);
    setting.endGroup();
    setting.beginGroup("fileName");
    ui->lineEdit->setText(setting.value("fileName").toString());
    setting.endGroup();
    setting.beginGroup("exivFilter");
    setting.setValue("PosFilter",ui->comboBox->currentIndex());
    exivFilter =setting.value("exivFilter").toString();
    setting.endGroup();

    qDebug() << "Loaded settings";
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox::about(this,tr("Ошибка"),
                          tr("Редактор фильтров пока не готов"));

//    QMessageBox msgBox;
//    msgBox.setText("Редактор фильтров пока не готов");
//    msgBox.setStandardButtons(QMessageBox::Ok);
//    msgBox.setDefaultButton(QMessageBox::Ok );
//    msgBox.exec();
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QMessageBox::about(this,tr("Ошибка"),
                          tr("Фильтр пока не работает"));
//    QMessageBox msgBox;
//    msgBox.setText("Фильтр пока не работает");
//    msgBox.setStandardButtons(QMessageBox::Ok);
//    msgBox.setDefaultButton(QMessageBox::Ok );
//    msgBox.exec();
    ui->comboBox->setCurrentIndex(0);
}
