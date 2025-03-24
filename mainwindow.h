#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QJsonArray>
#include "canvasview.h"
#include "Entity.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void modeChanged(DrawMode mode);

private slots:
    //mode slots
    void SetSelectMode();
    void SetLineMode();
    void SetRectangleMode();
    void SetCircleMode();
    void SetResizeMode();
    //I/O slots
    void OnSaveTriggered();
    void OnSaveAsTriggered();
    void OnOpenFileTriggered();

    void OnClearCanvasTriggered();

private:
    Ui::MainWindow *ui;
    CanvasView *canvasView;
    DrawMode currentMode;
    QString currentFilePath;

    void SaveToFile(const QString &filePath);

};
#endif // MAINWINDOW_H
