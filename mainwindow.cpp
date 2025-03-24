#include "mainwindow.h"
#include "ui_MainWindow.h"
#include "settingsmanager.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , canvasView(new CanvasView(this))
    , currentFilePath("")
{
    ui->setupUi(this);
    if(!ui->CanvasContainer->layout()){
        QVBoxLayout *layout = new QVBoxLayout(ui->CanvasContainer);
        ui->CanvasContainer->setLayout(layout);
    }
    ui->CanvasContainer->layout()->addWidget(canvasView);

    //Undo Redo keybinds and connection
    ui->actionRedo->setShortcut(QKeySequence("Ctrl+Y"));
    ui->actionUndo->setShortcut(QKeySequence("Ctrl+Z"));
    connect(ui->actionRedo, &QAction::triggered, canvasView, &CanvasView::Redo);
    connect(ui->actionUndo, &QAction::triggered, canvasView, &CanvasView::Undo);

    //Connect toolbar actions
    connect(ui->actionSelect, &QAction::triggered, this, &MainWindow::SetSelectMode);
    connect(ui->actionLine, &QAction::triggered, this, &MainWindow::SetLineMode);
    connect(ui->actionRectangle, &QAction::triggered, this, &MainWindow::SetRectangleMode);
    connect(ui->actionCircle, &QAction::triggered, this, &MainWindow::SetCircleMode);
    connect(ui->actionResize, &QAction::triggered, this, &MainWindow::SetResizeMode);
    connect(ui->actionClear_Canvas, &QAction::triggered, this, &MainWindow::OnClearCanvasTriggered);

    //ope, save & save as actions
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::OnOpenFileTriggered);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::OnSaveTriggered);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::OnSaveAsTriggered);

    connect(this, &MainWindow::modeChanged, canvasView, &CanvasView::SetDrawMode);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*********** CLEAR CANVAS ***********/
void MainWindow::OnClearCanvasTriggered(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Clear Canvas", "Are you sure you want to clear the canvas?",
                                  QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes){
        canvasView->ClearCanvas();
    }
}

/*********** SAVE ***********/
void MainWindow::OnSaveTriggered() {
    if(canvasView->SerializeCanvas().isEmpty()){
        QMessageBox::warning(this, "Warning", "Canvas is empty. Nothing to save.");
        return;
    }
    if (currentFilePath.isEmpty()) {
        currentFilePath = QFileDialog::getSaveFileName(this, "Save File", "", "CAD Files (*.json)");
    }

    if (!currentFilePath.isEmpty()) {
        SaveToFile(currentFilePath);
    }
}

/*********** SAVE AS ***********/
void MainWindow::OnSaveAsTriggered() {
    if(canvasView->SerializeCanvas().isEmpty()){
        QMessageBox::warning(this, "Warning", "Canvas is empty. Nothing to save.");
        return;
    }
    QString filePath = QFileDialog::getSaveFileName(this, "Save File As", "", "CAD Files (*.json)");

    if (!filePath.isEmpty()) {
        currentFilePath = filePath;
        SaveToFile(filePath);
    }
}

/*********** SAVE Method ***********/
void MainWindow::SaveToFile(const QString &filePath){
    QJsonArray shapesArray = canvasView->SerializeCanvas();

    if(SettingsManager::SaveToFile(filePath, shapesArray)){
        QMessageBox::information(this, "Success", "File saved successfully.");
    }
    else{
        QMessageBox::critical(this, "Error", "Failed to save file.");
    }
}

/*********** Open File ***********/
void MainWindow::OnOpenFileTriggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", "", "CAD Files (*.json)");

    if (!filePath.isEmpty()) {
        QJsonArray shapesArray;
        if(SettingsManager::LoadFromFile(filePath, shapesArray)){
            canvasView->DeserializeCanvas(shapesArray);
            currentFilePath = filePath;  // Set only if loading succeeds
            QMessageBox::information(this, "Success", "File loaded successfully.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to load file.");
        }
    }
}

/*********** MODE SELECTION ***********/
void MainWindow::SetSelectMode() { currentMode = DrawMode::Select; emit modeChanged(currentMode); }
void MainWindow::SetLineMode() { currentMode = DrawMode::Line; emit modeChanged(currentMode); }
void MainWindow::SetRectangleMode() { currentMode = DrawMode::Rectangle; emit modeChanged(currentMode); }
void MainWindow::SetCircleMode() { currentMode = DrawMode::Circle; emit modeChanged(currentMode); }
void MainWindow::SetResizeMode() { currentMode = DrawMode::Resize; emit modeChanged(currentMode); }
