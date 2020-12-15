#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QDebug>
#include <QFileDialog>
#include <QHeaderView>
#include <QPainter>
//#include <stdlib>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Button_menuopen_clicked();

    void on_Button_openlabel_clicked();

    void on_Button_saveviz_clicked();

    void on_treeWidget_files_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;

    void resizeEvent(QResizeEvent *e);

    void Add_Content_to_TreeWidget(QString px1, QString py1, QString px2, QString py2, QString ptype, QString pz1, QString pz2);
    void DrawRectangle(int ix1, int iy1, int ix2, int iy2, QColor color, int penwidth);
    void keepmax(int &max, int &number);
    void keepmin(int &min, int &number);
    void ReadLabelFile(QString labelfilename);

    QString currentlyOpenedDir;
    QPixmap *currentimage;

    void addTreeRoot(QFileInfo fileinfo);
    void addTreeChild(QTreeWidgetItem *parent, QFileInfo fileinfo);
    void addTreeSubChild(QTreeWidgetItem *parent, QFileInfo fileinfo);

};

#endif // MAINWINDOW_H
