#ifndef ASOUVENIRS_H
#define ASOUVENIRS_H

#include <QWidget>
#include "QtSql"
#include "databasemanager.h"
#include "QtDebug"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <crtdbg.h>
using namespace std;

namespace Ui {
class aSouvenirs;
}

class aSouvenirs : public QWidget
{
    Q_OBJECT

public:

    explicit aSouvenirs(QWidget *parent = nullptr);
    //!< Constructor

    ~aSouvenirs();
    //!< Destructor

private slots:
    void on_addSouvenir_clicked();
    //!< Adds Souvenir to the Database

    void on_editSouvenir_clicked();
    //!< Updates Souvenir from Database

    void on_deleteSouvenir_clicked();
    //!< Deletes Souvenir from Database

    void on_comboBox_activated(const QString &arg1);
    //!< Updates listView

private:
    Ui::aSouvenirs *ui;/*!< class object */
    QSqlDatabase myDB;/*!< database connection */
};

//USED IN ADD SOUVENIR -> ASOUVENIRS
//!< Map class which uses templates in order to take two different input types
template <class S, class T>
class Map
{
private:

    //!< Struct used as a datatype for storing a key value pair. This allows you to use one array rather than multiple arrays
    struct SPair {
        S key; //!< Key variable
        T value; //!< Value vraibale
    };

    int initialSize = 0; /*!< Initial Array Size used for initialising the size of the array and the temporary array */
    int arrSize; /*!< Array size counter, this stores the map size by incrementing or decrementing once an element pair is added or removed */

    /*!< Array of struct type SPair for storing key value pair which is initialised to the initial map size */
    SPair * keyValArr = new SPair[initialSize];
   /*!< Temporary array of struct type SPair for temporarly storing key value pair which is initialised to the initial map size. This will be used when reiszing the array*/
    SPair * newPairArr = new SPair[initialSize];

    /*!< Function to resize the array. This a private function and will only be called once the set function has been called. This will increase the array size by 1. */
    void resizeArr();

public:
    //!< Default constructor  for the map which called when the map is created
    Map();
    //!< This destructor for the map will delete the map once the program finishes.
    ~Map();
    //!< This function will return the value by using the key passed by the user. This will search through map and return the value and if the key isn't found an exception is thrown
    T getValue(S keyInput);
    //!< Function to set the key and value pair. The user passes a key and value and then the resizeArr() function is called in order to resize the map and then place the pair in the map. If key already exists an exception is thrown
    void set(S k, T v);
    //!< Function to display the items in key value array
    void display();
    //!< Function deletes an key value pair element using the key. The user passes the key, the elements are besides the key wanted removed are copied over from original array into the temporary array
    //!< whilst the orginal array is resized down by 1 and then all the elements beside the chosen one are copied over back into reinitialised array. If key doesnt exists exception is thrown
    void deleteElement(S key);
    //!< The users passes a key and value. It searches for the key and if it find the current value is updated to the passed value. If key isn't found then an exception is thrown
    S update(S key, T value);
  //!< Returns the map size by returning the array size counter
    int MapSize();
    //!< This writes all the current key value pairs into a text file. The user passes the filename
    void WriteToFile(string file);
    //!< //This reads key value pairs from a text file and placed in a map. The user passes the filename
    void ReadFromFile(string file);

};

//Constructor
template <class S, class T>
Map<S, T>::Map()
{
    //Default constructor when the map is initilised.
    arrSize = 0; //Setting Array size counter to 0
}

template <class S, class T>
Map<S, T>::~Map()
{
    //This called when the program finishes in order to release unneeded memory
    delete[] keyValArr;
    delete[] newPairArr;
}

//Getting the maps size function
template <class S, class T>
int Map<S, T>::MapSize()
{
    return arrSize ; //Getting the size of the Map by return the array size
}

//Getting the values by passing the key function
template <class S, class T>
T Map<S, T>::getValue(S keyInput)
{

    bool foundKey = false; //Bool to check to if the key has been found

    //For loop to look through all the elements in the array
    for (int i = 0; i < arrSize; i++)
    {
        //Checking if input key matches the current key in the array
        if (keyInput == keyValArr[i].key)
        {
            return keyValArr[i].value; //Returns the value of that key
            foundKey = true; //Key has been found so set to true
        }
    }
    if (foundKey) //If it has been found
    {
        foundKey = false;
    }
    else
    {
//        throw NonExistingKeyException(); //If key hasn't been found an exception is thrown
    }

}

//Setting the key value pair by passing a key and value
template <class S, class T>
void Map<S, T>::set(S k, T v)
{
    bool keyExists = false; //Bool to check to if the key has been found

    //For loop to look through all the elements in the array
    for (int i = 0; i < arrSize; i++)
    {
        //Checking if input key matches the current key in the array
        if (k == keyValArr[i].key)
        {
            keyExists = true; //Key found has been set to true in order to know whether to throw an exception or not
        }
    }
    if (keyExists) //If it has been found
    {
        //keyExists = false;
//        throw DuplicateKeyException(); //Ducplicate exception thrown
    }
    else
    {
        resizeArr(); //Calling function to resize the array to increase the array size by
        keyValArr[arrSize - 1].key = k; //Setting the key at the top of current array size. Adding it onto the end of the increased array
        keyValArr[arrSize - 1].value = v; //Setting the value at the top of current array size. Adding it onto the end of the increased array
    }

}

//Display function
template <class S, class T>
void Map<S, T>::display()
{
    //Displating every key value pair in the array
    for (int i = 0; i < arrSize ; i++)
    {
        cout << "Element: " << i << " || Key: "  << keyValArr[i].key <<  "->" << " Value: ";
        cout << keyValArr[i].value << endl;
    }

}

//Dynamic resize function
template <class S, class T>
void Map<S, T>::resizeArr()
{
    //Loop through the old array size
    for (int i = 0; i < arrSize; i++)
    {
        newPairArr[i] = keyValArr[i]; //Items in the current array are copied into a new tempory array

    }

    arrSize++; //Increasing the array size counter
    delete[] keyValArr; //Deeleting the old array
    keyValArr = new SPair[arrSize]; //Reinitialising the array to the increased size using the counter. This requests and allocates memory that is large enough for the array size.

    //This is for loop the size of the current array size using the array size counter
    for (int i = 0; i < arrSize - 1; i++)
    {
        keyValArr[i] = newPairArr[i]; //This puts all the items in temporary back into the old array
    }
    delete[] newPairArr; // Temporary array is deleted
    newPairArr = new SPair[arrSize]; //This overrides and clears the current elements in the temporary array and sets the increased size

}

//Deleting a key value pair using the key
template <class S, class T>
void Map<S, T>::deleteElement(S key)
{
    bool isFound = false; //Checking if the key has been found
    int count = 0; //Loop counter
    int keyIndex = 0; //Stores the key index

    //For loop going through the array size counter in order to search through the array key element
    while (count < arrSize && !isFound)
    {
        //Checking if the input key matches a key stored in the array
        if (key == keyValArr[count].key)
        {
            isFound = true; //To show that the key has been found
            keyIndex = count; //Saving the key index where it is located
        }
        count++; // Increasing the loop counter
    }
    //If the key has been found
    if (isFound)
    {
        delete[] newPairArr; //Deleting the temporary array to release memory
        newPairArr = new SPair[arrSize - 1]; //Declaring the new array with a decreased size by than current map size
        int tempCount = 0; //Temporary counter due to the different array sizes between the orginal array and temperorary
        //Looping through the orginal array size
        for (int i = 0; i < arrSize; i++)
        {
            //Check if the current index counter isn't the same as the key index of the key to be removed. If does equal the key index then that element isn't copied over into the temp array.
            if (i != keyIndex)
            {
                newPairArr[tempCount] = keyValArr[i]; // Copying the current key value pair into the temporary array
                tempCount++; //Incremnting the the temporary counter
            }
        }
        arrSize--; //Decreasing the array size counter by one
        delete[] keyValArr; //Deleting the old array
        keyValArr = new SPair[arrSize]; //Reinitialising the old array with a decreased size (by 1)

        //Looping through the decreased array size
        for (int i = 0; i < arrSize; i++)
        {
            keyValArr[i] = newPairArr[i]; //Copying the items from the temp array into the map array
        }

        delete[] newPairArr; //Free memory by deleting the temp array

    }

}

template <class S, class T>
S Map<S, T>::update(S key, T value)
{
    bool keyFound = false; //Bool to check to if the key has been found
    //For loop to look through all the elements in the array
    for (int i = 0; i < arrSize; i++)
    {
        //Checking if input key matches the current key in the array
        if (key == keyValArr[i].key)
        {
            keyValArr[i].value = value; //Updating the value of the associated with the desired key with the input key
            return keyValArr[i].key; //Returns the current key
            keyFound = true; //Key found has been set to true in order to know whether to throw an exception or not
        }

    }
    if (keyFound) //If it has been found
    {
        keyFound = false; //Then set it to false for next check
    }

}

template <class S, class T>
void Map<S, T>::WriteToFile(string file)
{
    ofstream out(file.c_str());

    if (out.is_open())
    {
        //Looping through the array
        for (int i = 0; i < arrSize; i++)
        {
            //Writing the key value pair
            out << keyValArr[i].key << " " << keyValArr[i].value << endl;
        }
    }
    else
    {
        cout << "File unable to open";
    }

    out.close();
}

template <class S, class T>
void Map<S, T>::ReadFromFile(string file)
{
    ifstream in(file.c_str());
    S key; //Temporarly storing the key from the text file
    T value; //Temporarly storing the value from the text file
    while (!in.eof())
    {
            in >> key >> value; //Reading the key and inserting it into the temp holders

                set(key, value); //Setting values into map from the file

    }

}
#endif // ASOUVENIRS_H
