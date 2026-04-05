#include "text.h"

Text::Text()
    : QGraphicsTextItem("")
{}
Text::Text(qreal x, qreal y, QString s) : QGraphicsTextItem(s) {
    setPos(x, y);
}
Text::~Text() {};
