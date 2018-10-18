#ifndef CART_H
#define CART_H

#include "dbmanager.h"

class Transaction {
public:
    College college;
    souvenirItem itemPurchased;

    bool operator==(Transaction &other);

    Transaction(College college, souvenirItem itemPurchased);
    Transaction() {}
    ~Transaction() {}
};

class Cart
{
public:
    Cart() {}

    int size();
    void printCart();
    void addTransaction(College college, souvenirItem itemPurchased);
    void deleteTransaction(Transaction transaction);
    double getTotal();
    QVector<Transaction> getPurchases();

private:
    QVector<Transaction> purchases;
};

#endif // CART_H
