#include "cart.h"


//Transaction class functions

Transaction::Transaction(College college, souvenirItem itemPurchased)
{

    this->college = college;
    this->itemPurchased = itemPurchased;
}

bool Transaction::operator==(Transaction& other)
{
    return ((other.itemPurchased.name == this->itemPurchased.name) && (other.college.id == this->college.id));
}



//Cart functions

int Cart::size()
{
    return purchases.size();
}

void Cart::printCart()
{
    if (this->purchases.size())
    {
        int index= 0;

        College college = purchases[index].college;

        while(index < purchases.size())
        {
            if (purchases[index].college.id != college.id)
            {
                college = purchases[index].college;
            }
            index++;
        }
    }
}

void Cart::addTransaction(College college, souvenirItem itemPurchased)
{
    Transaction newTrans = Transaction(college, itemPurchased);
    purchases.push_back(newTrans);
}

void Cart::deleteTransaction(Transaction transaction)
{
    for (int index=0; index < purchases.size(); index++)
    {
        if (purchases[index] == transaction)
        {
            purchases.remove(index);
            break;
        }
    }
}

double Cart::getTotal()
{
    double total = 0;
    for (int index=0; index < purchases.size(); index++)
    {
        total += purchases[index].itemPurchased.price;
    }
    return total;
}

QVector<Transaction> Cart::getPurchases()
{
    QVector<Transaction> dummyVector;
    for(int i = 0; i < purchases.size(); i++)
    {
        dummyVector.append(purchases[i]);
    }
    return dummyVector;
}
