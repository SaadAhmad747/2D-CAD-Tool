#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QMenu>
#include <QJsonObject>
#include <QJsonArray>
#include <QScrollBar>
#include "canvasview.h"

CanvasView::CanvasView(QWidget *parent)
    : QGraphicsView(parent)
    , scene(new QGraphicsScene(this))
    , currentMode(DrawMode::Select)
    , currentItem(nullptr)
    , selectedItem(nullptr)
    , undoStack(new QUndoStack(this))
{
    undoStack->setUndoLimit(20); //limit the commands
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setSceneRect(0, 0, 1000, 1000);
    setDragMode(QGraphicsView::NoDrag); // Default to no drag

}

/***********************Saving & Loading Canvas**********************/
QJsonArray CanvasView::SerializeCanvas() const
{
    QJsonArray shapesArray;

    for(QGraphicsItem *item : scene->items()){
        QJsonObject shapeObj;

        if(auto *line = dynamic_cast<QGraphicsLineItem *>(item)){
            shapeObj["type"] = "line";
            shapeObj["x1"] = line->line().p1().x();
            shapeObj["y1"] = line->line().p1().y();
            shapeObj["x2"] = line->line().p2().x();
            shapeObj["y2"] = line->line().p2().y();
        }
        else if (auto *rect = dynamic_cast<QGraphicsRectItem *>(item)) {
            shapeObj["type"] = "rectangle";
            shapeObj["x"] = rect->rect().x();
            shapeObj["y"] = rect->rect().y();
            shapeObj["width"] = rect->rect().width();
            shapeObj["height"] = rect->rect().height();
        }
        else if (auto *ellipse = dynamic_cast<QGraphicsEllipseItem *>(item)) {
            shapeObj["type"] = "circle";
            shapeObj["x"] = ellipse->rect().x();
            shapeObj["y"] = ellipse->rect().y();
            shapeObj["width"] = ellipse->rect().width();
            shapeObj["height"] = ellipse->rect().height();
        }

        if (!shapeObj.isEmpty()) {
            shapesArray.append(shapeObj);
        }
    }

    return shapesArray;
}

void CanvasView::DeserializeCanvas(const QJsonArray &shapesArray) {
    scene->clear(); // Clear existing shapes

    for (const QJsonValue &value : shapesArray) {
        QJsonObject obj = value.toObject();
        QString type = obj["type"].toString();

        if (type == "line") {
            int x1 = obj["x1"].toInt();
            int y1 = obj["y1"].toInt();
            int x2 = obj["x2"].toInt();
            int y2 = obj["y2"].toInt();

            QGraphicsLineItem* line = new QGraphicsLineItem(x1, y1, x2, y2);
            scene->addItem(line);
        }
        else if (type == "circle") {
            int x = obj["x"].toInt();
            int y = obj["y"].toInt();
            int width = obj["width"].toInt();
            int height = obj["height"].toInt();

            QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(x, y, width, height);
            scene->addItem(circle);
        }
        else if (type == "rectangle") {
            int x = obj["x"].toInt();
            int y = obj["y"].toInt();
            int width = obj["width"].toInt();
            int height = obj["height"].toInt();

            QGraphicsRectItem* rectangle = new QGraphicsRectItem(x, y, width, height);
            scene->addItem(rectangle);
        }
    }

    scene->update(); // Force refresh
}

/***********************Undo Redo**********************/
void CanvasView::Undo(){
    if(undoStack) undoStack->undo();
}

void CanvasView::Redo(){
    if(undoStack) undoStack->redo();
}

/***********************Actions**********************/
void CanvasView::SetDrawMode(DrawMode mode)
{
    currentMode = mode;
}

void CanvasView::ClearCanvas()
{
    undoStack->clear();
    scene->clear();
    selectedItem = nullptr;
    currentItem = nullptr;
    originalRect = QRectF();
}

void CanvasView::DuplicateShape(QGraphicsItem *item){
    if(!item) return;
    QGraphicsItem *newItem = nullptr;

    if(auto *line = dynamic_cast<QGraphicsLineItem *>(item)){
        QLineF originalLine = line->line();
        QLineF newLine(originalLine.p1()+QPointF(10, 10), originalLine.p2()+QPointF(10, 10));
        newItem = scene->addLine(newLine, QPen(Qt::black, 2));
    }
    else if(auto *rect = dynamic_cast<QGraphicsRectItem *>(item)){
        QRectF newRect = rect->rect().translated(10,10);
        newItem = scene->addRect(newRect, QPen(Qt::black, 2));
    }
    else if(auto ellipse = dynamic_cast<QGraphicsEllipseItem *>(item)){
        QRectF newellipse = ellipse->rect().translated(10,10);
        newItem = scene->addEllipse(newellipse, QPen(Qt::black, 2));
    }

    if(newItem){
        undoStack->push(new AddShapeCommand(scene, newItem));
    }
}

void CanvasView::DeleteShape(QGraphicsItem *item)
{
    if(item) undoStack->push(new DeleteShapeCommand(scene, item));
}

/***********************Mouse Events**********************/
void CanvasView::mousePressEvent(QMouseEvent *event)
{
    //Pan
    if (event->button() == Qt::MiddleButton) {
        lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }

    //Right Click
    if(event->button() == Qt::RightButton && currentMode == DrawMode::Select){
        startPoint = mapToScene(event->position().toPoint());
        selectedItem = scene->itemAt(startPoint, QTransform());
        if(selectedItem){
            QMenu ContextMenu;
            QAction *duplicateAction = ContextMenu.addAction("Duplicate");
            QAction *deleteAction = ContextMenu.addAction("Delete");

            QAction *selectedAction = ContextMenu.exec(event->globalPosition().toPoint());

            if(selectedAction == duplicateAction){
                DuplicateShape(selectedItem);
            }
            else if(selectedAction == deleteAction){
                DeleteShape(selectedItem);
            }
        }
    }

    //Left Click
    if(event->button() == Qt::LeftButton){
        startPoint = mapToScene(event->position().toPoint());

        setCursor(Qt::CrossCursor);
        if (currentMode == DrawMode::Select) {
            setCursor(Qt::SizeAllCursor);
            selectedItem = scene->itemAt(startPoint, QTransform());
            if (selectedItem) {
                originalPos = selectedItem->pos(); //stor exact pos for undo
                lastMousePos = startPoint; // Store initial position
            }
        }
        else if(currentMode == DrawMode::Resize){
            selectedItem = scene->itemAt(startPoint, QTransform());
            setCursor(Qt::SizeBDiagCursor);
            if(selectedItem){
                originalRect = selectedItem->boundingRect();
                lastMousePos = startPoint;
            }
        }
        else{
            selectedItem = nullptr;
        }

        switch(currentMode){
            case DrawMode::Line:
                currentItem = scene->addLine(QLineF(startPoint, startPoint), QPen(Qt::black, 2));
                break;
            case DrawMode::Rectangle:
                currentItem = scene->addRect(QRectF(startPoint, startPoint), QPen(Qt::black, 2));
                break;
            case DrawMode::Circle:
                currentItem = scene->addEllipse(QRectF(startPoint, startPoint), QPen(Qt::black, 2));
                break;
            default:
                break;
        }

    }
}

void CanvasView::mouseMoveEvent(QMouseEvent *event)
{
    //pan
    if (event->buttons() & Qt::MiddleButton) {
        QPointF delta = event->pos() - lastPanPoint;

        // Only update if there's actual movement
        if (!delta.isNull()) {
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
            lastPanPoint = event->pos();
        }
    }

    if(event->buttons() & Qt::LeftButton){
        QPointF newMousePos = mapToScene(event->position().toPoint());

        if (currentMode == DrawMode::Select && selectedItem) {
            QPointF delta = newMousePos - lastMousePos; // Movement difference
            selectedItem->setPos(selectedItem->pos() + delta); // Move selected item
            lastMousePos = newMousePos; // Update last position
        }
        else if (currentMode == DrawMode::Resize && selectedItem) {
            qreal scaleFactor = 1.0 + (newMousePos.x() - startPoint.x()) / 100.0; // Adjust scale factor

            if (scaleFactor < 0.1) scaleFactor = 0.1; // Prevent too small size

            QPointF center = originalRect.center();

            qreal newWidth = originalRect.width() * scaleFactor;
            qreal newHeight = originalRect.height() * scaleFactor;

            QRectF newRect(center.x() - newWidth / 2, center.y() - newHeight / 2, newWidth, newHeight);

            if (auto *rect = dynamic_cast<QGraphicsRectItem *>(selectedItem)) {
                rect->setRect(newRect);
            }
            else if (auto *ellipse = dynamic_cast<QGraphicsEllipseItem *>(selectedItem)) {
                ellipse->setRect(newRect);
            }
        }

        else if (currentItem) { // Only update if drawing
            if(auto *line = dynamic_cast<QGraphicsLineItem *>(currentItem)) {
                line->setLine(QLineF(startPoint, newMousePos));
            }
            else if(auto *rect = dynamic_cast<QGraphicsRectItem *>(currentItem)){
                rect->setRect(QRectF(startPoint, newMousePos).normalized());
            }
            else if(auto *ellipse = dynamic_cast<QGraphicsEllipseItem *>(currentItem)){
                ellipse->setRect(QRectF(startPoint, newMousePos).normalized());
            }
        }
    }
}

void CanvasView::mouseReleaseEvent(QMouseEvent *event)
{
    if(currentItem){
        undoStack->push(new AddShapeCommand(scene, currentItem));
        currentItem = nullptr;
    }
    else if(selectedItem && currentMode == DrawMode::Select){
        QPointF newPos = selectedItem->pos();
        if(newPos != originalPos){
            undoStack->push(new MoveShapeCommand(selectedItem, originalPos, newPos));
        }
    }
    else if(selectedItem && currentMode == DrawMode::Resize){
        QRectF newRect = selectedItem->boundingRect();
        if(newRect != originalRect){
            undoStack->push(new ResizeShapeCommand(selectedItem, originalRect, newRect));
        }
    }
    setCursor(Qt::ArrowCursor);
    selectedItem = nullptr;
}

void CanvasView::wheelEvent(QWheelEvent *event) //Zoom feature
{
    if(event->modifiers() & Qt::ControlModifier){
        double scaleFactor = 1.1;
        if(event->angleDelta().y() < 0)
            scaleFactor = 0.9;

        qreal newScale = transform().m11() * scaleFactor;
        if (newScale < 0.2 || newScale > 5.0) return;

        scale(scaleFactor, scaleFactor);
    } else{
        QGraphicsView::wheelEvent(event);
    }
}
