/*******************************************************************************
NAME: Peter Ramses Madakson.

PROGRAM DESCRIPTION:This Program reads Keirsey Personality Test data from a file
and processes the information. The program displays the category result for
candidates and saves the results in an output file.
*******************************************************************************/

/** PREPROCESSOR DIRECTIVES **/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/** GLOBAL DEFINITIONS **/

#define FILE_ERROR -1
#define SUCCESS 0
#define FAILURE -10
const int SIZE = 4;
const string LETTERS = "ESTJINFP";
const char X = 'X';

/** PERSONALITY STUCTURE: This is the Structure that stores Names, Answers, Letter Counts,
Percentages and the Results of each Test Candidate **/

struct Personality
{
    string name;
    string answers;
    int counts[SIZE * 2];
    int percentages[SIZE];
    char result[SIZE + 1];

};

/** FUNCTION PROTOTYPES **/

Personality * processfile(string, int&);
void processcounts(Personality *, int);
void calculatepercentages(Personality *, int);
void determinepersonality(Personality *, int);
void writetofile(string, const Personality *, int);
void displayoutput(const Personality *, int);

/** THE MAIN FUNCTION: This is where the main menu is displayed and all of the program
functions are ran**/

int main()
{
    //Main Menu Display
    cout << " ************************************************" << endl;
    cout << " *                                              *" << endl;
    cout << " *      TCES KEIRSEY TEST PROCESSING PROGRAM    *" << endl;
    cout << " *                                              *" << endl;
    cout << " ************************************************" << endl;
    cout << "                     WELCOME!" << endl;
    cout << "This Program reads Keirsey Personality Test data from " << endl;
    cout << " a file and processes the information. The program" << endl;
    cout << "displays the category result for candidates and saves" << endl;
    cout << "          the results in an output file. " << endl << endl;


    Personality * ptr;
    int size;
    ifstream file;
    char filename[100];
    char filename2[100];

    //Gets Data File name and Save/Output file name from user.
    cout << "Please Enter the Test Data File Name (Include '.txt'): ";
    cin >> filename;
    cout << "Please Enter the Test Results Save File Name (Include '.out'): ";
    cin >> filename2;
    cout << endl;

    //Function Calls
    ptr = processfile(filename, size);

    processcounts(ptr, size);

    calculatepercentages(ptr, size);

    determinepersonality(ptr, size);

    writetofile(filename2, ptr, size);

    displayoutput(ptr, size);



    //Returns Success or 0
    return SUCCESS;

}

/**PROCESS FILE FUNCTION: This is the function where the file containing data is opened and processed.
It returns the personalities.**/

Personality * processfile(string file, int& size)
{
    string line;
    ifstream datafile;
    Personality * ptr;
    int x;

    //Opening the Data File
    datafile.open(file.c_str());

    //Checks if there is a problem opening the data file. Displays message, Returns failure and NULL if problem exists.
    if (!datafile)
    {
        cout << "                  *** ERROR! ***" << endl;
        cout << "      \"" << file << "\" Can't Be Opened or Found!"<< endl;
        cout << endl << endl;
        exit(FAILURE);
        return NULL;

    }
    else
    {
        cout << "PERSONALITY TEST RESULTS (Saved to File Successfully): " << endl;
        cout << "-----------------------------------------------------" << endl << endl;

        //Gets the Size which is the first character in the file.
        datafile >> size;
        //Gets the first line in order to allow the name and answer lines accessible after.
        getline(datafile, line);

        //This creates a Personality struct using the size gotten from the first line in file.
        ptr = new Personality[size];

        //For loop gets all the Test candidates in the file. First getline gets Name and second gets answers.
        for (x = 0; x < size; x++)
        {
            getline(datafile, ptr[x].name);
            getline(datafile, ptr[x].answers);
        }

    }

    //Closes file and returns personalities.
    datafile.close();
    return ptr;
}

/** DISPLAY OUTPUT FUNCTION: This function display's the results and data for user
to see what the program calculated and what is saved to the Output file **/

void displayoutput(const Personality * ptr, int size)
{
    int counter = 0;
    int c, x;

    //For loop displays data for every single person in the file by using the candidates size.
    for (x = 0; x < size; x++)
    {

        cout << "(" << counter + 1 << ")";
        cout << " " << ptr[x].name << ": ";
        cout << "(A|B) ";

        //For loop displays the data and counts of each dimension.
        for (c = 0; c < 8; c++)
        {
            cout << ptr[x].counts[c];
            cout << " ";
        }
        //Counts Test candidates for display and also displays percentages for each candidate.
        counter++;
        cout << "(B%) ";
        cout << ptr[x].percentages[0];
        cout << ", " << ptr[x].percentages[1];
        cout << ", " << ptr[x].percentages[2];
        cout << ", " << ptr[x].percentages[3];

        cout << " -> RESULT: " << ptr[x].result << endl;

    }

    cout << endl;
}

/** CALCULATE PERCENTAGES FUNCTION: This function gets all the A/B counts and calculates the B percentages.
Stores calculations in the percentages array**/

void calculatepercentages(Personality * ptr, int size)
{
    int i, per_math;
    float cal_percent;

    //For loop performs the calculation for each test candidate using the size of all candidates.
    for (int x = 0; x < size; x++)
    {
        //For loop performs calculation for all 4 dimensions.
        for(i = 0; i < 4; i++)
        {

            cal_percent = (1 / ((float) ptr[x].counts[i+4] +(float) ptr[x].counts[i])) * ptr[x].counts[i+4] * 100;

            per_math = int(cal_percent + 0.5);
            //Stores percentage calculated values in the percentages array.
            ptr[x].percentages[i] = per_math;

        }
    }
}

/** THE DETERMINE PERSONALITY FUNCTION: This function determines the personality of each candidate using the calculated
percentages and stores the result in an array.**/

void determinepersonality(Personality * ptr, int size)
{
    int x, y;

    //For loop performs the personality determination for each candidate using the number of candidate 'size'.
    for (x = 0; x < size; x++)
    {
        //For loop performs the personality determination for each of 4 dimensions.
        for (y = 0; y < 4; y++)
        {

            if(ptr[x].percentages[y] > 50)
            {
                //Stores the resulting Letter from the global 'const string LETTERS' in the result array.
                ptr[x].result[y] = LETTERS[y+4];
            }
            else if(ptr[x].percentages[y] < 50)
            {
                //Stores the resulting Letter from the global 'const string LETTERS' in the result array.
                ptr[x].result[y] = LETTERS[y];
            }
            else
            {
                //Store X from the global 'char X' int the result array if candidate skipped a question.
                ptr[x].result[y] = X;
            }
        }

    }
}

/** THE PROCESS COUNTS FUNCTION: This is the function that processes and counts the test entries by candidates.
It counts the 'A's, 'B's and the '-'s and stores the counts in an array.**/

void processcounts(Personality * ptr, int size)
{
    int w, x, y1, y2, z;
    int store_answers[8];

    //Initializes every element in the Store-Answers array to zero.
    for (z = 0; z < 8; z++)
    {
        store_answers[z] = 0;
    }

    //For loop performs counts for all test candidates using the number 'size' of all candidates.
    for(x = 0; x < size; x++)
    {
        //Extrovert/Introvert For loop Counts 'A's and 'B's for all 70 answers in Increments of 7.
        for (y1 = 0; y1 < 70; y1 += 7)
        {
            //Checks for 'A' and 'a' and puts counts in the store-answers array.
            //0 and 4 are the places in the array for Extrovert/Introvert
            if(ptr[x].answers[y1] == 'a' || ptr[x].answers[y1] == 'A')
            {
                store_answers[0] = store_answers[0] + 1;
            }
            else if (ptr[x].answers[y1] == 'b' || ptr[x].answers[y1] == 'B')
            {
                store_answers[4] = store_answers[4] + 1;
            }
        }

        //For loop performs counts of the other 3 dimensions (Sensing/intuition, Thinking/Feeling, Judging/Perceiving)
        for(int h =1; h < 4; h++)
        {

            //For loop Counts 'A's and 'B's for all 70 answers.
            for (y2 = 2*h-1; y2 < 70; y2 += 6)
            {
                //h and h+4 stores answers in the appropriate places in the array
                //h/h+4(Sensing/intuition = 1/5, Thinking/Feeling = 2/6, Judging/Perceiving = 3/7)
                //y2 = 2*h-1 makes y2 1, 3, 5 for h<4
                if (ptr[x].answers[y2] == 'a' || ptr[x].answers[y2] == 'A')
                {
                    store_answers[h] = store_answers[h] + 1;
                }
                else if (ptr[x].answers[y2] == 'b' || ptr[x].answers[y2] == 'B')
                {
                    store_answers[h+4] = store_answers[h+4] + 1;
                }

                y2++;

                if (ptr[x].answers[y2] == 'a' || ptr[x].answers[y2] == 'A')
                {
                    store_answers[h] = store_answers[h] + 1;
                }
                else if (ptr[x].answers[y2] == 'b' || ptr[x].answers[y2] == 'B')
                {
                    store_answers[h+4] = store_answers[h+4] + 1;
                }
            }
        }

        //For loop performs the insertion of all 8 store-answers array elements into the counts array.
        //Also resets all 8 store_answers elements to zero.
        for (w = 0; w < 8; w++)
        {

            ptr[x].counts[w] = store_answers[w];
            store_answers[w] = 0;

        }

    }

}

/** THE WRITE TO FILE FUNCTION: This is the function where all the personality results and calculations
are stored in a file **/

void writetofile(string savedfile, const Personality * ptr, int size)
{
    ofstream file;
    int a, x, y;

    //Opening the Save File
    file.open(savedfile.c_str());

    //Checks if there is a problem opening the save file. Displays message and Returns failure if problem exists.
    if (!file)
    {
        cout << "                   *** ERROR! ***" << endl;
        cout << "   Can't Save Data to \"" << savedfile << "\""<< endl;
        cout << endl << endl;
        exit(FAILURE);

    }

    else
    {
        file << "A & B Answers" << endl;
        file << "-------------" << endl;

        //For loop saves the information for each candidate using the size of candidates.
        for (x = 0; x < size; x++)
        {
            file << ptr[x].name << ": ";

            //For loop displays the counts for all 8 values of dimensions.
            for (a = 0; a < 8; a++)
            {
                file << ptr[x].counts[a];
                file << " ";
            }

            file << endl;

        }
        file << endl;
        file << "RESULTS" << endl;
        file << "-------" << endl;

        //For loop saves the percentages for all candidates.
        for (y = 0; y < size; y++)
        {
            file << ptr[y].name << ": [";
            file << ptr[y].percentages[0];
            file << ", " << ptr[y].percentages[1];
            file << ", " << ptr[y].percentages[2];
            file << ", " << ptr[y].percentages[3];
            file << "] = " << ptr[y].result;
            file << endl;
        }

    }

    file.close();

}




