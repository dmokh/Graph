#ifndef TEXT_H
#define TEXT_H
#include <QGraphicsTextItem>
class Text : public QGraphicsTextItem
{
public:
    Text();
    Text(qreal x, qreal y, QString s);
    ~Text();
};

#endif // TEXTH_H
