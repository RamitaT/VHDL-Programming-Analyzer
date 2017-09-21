#include "parserClasses.h"

//Return the number of tokens in tokenlist
int numOfTokens(const TokenList& tokens)
{
    Token *t = tokens.getFirst();
    int count(0);
    while(t)
    {
        count++;
        t =     t->getNext();
    }
    return count;
}


//Checking for the number of possible condition (number of "if", "elsif")
//Checking for the number of missing "then"
//Return the number of condition expression in tokenlist by subtracting the number of possible condition by the number of missing "then"
int numOfCondition(const TokenList& tokens)
{
    Token *t = tokens.getFirst();
    int num(0); //variable to hold the number of possible condition
    while(t)
    {
        //Case1: "if" is at the beginning of the tokenlist
        if(t->getStringRep() == "if" && t->getPrev() == nullptr)
        {
            num++;
            t = t->getNext();
        }
        //Case2: "if" token which is not "end if" statement
        else if(t->getStringRep() == "if" && t->getPrev()->getStringRep() != "end")
        {
            num++;
            t = t->getNext();
        }
        //Case3: "elsif" token
        else if(t->getStringRep() == "elsif")
        {
            num++;
            t = t->getNext();
        }
        else
            t = t->getNext();
    }
    return (num - numOfMissingThen(tokens));

}

//Checking for the number of "if" which is not "end if" statement
//Checking for the number of "end if"
//Return the number of missing "end if" by subtracting number of "if" and "end if"
int numOfMissingEndif(const TokenList& tokens)
{
    Token *t = tokens.getFirst();
    int numOfIf(0), numOfEnd(0);
    while(t)
    {
        //Case1: "if" is at the beginning of the tokenlist
        if(t->getStringRep() == "if" && t->getPrev() == nullptr)
        {
            numOfIf++;
            t = t->getNext();
        }
        //Case2: "if" token which is not "end if" statement
        else if(t->getStringRep() == "if" && t->getPrev()->getStringRep() != "end")
        {
            numOfIf++;
            t = t->getNext();
        }
        //Case3: "end if" statement
        else if(t->getStringRep() == "if" && t->getPrev()->getStringRep() == "end")
        {
            numOfEnd++;
            t = t->getNext();
        }
        else
            t = t->getNext();
    }
    return (numOfIf - numOfEnd);
}

//Checking the number of "if" which is not "end if" statement and the number of "elsif"
//Checking the number of "then"
//Return the number of missing "then" by adding number of "if" and "elsif", then subtracting with the number of "then"
int numOfMissingThen(const TokenList& tokens)
{
    Token *t = tokens.getFirst();
    int numOfIf(0), numOfElsif(0), numOfThen(0);
    while(t)
    {
        //Case1: "if" is at the beginning of the tokenlist
        if(t->getStringRep() == "if" && t->getPrev() == nullptr)
        {
            numOfIf++;
            t = t->getNext();
        }
        //Case2: "if" token which is not "end if" statement
        else if(t->getStringRep() == "if" && t->getPrev()->getStringRep() != "end")
        {
            numOfIf++;
            t = t->getNext();
        }
        //Case3: "end if" statement
        else if(t->getStringRep() == "elsif")
        {
            numOfElsif++;
            t = t->getNext();
        }
        //Case3: "then" statement
        else if(t->getStringRep() == "then")
        {
            numOfThen++;
            t = t->getNext();
        }
        else
            t = t->getNext();
    }
    return ((numOfIf+numOfElsif)-numOfThen);

}

//Return boolean value
//Check for type mismatch by taking two arguments which are token pointers
bool checkType(Token* t1, Token* t2)
{
    return ((t1->getTokenDetails()->type) == (t2->getTokenDetails()->type));
}

//Return boolean value
//Check for width mismatch by taking two arguments which are token pointers
bool checkWidth(Token* t1, Token* t2)
{
    return ((t1->getTokenDetails()->width) == (t2->getTokenDetails()->width));
}

//Return the number of type mismatch
int numOfType(const TokenList& tokens)
{
    Token *t = tokens.getFirst();
    int count(0);
    while(t)
    {
        if(t->getStringRep() == "=" || t->getStringRep() == "/=" || t->getStringRep() == "<" || t->getStringRep() == ">" || t->getStringRep() == "<=" || t->getStringRep() == ">=")
        {
            if(checkType(t->getPrev(),t->getNext()))
            {
                t = t->getNext()->getNext();
            }
            else
            {
                count++;
                t = t->getNext()->getNext();
            }
        }
        else
            t = t->getNext();
    }
    return count;
}

//Return the number of width mismatch
int numOfWidth(const TokenList& tokens)
{
    Token *t = tokens.getFirst();
    int count(0);
    while(t)
    {
        if(t->getStringRep() == "=" || t->getStringRep() == "/=" || t->getStringRep() == "<" || t->getStringRep() == ">" || t->getStringRep() == "<=" || t->getStringRep() == ">=")
        {
            if(checkType(t->getPrev(),t->getNext()))
            {
                if(checkWidth(t->getPrev(),t->getNext()))
                {
                    t = t->getNext()->getNext();
                }
                else
                {
                    count++;
                    t = t->getNext()->getNext();
                }

            }
            else
                t = t->getNext()->getNext();
        }
        else
            t = t->getNext();
    }
    return count;
}

//Return the number of "if" which is not a part of "end if"
int numOfIf(const TokenList& tokens)
{
    Token *t = tokens.getFirst();
    int numOfIf(0);
    while(t)
    {
        //Case1: "if" is at the beginning of the tokenlist
        if(t->getStringRep() == "if" && t->getPrev() == nullptr)
        {
            numOfIf++;
            t = t->getNext();
        }
        //Case2: "if" token which is not "end if" statement
        else if(t->getStringRep() == "if" && t->getPrev()->getStringRep() != "end")
        {
            numOfIf++;
            t = t->getNext();
        }
        else
            t = t->getNext();
    }
    return numOfIf;
}

