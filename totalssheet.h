#ifndef TOTALSSHEET_H
#define TOTALSSHEET_H

#include <QDialog>
#include "cart.h"

namespace Ui {
class totalsSheet;
}

class totalsSheet : public QDialog
{
    Q_OBJECT

public:
    explicit totalsSheet(Cart cartInfo, QWidget *parent = 0);
    ~totalsSheet();

private:
    Ui::totalsSheet *ui;
    Cart cart;
};

#endif // TOTALSSHEET_H
