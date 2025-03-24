#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItem>

/*********************** Add Shape Command ***********************/
class AddShapeCommand : public QUndoCommand{
public:
    AddShapeCommand(QGraphicsScene *scene, QGraphicsItem *item);
    void redo() override;
    void undo() override;

private:
    QGraphicsScene *scene;
    QGraphicsItem *item;
};

/*********************** Delete Shape Command ***********************/
class DeleteShapeCommand : public QUndoCommand{
public:
    DeleteShapeCommand(QGraphicsScene *scene, QGraphicsItem *item);
    void redo() override;
    void undo() override;

private:
    QGraphicsScene *scene;
    QGraphicsItem *item;
};

/*********************** Move Shape Command ***********************/
class MoveShapeCommand : public QUndoCommand{
public:
    MoveShapeCommand(QGraphicsItem *item, const QPointF &oldPos, const QPointF &newPos);
    void redo() override;
    void undo() override;

private:
    QGraphicsItem *item;
    QPointF oldPos, newPos;
};

/*********************** Resize Shape Command ***********************/
class ResizeShapeCommand : public QUndoCommand{
public:
    ResizeShapeCommand(QGraphicsItem *item, const QRectF &oldRect, const QRectF &newRect);
    void redo() override;
    void undo() override;

private:
    QGraphicsItem *item;
    QRectF oldRect, newRect;
};

#endif // COMMANDS_H
