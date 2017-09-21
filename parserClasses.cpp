//Use only the following libraries:
#include "parserClasses.h"
//don't forget to remove this && CHANGE ATOI TO STOI !!!!!!!!!!!!!!!!!
#include <cstdlib>



//****TokenList class function definitions******
//           function implementations for append have been provided and do not need to be modified

//Creates a new token for the string input, str
//Appends this new token to the TokenList
//On return from the function, it will be the last token in the list
void TokenList::append(const string &str) {
	Token *token = new Token(str);
	append(token);
}

//Appends the token to the TokenList if not null
//On return from the function, it will be the last token in the list
void TokenList::append(Token *token) {
	if (!head) {
		head = token;
		tail = token;
	}
	else {
		tail->setNext(token);
		token->setPrev(tail);
		tail = token;
	}
}


//Complete the implementation of the following member functions:
//****Tokenizer class function definitions******

//Computes a new tokenLength for the next token
//Modifies: size_t tokenLength, and bool complete
//(Optionally): may modify offset
//Does NOT modify any other member variable of Tokenizer
void Tokenizer::prepareNextToken()
{/*Fill in implementation */

    string mystring=*str;
    string substring;
    string remain_str=mystring.substr(offset,mystring.length()-offset);
    size_t found=remain_str.find_first_of(delimiters);

    if (found==0 && remain_str[found]=='\t')
    {
        size_t pos=remain_str.find_first_not_of('\t');

        if (pos!=string::npos)
        {
            offset=offset+pos;
            remain_str=remain_str.substr(pos, (*str).length()-offset);
            found=remain_str.find_first_of(delimiters);
        }
        else
        {
            offset=(*str).length();
            found=string::npos;
            complete=true;
        }

    }

    if (found==0 && remain_str[found]==' ')                          // case of whitespace at very front
    {
        size_t pos=remain_str.find_first_not_of(' ');

        if (pos!=string::npos)
        {
            offset=offset+pos;
            remain_str=remain_str.substr(pos, (*str).length()-offset);
            found=remain_str.find_first_of(delimiters);
        }
        else
        {
            offset=(*str).length();     //not sure
            found=string::npos;
            complete=true;
        }

    }

    if (ifcomment==true)
    {
        tokenLength=mystring.length()-offset;

    }

    else
    {

        if (found == string::npos)
        {
            tokenLength=remain_str.length();
            if (offset==mystring.length())
                complete=true;
        }

        else if (remain_str[found]==' ')
        {
            tokenLength=found;
        }
        else if (remain_str[found]=='\t')
        {
            tokenLength=found;
        }



        else if (remain_str[found]=='-' )    //comment --
        {
            if(found == 0)
            {
                if (remain_str[found+1]=='-')
                    tokenLength=2;
                else
                    tokenLength=1;
            }
            else
                tokenLength = found;

        }



        else
        {

            //multi-charactors operator
            if (ifmulticha(remain_str, found))
            {
                if(found==0)
                    tokenLength=2;

                else
                    tokenLength=found;

                //if (offset==(*str).length())
                    //complete=true;
            } //multi-charactor opertors end


            else
            {
                // check binary bit vector      'b' 'o' 'x'  only there three case?
                if (if_bin_vec(remain_str, found))
                {
                    if (found==0)
                        tokenLength=3;
                    else
                        tokenLength=found;
                }
                else if(remain_str[found]=='"')
                {

                    if (found==0)
                    {
                        string substring=remain_str.substr(found+1,remain_str.length()-found-1);   //used to find another "
                        size_t pos=substring.find_first_of('"');
                        tokenLength=pos+2;
                    }
                    else
                        tokenLength=found;


                } //bit vector chack end



                // sigle delimiter
                else
                {
                    if (found==0)
                    {
                        tokenLength=1;
                    }
                    else
                        tokenLength=found;

                }

            }

        }

    }
}

//Sets the current string to be tokenized
//Resets all Tokenizer state variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
void Tokenizer::setString(string *gstr)
{/*Fill in implementation */
    this->str=gstr;
    offset=0;
    tokenLength=0;
    complete=false;
    prepareNextToken();


}

bool Tokenizer::ifmulticha(string s, size_t pos) const
{
    string str=s.substr(pos,2);
    if (str=="<=")
        return true;
    else if (str==">=")
        return true;
    else if (str=="/=")
        return true;
    else if (str==":=")
        return true;
    else if (str=="=<")
        return true;
    else if (str=="=>")
        return true;
    else if (str=="**")
        return true;
    else
        return false;
}

bool Tokenizer::if_bin_vec(string s, size_t pos) const
{
    string str=s.substr(pos,3);
    if (str[0]=='\'' && str[2]=='\'')
        return true;
    else
        return false;

}


//Returns the next token. Hint: consider the substr function
//Updates the tokenizer state
//Updates offset, resets tokenLength, updates processingABC member variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
string Tokenizer::getNextToken()
{/*Fill in implementation */


    string s=(*str).substr(offset,tokenLength);

    offset=offset+tokenLength;

    if (s=="x" || s=="o" || s=="b" || s=="X" || s=="B" || s=="O")   //x"123" o"123" b"123" X"123" O"123" B"123
    {
        if ((*str)[offset]=='\"')
        {
            string aremain= (*str).substr(offset+1,(*str).length()-offset-1);
            size_t found=aremain.find_first_of('\"');
            if(found != string::npos)
            {
                s=(*str).substr(offset-1,found+3);
                offset=offset+found+2;
            }
        }
    }




    string remain= (*str).substr(offset,(*str).length()-offset);


    if ((*str)[offset] == ' ')
    {
        if (remain.find_first_not_of(' ') != string::npos )
        {
            offset=offset+remain.find_first_not_of(' ');
            remain= (*str).substr(offset,(*str).length()-offset);
        }
        else                                          // all whitespace remain
            complete=true;

    }

    if (s=="--")
    {
        ifcomment=true;
    }



    if ( offset == (*str).length())
    {
        complete=true;
        ifcomment=false;                                // make sure next line wont be comment
        while (s[s.length()-1]==' ')                   //case of whitespace at very end fo comment
        {
            s.erase(s.length()-1);
        }

    }



    tokenLength=0;

    prepareNextToken();
    return s;



}



//****Challenge Task Functions******

//Removes the token from the linked list if it is not null
//Deletes the token
//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.
void TokenList::deleteToken(Token *token)
{
    if(token->getPrev()==nullptr)
    {
        token->getNext()->prev=nullptr;
        head=token->getNext();
        delete token;
    }
    else if(token->getNext()==nullptr)
    {
        token->getPrev()->next=nullptr;
        tail=token->getPrev();
        delete token;
    }
    else
    {
        Token *temp=token;
        Token *pre_token=token->getPrev();
        Token *next_token=token->getNext();
        pre_token->next=temp->next;
        next_token->prev=temp->prev;
        delete temp;
    }


}

//Removes all comments from the tokenList including the -- marker
//Returns the number of comments removed
int removeComments(TokenList &tokenList)
{
    Token *t=tokenList.getFirst();
    int num=0;
    while (t)
    {
        if (t->getStringRep()=="--")
        {
            num++;
            Token *temp=t;
            t=t->getNext();
            tokenList.deleteToken(temp);
            temp=t;
            t=t->getNext();
            tokenList.deleteToken(temp);

        }
        else
            t=t->getNext();
    }
    t=nullptr;
    return num;
}

//code for project
//Token class
Token::Token()
{
    next = nullptr;
    prev = nullptr;
    details = nullptr;
}

Token::Token(const string &stringRep)
{
    next = nullptr;
    prev = nullptr;
    this->stringRep = stringRep;
    details = nullptr;

}

Token::Token(const Token &token)
{
    this->next = nullptr;
    this->prev = nullptr;
    this->stringRep = token.stringRep;
    this->_isKeyword = token._isKeyword;
    this->type = token.type;
    if(token.details != nullptr)
    {
        this->details = new tokenDetails;
        this->details->type = token.details->type;
        this->details->width = token.details->width;
    }
    else
        this->details = nullptr;
}

Token::~Token()
{
    if(details == nullptr)
        delete details;
}

void Token::operator =(const Token& token)
{
    this->next = token.next;
    this->prev = token.prev;
    this->stringRep = token.stringRep;
    this->_isKeyword = token._isKeyword;
    this->type = token.type;
    if(token.details != nullptr)
    {
        this->details = new tokenDetails;
        this->details->type = token.details->type;
        this->details->width = token.details->width;
    }
    else
        this->details = nullptr;
}

void Token::setTokenDetails(const string &type, int width)
{
    details = new tokenDetails;
    details->type = type;
    details->width = width;
}

//TokenList class

//Find token details and type and update token.
void TokenList::findAndSetTokenDetails(Token *token)
{
    //change all of character in token to be lower case
    string str = token->getStringRep();
    int len = str.length();
    for(int i = 0; i < len; i++)
        str[i] = tolower(str[i]);
    token->setStringRep(str);
    //check if it is keyword
    token->_isKeyword = false;
    for(int i = 0; i < 97; i++)
    {
        if(token->getStringRep() == keywords[i])
        {
            token->setKeyword();
        }
    }

    //Initialized the type to be other first
    token->setTokenType(T_Other);

    //check for comment bodies type
    Token* temp = token->getPrev();
    if(temp != nullptr)
    {
        if(temp->getStringRep() == "--")
        {
            token->setTokenType(T_CommentBody);
            return;
        }
    }

    //token is a keyword case
    if(token->isKeyword())
    {
        //Identifier start with alphabet
        token->setTokenType(T_Identifier);
        //check for operators type
        for(int i = 0; i < 16; i++)
        {
            if(token->getStringRep() == k_operators[i])
            {
                token->setTokenType(T_Operator);
                return;
            }
        }
    }

    //token is not a keyword case
    else if(!(token->isKeyword()))
    {
        //check for operator type
        for(int i = 0; i < 12; i++)
        {
            if(token->getStringRep() == operators[i])
            {
                token->setTokenType(T_Operator);
                return;
            }
        }

        //check for literals
        //CASE1: true & false
        if(token->getStringRep() == "true" || token->getStringRep() == "false")
        {
            token->setTokenType(T_Literal);
            token->setTokenDetails("boolean",0);
            return;
        }
        //CASE2: '0' & '1'
        else if(token->getStringRep() == "'0'" || token->getStringRep() == "'1'")
        {
            token->setTokenType(T_Literal);
            token->setTokenDetails("std_logic",0);
            return;
        }
        //CASE3: integers
        else if(token->getStringRep() >= "0" && token->getStringRep() <= "9")
        {
            token->setTokenType(T_Literal);
            token->setTokenDetails("integer",0);
            return;
        }
        //CASE4: binary vector
        else if(token->getStringRep()[0] == 'b' || token->getStringRep()[0] == 'o' || token->getStringRep()[0] == 'x' || token->getStringRep()[0] == '\"')
        {
           if(token->getStringRep()[1] == '\"' || token->getStringRep()[0] == '\"')
           {
               int v_width;
               if(token->getStringRep()[0] == '\"')
                v_width = len-2;
               else if(token->getStringRep()[0] == 'b')
                v_width = len-3;
               else if(token->getStringRep()[0] == 'o')
                v_width = (len-3)*3;
               else if(token->getStringRep()[0] == 'x')
                v_width = (len-3)*4;
               token->setTokenType(T_Literal);
               token->setTokenDetails("std_logic_vector",v_width);
               return;
           }
        }

        //check for identifiers (token which is not operator, literal, comment and start with alphabet is identifiers)
        if(!(token->isKeyword()) && token->getStringRep()[0] >= 'a' && token->getStringRep()[0] <= 'z')
        {
            token->setTokenType(T_Identifier);
            //check for identifier that has same name (they will have same type)
            //to find the type
            Token* temp_ptr = token->getNext();
            if(temp_ptr->getStringRep() == ":" || temp_ptr->getStringRep() == ",")
            {
                //in case that the user declare more than one variable in one line
                if(temp_ptr->getStringRep() == ",")
                {
                    do{
                        temp_ptr = temp_ptr->getNext();
                    }while(temp_ptr->getStringRep() != ":");
                }
                temp_ptr = temp_ptr->getNext();
                //for vector: std_logic_vector(# down/to #)
                if(temp_ptr->next->stringRep == "(")
                {
                    Token* w_ptr = temp_ptr->next;
                    int detail_width;
                    w_ptr = w_ptr->next;
                    int m = atoi((w_ptr->stringRep).c_str());             //change back to: stoi(w_ptr->stringRep)
                    w_ptr = w_ptr->next;
                    int n = atoi((w_ptr->next->stringRep).c_str());       //change back to: stoi(w_ptr->stringRep)
                    if(m>n)
                        detail_width = m-n+1;
                    else
                        detail_width = n-m+1;
                    token->setTokenDetails("std_logic_vector",detail_width);
                }
                //case for signal a : std_logic;
                else
                {
                    string s = temp_ptr->getStringRep();
                    int length = s.length();
                    for(int i = 0; i < length; i++)
                    {
                        //change the string of type to lower case
                        s[i] = tolower(s[i]);
                    }
                    token->setTokenDetails(s,0);
                }
                return;
            }

            else
            {
                Token* temp = getFirst();
                while(temp->stringRep != token->stringRep)
                {
                    if(temp->next->stringRep == token->stringRep && temp->next->details != nullptr)
                    {
                        temp = temp->next;
                        tokenDetails* detail_ptr = temp->details;
                        token->setTokenDetails(detail_ptr->type,detail_ptr->width);
                        return;
                    }
                    temp = temp->getNext();
                }
            }
        }

    }

}

//Removes all tokens of the given tokenType
//Returns the number of tokens removed
int removeTokensOfType(TokenList &tokenList, tokenType type)
{
    Token* t = tokenList.getFirst();
    int num(0);
    while(t)
    {
        if(t->getTokenType() == type)
        {
            num++;
            Token* temp=t;
            t=t->getNext();
            tokenList.deleteToken(temp);
        }
        else
            t=t->getNext();
    }
    return num;
}

//Creates a new TokenList, and returns a pointer to this list
//Searches for all conditional expressions in tokenList and appends them to the new list
//Each token that is part of a condtional expression is appended sequentially
//At the end of a conditional expression a newline character is appened
//tokenList is NOT modified
TokenList* findAllConditionalExpressions(const TokenList &tokenList)
{
    Token* t = tokenList.getFirst();
    TokenList *t_result = new TokenList;
    while(t)
    {
        if(t->getStringRep() == "if" || t->getStringRep() == "elsif" || t->getStringRep() == "when")
        {
            t = t->getNext();
            while(t->getStringRep() != "then" && t->getStringRep() != "else")
            {
                t_result->append(t);
                t = t->getNext();
            }
            t_result->append("\n");
        }
        if(t->getStringRep() == "end" && t->getNext()->getStringRep() == "if")
        {
            t = t->getNext()->getNext();
        }
        else
            t = t->getNext();
    }
    return t_result;
}











