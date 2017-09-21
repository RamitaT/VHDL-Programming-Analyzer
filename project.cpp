//Use only the following three libraries:
#include "parserClasses.h"
#include <iostream>
#include <fstream>

using namespace std;

//Example Test code for interacting with your Token, TokenList, and Tokenizer classes
//Add your own code to further test the operation of your Token, TokenList, and Tokenizer classes
int main() {
	ifstream sourceFile;
	TokenList tokens;

  //Lists for types of tokens
  TokenList operatorTokens;
  TokenList identifierTokens;
  TokenList literalTokens;
  TokenList commentBodyTokens;
  TokenList otherTokens;

	Tokenizer tokenizer;

	//Read in a file line-by-line and tokenize each line
	sourceFile.open("vhdl test case.txt");                //don't for get to change the file name back
	if (!sourceFile.is_open()) {
		cout << "Failed to open file" << endl;
		return 1;
	}

	while(!sourceFile.eof()) {
		string line;
		getline(sourceFile, line);

		tokenizer.setString(&line);
		while(!tokenizer.isComplete()) {
			tokens.append(tokenizer.getNextToken());
		}
	}


	/*Test your tokenization of the file by traversing the tokens list and printing out the tokens*/

	//int n = removeComments(tokens);
	//cout << n << endl;
	Token *t = tokens.getFirst();
	while(t) {
		//cout << t->getStringRep() << " ";
        tokens.findAndSetTokenDetails(t);
        cout << t->getStringRep() << " ";
		//cout << t->isKeyword() << " ";
		cout << t->getTokenType() << " ";
        tokenDetails* temp = t->getTokenDetails();
		if(temp != nullptr)
        {
            //cout << temp;
            //cout << "in" << " ";
            cout << temp->type << " " << temp->width;
            cout << endl;
            //delete temp;
        }
        /*else*/
            //cout << "out" << " ";
        cout << endl;

		t = t->getNext();
	}

	/*int n = removeTokensOfType(tokens,T_Identifier);
	cout << n << endl;

    t = tokens.getFirst();
	while(t) {
		cout << t->getStringRep() << endl;
		t = t->getNext();
	}*/

    TokenList* t_result = findAllConditionalExpressions(tokens);
    t = t_result->getFirst();
    while(t)
    {
        cout << t->getStringRep() << " ";
        t = t->getNext();
    }

  /* For your testing purposes only */

  /* Ensure that tokens have all type information set*/

  /* Create operator,identifier,literal, etc. tokenLists from the master list of tokens */
    t = tokens.getFirst();
    while(t)
    {
        if(t->getTokenType() == T_Operator)
        {
            Token *new_token = new Token(*t);
            operatorTokens.append(new_token);
        }
        else if(t->getTokenType() == T_Identifier)
        {
            Token *new_token = new Token(*t);
            identifierTokens.append(new_token);
        }
        else if(t->getTokenType() == T_Literal)
        {
            Token *new_token = new Token(*t);
            literalTokens.append(new_token);
        }
        else if(t->getTokenType() == T_CommentBody)
        {
            Token *new_token = new Token(*t);
            commentBodyTokens.append(new_token);
        }
        else if(t->getTokenType() == T_Other)
        {
            Token *new_token = new Token(*t);
            otherTokens.append(new_token);
        }
        t = t->getNext();
    }
    cout << endl;
    Token *t1 = operatorTokens.getFirst();
    while(t1)
    {
        cout << t1->getStringRep() << " " << t1->getTokenType() << " ";
        if(t1->getTokenDetails() != nullptr)
        {
            cout << t1->getTokenDetails()->type << " " << t1->getTokenDetails()->width;
        }
        cout << endl;
        t1 = t1->getNext();
    }


  /*PART 3 ERROR CHECKING*/
    string mode;
    cout << "Select the mode(verbose/non-verbose): ";
    cin >> mode;
    cout << endl;
    cout << "The number of tokens: " << numOfTokens(tokens) << endl;
    cout << "The number of conditional expressions: " << numOfCondition(tokens) << endl;
    cout << "The number of missing \"end if\"s: " << numOfMissingEndif(tokens) << endl;
    cout << "The number of missing \"then\"s: " << numOfMissingThen(tokens) << endl;
    cout << "The number of type mismatches: " << numOfType(tokens) << endl;
    cout << "The number of width mismatches: " << numOfWidth(tokens) << endl;
    if(mode == "verbose")
    {
        //creat new tokenlist that ass the newline character as a signal of ending each line in vhdl file
        TokenList new_token;
        ifstream file;
        file.open("vhdl test case.txt");
        while(!file.eof())
        {
            string line;
            getline(file, line);
            tokenizer.setString(&line);
            while(!tokenizer.isComplete())
            {
                new_token.append(tokenizer.getNextToken());
            }
            //Add newline character to separate each line (from vhdl file) in tokenlist
            new_token.append("\n");
        }
        Token *temp = new_token.getFirst();
        while(temp)
        {
            //set the type and details for all token
            new_token.findAndSetTokenDetails(temp);
            temp = temp->getNext();
        }

        //CASE1: Checking for missing "then"
        temp = new_token.getFirst();
        while(temp)
        {
            //Find "if" token which is not "end if" token or find "elsif" token
            if((temp->getStringRep() == "if" && temp->getPrev()->getStringRep() != "end") || temp->getStringRep() == "elsif")
            {
                //hold point to the "if" token in order to print out later
                Token *hold = temp;
                do{
                    temp = temp->getNext();
                }while(temp->getStringRep() != "\n");
                //"if there is missing then"
                if(temp->getPrev()->getStringRep() != "then")
                {
                    while(hold != temp)
                    {
                        cout << hold->getStringRep() << " ";
                        hold = hold->getNext();
                    }
                    cout << " --missing \"then\"" << endl;
                }
            }
            else
                temp = temp->getNext();
        }

        //CASE2: Checking for missing "end if"
        temp = new_token.getFirst();
        //SPECIAL CASE: Missing "end if" for entire file
        //Check if the number of missing "end if" is equal to number of "if"
        if(numOfMissingEndif(tokens) == numOfIf(tokens))
        {
            while(temp)
            {
                if(temp->getStringRep() == "if")
                {
                    while(temp)
                    {
                        cout << temp->getStringRep() << " ";
                        temp = temp->getNext();
                    }
                }
                else
                    temp = temp->getNext();

            }
            cout << " --missing \"end if\" for whole file" << endl;
        }
        //SPECIAL CASE: Missing "end if" at the end of file
        else if(numOfMissingEndif(new_token) == 1)
        {
            Token *tail = new_token.getLast();
            //Find the last "if" in the token
            while(tail->getStringRep() != "if")
            {
                tail = tail->getPrev();
            }
            //Check if that "if" token is not from "end if" statement
            if(tail->getPrev()->getStringRep() != "end")
            {
                //Make tail pointer terminate at last ";" token of tokenlist
                while(tail->getNext()->getNext())
                {
                    cout << tail->getStringRep() << " ";
                    tail = tail->getNext();
                }
                cout << " --missing \"end if\"" << endl;
                //update temp to nullptr
                temp = nullptr;
            }
        }
        //GENERAL CASE (NOT INCLUDE NESTED "IF" STATEMENT CASE)
        while(temp)
        {
            //Find first "if" statement
            if(temp->getStringRep() == "if" && (temp->getPrev()->getStringRep() != "end" || temp->getPrev() == nullptr))
            {
                Token *start;
                Token *end;
                start = temp;
                //check for the first "end if" after if token
                while(temp->getStringRep() != "end" && temp)
                {
                    //cout << temp->getStringRep();
                    temp = temp->getNext();
                }
                //end pointer points to token after "end if"
                if(temp != nullptr)
                    end = temp->getNext()->getNext();
                //Check wheter there are if between "if" and "end if"
                Token *check = start->getNext();
                while(check != end)
                {
                    if(check->getStringRep() == "if" && check->getPrev()->getStringRep() != "end")
                    {
                        while(start != end->getNext())
                        {
                            cout << start->getStringRep() << " ";
                            start = start->getNext();
                        }
                        //update check to terminate the while loop after missing "end if" is found
                        check = end;
                        cout << " --missing \"end if\"" << endl;
                    }
                    else
                        check = check->getNext();
                }
                temp = check->getNext();

            }
            else
                temp = temp->getNext();
        }

        //CASE3: Checking for type mismatch & width mismatch
        temp = new_token.getFirst();
        while(temp)
        {
            //Find the operator or multi-operator
            if(temp->getStringRep() == "=" || temp->getStringRep() == "/=" || temp->getStringRep() == "<" || temp->getStringRep() == ">" || temp->getStringRep() == "<=" || temp->getStringRep() == ">=")
            {
                //Check for type mismatch and width mismatch
                if(!checkType(temp->getPrev(),temp->getNext()) || !checkWidth(temp->getPrev(),temp->getNext()))
                {
                    Token *begin = temp;
                    Token *end = temp;
                    //find the token that is the beginning of this line
                    while(begin->getPrev()->getStringRep() != "\n")
                    {
                        begin = begin->getPrev();
                    }
                    //find the token that is the end of this ine
                    while(end->getStringRep() != "\n")
                    {
                        end = end->getNext();
                    }
                    while(begin != end)
                    {
                        cout << begin->getStringRep() << " ";
                        begin = begin->getNext();
                    }
                    //for type mismatch case
                    if(!checkType(temp->getPrev(),temp->getNext()))
                        cout << " --type mismatch" << endl;
                    //for width mismatch case
                    else if(!checkWidth(temp->getPrev(),temp->getNext()))
                        cout << " --width mismatch" << endl;
                    temp = end->getNext();
                }
                else
                    temp = temp->getNext();
            }
            else
                temp = temp->getNext();
        }

    }

	return 0;
}
