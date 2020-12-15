#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    //currentlyOpenedDir = "/home/nattaon/github/FloorNet/annotations/scan_1/annotation/floorplan.txt";
    currentlyOpenedDir = "/home/nattaon/github/FloorNet/annotations/";
    ui->lineEdit_filename->setText(currentlyOpenedDir);



    // Show header of treeWidget
    ui->treeWidget_content->setHeaderHidden(false);

    // Setup image area
    ui->imageLabel->setBackgroundRole(QPalette::Base);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLabel->setScaledContents(true);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(ui->imageLabel);
    ui->scrollArea->setVisible(true);
    ui->scrollArea->setWidgetResizable(false); //  if set true the image will very small, and no scrollbar



    // Set the number of columns in the tree
    //ui->treeWidget_files->setColumnCount(2);
    //if (currentlyOpenedDir.isEmpty())
    //    return;


}

MainWindow::~MainWindow()
{
    delete currentimage;
    delete ui;

}
void MainWindow::resizeEvent(QResizeEvent *e)
{
    QPoint p = ui->scrollArea->geometry().topLeft();
    int newwidth = e->size().width()-p.x()-30;
    int newheight = e->size().height()-p.y()-70;
    qDebug() << "new scollarea size " << newwidth << "*" << newheight;
    //ui->scrollArea->adjustSize();
    ui->scrollArea->resize(newwidth,newheight);

    ui->treeWidget_files->resize(ui->treeWidget_files->width(),newheight);
    ui->treeWidget_content->resize(ui->treeWidget_content->width(),newheight);

}
void MainWindow::on_Button_menuopen_clicked()
{
    QString currentlyOpenedDir = QFileDialog::getExistingDirectory(this, "Choose a directory to be read in");

    if (currentlyOpenedDir.isEmpty())
        return;

    ui->lineEdit_filename->setText(currentlyOpenedDir);
}

void MainWindow::on_Button_openlabel_clicked()
{
    // Show list of folder and sub files

    QDir directory(currentlyOpenedDir);
    QFileInfoList AllDirsfolders = directory.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Name);
    qDebug() << " total AllDirsfolders =  " << AllDirsfolders.size();
    foreach(QFileInfo folder , AllDirsfolders)
    {
        addTreeRoot(folder);
    }
}

void MainWindow::addTreeRoot(QFileInfo fileinfo)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget_files);

    treeItem->setText(0, fileinfo.fileName());
    treeItem->setText(1, fileinfo.absoluteFilePath()); // a hidden column for keeping the absolute path
    ui->treeWidget_files->expandItem(treeItem);

    //QDir subdirectory(currentlyOpenedDir + QDir::separator() + name);
    QDir subdirectory(fileinfo.absoluteFilePath());
    QFileInfoList subfolders = subdirectory.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Name);
    //qDebug() << " total AllDirsfolders =  " << AllDirsfolders.size();
    foreach(QFileInfo folder , subfolders)
    {
         addTreeChild(treeItem, folder);
    }

}

void MainWindow::addTreeChild(QTreeWidgetItem *parent, QFileInfo fileinfo)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(0, fileinfo.fileName());
    treeItem->setText(1, fileinfo.absoluteFilePath()); // a hidden column for keeping the absolute path

    parent->addChild(treeItem);
    ui->treeWidget_files->expandItem(treeItem);


    QDir subsubdir(fileinfo.absoluteFilePath());
    QFileInfoList subfilesinfo = subsubdir.entryInfoList(QDir::Files, QDir::Name);
    //QStringList subfiles = subsubdir.entryList(QDir::Files, QDir::Name);

   // qDebug() << "subfilesinfo " << subfilesinfo.size();
   // qDebug() << "subfiles " << subfiles.size();

    foreach(QFileInfo file , subfilesinfo)
    {
         addTreeSubChild(treeItem, file);
    }

}

void MainWindow::addTreeSubChild(QTreeWidgetItem *parent, QFileInfo fileinfo)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(0, fileinfo.fileName());
    treeItem->setText(1, fileinfo.absoluteFilePath()); // a hidden column for keeping the absolute path
    parent->addChild(treeItem);
    ui->treeWidget_files->expandItem(treeItem);
}



void MainWindow::ReadLabelFile(QString labelfilename)
{

    QFile currentlabeltxtfile;
    currentlabeltxtfile.setFileName(labelfilename);

    //QPixmap currentimage(800,800);
    //currentimage.fill( Qt::black );

    currentimage = new QPixmap(800,800);
    currentimage->fill(Qt::black);
    ui->imageLabel->setPixmap(*currentimage);
    ui->imageLabel->adjustSize();

    int max_x = 0;
    int max_y = 0;
    int min_x = 99999;
    int min_y = 99999;
    if ( currentlabeltxtfile.open(QIODevice::ReadOnly) ) //read file
    {
        QTextStream in(&currentlabeltxtfile);
        QString line = in.readLine(); // Read line by line
        QString x1,y1,x2,y2,type,z1,z2;
        int ix1,iy1,ix2,iy2;

        while (!line.isNull())
        {
            if(line.isEmpty()) return; // for last line = ""

            x1 = line.section('\t',0,0);
            y1 = line.section('\t',1,1);
            x2 = line.section('\t',2,2);
            y2 = line.section('\t',3,3);
            type = line.section('\t',4,4);
            z1 = line.section('\t',5,5);
            z2 = line.section('\t',6,6);
            //qDebug() << x1 << ","  << y1 << " : " << x2 << "," << y2;

            ix1=x1.toInt();
            iy1=y1.toInt();
            ix2=x2.toInt();
            iy2=y2.toInt();

            keepmax(max_x,ix1);
            keepmax(max_x,ix2);
            keepmax(max_y,iy1);
            keepmax(max_y,iy2);
            keepmin(min_x,ix1);
            keepmin(min_x,ix2);
            keepmin(min_y,iy1);
            keepmin(min_y,iy2);

            if(type=="wall") DrawRectangle(ix1,iy1,ix2,iy2,Qt::green,1);
            if(type=="window") DrawRectangle(ix1,iy1,ix2,iy2,Qt::blue,3);
            if(type=="door") DrawRectangle(ix1,iy1,ix2,iy2,Qt::red,3);

            Add_Content_to_TreeWidget(x1,y1,x2,y2,type,z1,z2);
            line = in.readLine();
        }

    }

    ui->treeWidget_content->header()->resizeSections(QHeaderView::ResizeToContents);

    qDebug() << "min_max_x : "  << min_x << "," << max_x;
    qDebug() << "min_max_y : "  << min_y << "," << max_y;
    //QRect rect(0, 0, max_x+10, max_y+10);
    *currentimage = currentimage->copy(0, 0, max_x+10, max_y+10);

    //QPixmap *pixmap = new QPixmap(min_x+max_x,min_y+max_y);
    //pixmap->fill(Qt::white);
    //ui->imageLabel->setPixmap(*pixmap);
    //ui->imageLabel->adjustSize();
    ui->imageLabel->setPixmap(*currentimage);
    ui->imageLabel->adjustSize();

}




void MainWindow::keepmax(int &max, int &number)
{
    if(number>max) max=number;
}
void MainWindow::keepmin(int &min, int &number)
{
    if(number<min) min=number;
}

void MainWindow::Add_Content_to_TreeWidget(QString px1, QString py1, QString px2, QString py2, QString ptype, QString pz1, QString pz2)
{
    QTreeWidgetItem *currentLine = new QTreeWidgetItem(ui->treeWidget_content);
    currentLine->setText(0, px1);
    currentLine->setText(1, py1);
    currentLine->setText(2, px2);
    currentLine->setText(3, py2);
    currentLine->setText(4, ptype);
    currentLine->setText(5, pz1);
    currentLine->setText(6, pz2);

    //currentLine->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    //currentLine->setTextAlignment(0, Qt::AlignLeft);
}

void MainWindow::DrawRectangle(int ix1, int iy1, int ix2, int iy2, QColor color, int penwidth)
{
    QPainter *painter = new QPainter(currentimage);
    QPen myPen(color, penwidth, Qt::SolidLine);
    painter->setPen(myPen);
    //int width = abs(ix2-ix1);
    //int height = abs(iy2-iy1);
    //painter->drawRect(ix1,iy1,width,height);
    painter->drawLine(ix1,iy1,ix2,iy2);
    delete painter;

    // # Draw image
    ui->imageLabel->setPixmap(*currentimage);

}

void MainWindow::on_Button_saveviz_clicked()
{
    QString imagefilename = ui->lineEdit_imagename->text();
    qDebug() << "imagefilename: " << imagefilename;
    if(imagefilename=="")
    {
        imagefilename = QFileDialog::getSaveFileName(this, tr("Save image file"),"0000.png", tr("Images (*.png)"));

        if (imagefilename.isEmpty())
            return;
    }
    qDebug() << "_imagefilename: " << imagefilename;

    currentimage->save(imagefilename,"PNG",100);
}

void MainWindow::on_treeWidget_files_itemClicked(QTreeWidgetItem *item, int column)
{
    QString itempath = item->text(1);
    qDebug() << "itempath: " << itempath;

    QString ext = itempath.section('.',-1,-1);
    //qDebug() << "ext: " << ext;


    QString ppfoldername =  item->parent()->parent()->text(0);
    qDebug() << "ppfoldername: " << ppfoldername;
    ui->lineEdit_imagename->setText(ppfoldername+".png");

    if(ext=="txt")
    {
        ReadLabelFile(itempath);

    }

    //QString labelfilename = ui->lineEdit_filename->text();
    //
}
