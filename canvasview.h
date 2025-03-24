#ifndef CANVASVIEW_H
#define CANVASVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QWheelEvent>
#include <QJsonArray>
#include <QUndoStack>
#include "commands.h"
#include "Entity.h"

class CanvasView : public QGraphicsView {
    Q_OBJECT

public:
    explicit CanvasView(QWidget *parent = nullptr);

    void SetDrawMode(DrawMode mode);
    void ClearCanvas();

    void Undo();
    void Redo();

    void DeserializeCanvas(const QJsonArray &shapes);
    QJsonArray SerializeCanvas() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event);

private:
    QGraphicsScene *scene;
    DrawMode currentMode;
    QPointF startPoint;
    QPointF originalPos;
    QPointF lastMousePos;
    QPointF lastPanPoint;
    QGraphicsItem *currentItem;
    QGraphicsItem *selectedItem;
    QRectF originalRect;
    QUndoStack *undoStack = nullptr;

    void DuplicateShape(QGraphicsItem *item);
    void DeleteShape(QGraphicsItem *item);
};

#endif // CANVASVIEW_H
