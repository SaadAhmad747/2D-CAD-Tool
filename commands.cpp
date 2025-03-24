#include "commands.h"

/*********************** Add Shape Command Implementation ***********************/
AddShapeCommand::AddShapeCommand(QGraphicsScene *scene, QGraphicsItem *item)
    : scene(scene), item(item) {
    setText("Add Shape");
}

void AddShapeCommand::redo(){
    scene->addItem(item);
}

void AddShapeCommand::undo(){
    scene->removeItem(item);
}

/*********************** Delete Shape Command Implementation ***********************/
DeleteShapeCommand::DeleteShapeCommand(QGraphicsScene *scene, QGraphicsItem *item)
    :scene(scene), item(item){
    setText("Delete Shape");
}

void DeleteShapeCommand::redo(){
    scene->removeItem(item);
}

void DeleteShapeCommand::undo(){
    scene->addItem(item);
}

/*********************** Move Shape Command Implementation ***********************/
MoveShapeCommand::MoveShapeCommand(QGraphicsItem *item, const QPointF &oldPos, const QPointF &newPos)
    : item(item), oldPos(oldPos), newPos(newPos){
    setText("Move Shape");
}

void MoveShapeCommand::redo(){
    if(item) item->setPos(newPos);
}

void MoveShapeCommand::undo(){
    if(item) item->setPos(oldPos);
}

/*********************** Resize Shape Command Implementation ***********************/
ResizeShapeCommand::ResizeShapeCommand(QGraphicsItem *item, const QRectF &oldRect, const QRectF &newRect)
    : item(item), oldRect(oldRect), newRect(newRect){
    setText("Resize Shape");
}

void ResizeShapeCommand::redo(){
    if(auto *rect = dynamic_cast<QGraphicsRectItem*>(item)){
        rect->setRect(newRect);
    }
    else if(auto *ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)){
        ellipse->setRect(newRect);
    }
}

void ResizeShapeCommand::undo(){
    if(auto *rect = dynamic_cast<QGraphicsRectItem*>(item)){
        rect->setRect(oldRect);
    }
    else if(auto *ellipse = dynamic_cast<QGraphicsEllipseItem*>(item)){
        ellipse->setRect(oldRect);
    }
}
