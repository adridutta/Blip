#include <iostream>
#include <map>
#include <string>
#include <string.h>
#include "Parse.h"

using namespace std;

typedef struct node {
    int type;         // 0 for symbol and 1 for value;
    int operation; //0 for +, 1 for -, 2 for mult, 3 for divide, 4 for mod, 5 for &&, 6 for ||, 7 for <, 8 for >, 9 for ==, 10 for !=, 11 for <=, 12 for >=, 13 for !, 14 for `;
    int val = 0;
    node* parent = NULL;
    node* left = NULL;
    node* right = NULL;
    char* variable = NULL;

} node;

typedef struct stattype {
    int exptype;
    char* variable;
    node* tree = NULL;
    int val;
    char* othervariable = NULL;
    node* condition = NULL;
    stattype* positive= NULL;
    stattype* negative= NULL;
    stattype* prev = NULL;
    stattype* next = NULL;




} stattype;



std:: map<char*, int> symbols;
std:: map<char*, int>::iterator symbolit;
void createvar (char* , int );
int processtree(node*);
node* childgen(node*);
node* treegen(void);
void processtext(void);
void processoutput(void);
void run (void);
bool checkmap (char*);
char* checkmap2 (char*);
void setval (char*, int);
int getval (char*);
void processdo(void);
void processifmain(void);
void deletetree(node*);
void skipif(void);

void skipif (void)
{
    const char* fi = "fi";
    while (strcmp(fi, next_token()) !=0)
    {
        read_next_token();
    }
    read_next_token();
}

void deletetree(node* root)
{
    if (root->left == NULL && root->right == NULL)
    {
        delete root; return;
    }

    else if (root->left != NULL && root->right == NULL)
    {
        deletetree(root->left);
        delete root;
        return;
    }
    else if (root->left == NULL && root->right != NULL)
    {
        deletetree(root->right);
        delete root;
        return;
    }
    deletetree(root->left);
    deletetree(root->right);
    delete root;
}

int getval (char* location) //gets value given a variable name
{
    for (symbolit = symbols.begin(); symbolit!=symbols.end();symbolit++)
    {
        int istrue = strcmp(location, symbolit->first);
        if (istrue ==0)
        {
            return symbolit->second;
        }
    }
    return 0;
}


char* checkmap2 (char* check) // checks if variable is in table and returns that variable if it is
{
    int istrue = -1000;
    for (symbolit = symbols.begin(); symbolit !=symbols.end();symbolit++)
    {
        istrue = strcmp(check, symbolit->first);
        if (istrue== 0)
        {
            return symbolit->first;
        }
    }
    return NULL;
}

void setval (char* var, int newval) //sets an existing variable or creates a new one
{
    char* location = checkmap2(var);
    if (location == NULL)
    {
        cout << "variable " << var <<  " not declared" << endl;
        char* heapvar = new char[strlen(var)];
        strcpy(heapvar, var);
        createvar(heapvar, newval);
        return;
    }

    symbols[location] = newval;
}

bool checkmap (char* check) // sees if variable exists in map
{
    int istrue= -1000;
    for (symbolit = symbols.begin(); symbolit !=symbols.end();symbolit++)
    {
        istrue = strcmp(check, symbolit->first);
        if (istrue== 0)
        {
            return true;
        }
    }
    return false;
}

void createvar (char* varname, int val) //creates a new variable given a name and value
{

    if (checkmap(varname))
    {
        cout << "variable " << varname << " incorrectly re-initialized" << endl;
        setval(varname, val);
    }
    char* newvar = new char[strlen(varname)];
    strcpy(newvar,varname);
    symbols.insert(std:: pair<char*, int>(newvar, val));

}


int processtree(node* root){
    if (root->type == 1)
    {
        if (root->variable !=NULL)
        {
            return getval(root->variable);
        }

        else return root->val;
    }
    else if (root->type == 0)
    {
        if (root->operation == 0)
        {
            return processtree (root->left) + processtree(root->right);
        }
        if (root->operation == 1)
        {
            return processtree (root->left) - processtree(root->right);
        }
        if (root->operation == 2)
        {
            return processtree (root->left) * processtree(root->right);
        }

        if (root->operation == 3)
        {
            return processtree (root->left) / processtree(root->right);
        }

        if (root->operation == 4)
        {
            return processtree (root->left) % processtree(root->right);
        }
        if (root->operation == 5)
        {
            return processtree (root->left) && processtree(root->right);
        }
        if (root->operation == 6)
        {
            return processtree (root->left) || processtree(root->right);
        }
        if (root->operation == 7)
        {
            return processtree (root->left) < processtree(root->right);
        }
        if (root->operation == 8)
        {
            return processtree (root->left) > processtree(root->right);
        }
        if (root->operation == 9)
        {
            return processtree (root->left) == processtree(root->right);
        }
        if (root->operation == 10)
        {
            return processtree (root->left) != processtree(root->right);
        }
        if (root->operation == 11)
        {
            return processtree (root->left) <= processtree(root->right);
        }
        if (root->operation == 12)
        {
            return processtree (root->left) >= processtree(root->right);
        }
        if (root->operation == 13)
        {
            return !(processtree (root->left));
        }
        if (root->operation == 14)
        {
            return (processtree (root->left)) * -1;
        }
    }
} //given a root processes an expression tree

node* childgen(node* parent){
    node* child = new node;
    const char* operationvalue;
    const char* add = "+";
    const char* sub = "-";
    const char* mult = "*";
    const char* div = "/";
    const char* mod = "%";
    const char* nd = "&&";
    const char* orr = "||";
    const char* less = "<";
    const char* great = ">";
    const char* eqq = "==";
    const char* noteq = "!=";
    const char* los = "<=";
    const char* gos = ">=";
    const char* nott = "!";
    const char* neg = "~";
    int istrue;

    read_next_token();
    if (next_token_type == NUMBER)
    {
        child->type = 1;
        child->val = token_number_value;
        child->parent = parent;
        return child;
    }
    else if (next_token_type == SYMBOL)
    {
        child->type = 0;
        child->parent = parent;
        operationvalue = next_token();
        istrue = strcmp(operationvalue, add);
        if (istrue == 0) { child->operation = 0; }
        istrue = strcmp(operationvalue, sub);
        if (istrue == 0) { child->operation = 1; }
        istrue = strcmp(operationvalue, mult);
        if (istrue == 0) { child->operation = 2; }
        istrue = strcmp(operationvalue, div);
        if (istrue == 0) { child->operation = 3; }
        istrue = strcmp(operationvalue, mod);
        if (istrue == 0) { child->operation = 4; }
        istrue = strcmp(operationvalue, nd);
        if (istrue == 0) { child->operation = 5; }
        istrue = strcmp(operationvalue, orr);
        if (istrue == 0) { child->operation = 6; }
        istrue = strcmp(operationvalue, less);
        if (istrue == 0) { child->operation = 7; }
        istrue = strcmp(operationvalue, great);
        if (istrue == 0) { child->operation = 8; }
        istrue = strcmp(operationvalue, eqq);
        if (istrue == 0) { child->operation = 9; }
        istrue = strcmp(operationvalue, noteq);
        if (istrue == 0) { child->operation = 10; }
        istrue = strcmp(operationvalue, los);
        if (istrue == 0) { child->operation = 11; }
        istrue = strcmp(operationvalue, gos);
        if (istrue == 0) { child->operation = 12; }
        istrue = strcmp(operationvalue, nott);
        if (istrue == 0) { child->operation = 13; }
        istrue = strcmp(operationvalue, neg);
        if (istrue == 0) { child->operation = 14; }
        read_next_token;
        if ((child->operation == 13) || (child->operation == 14))
        {
            child->left = childgen(child);
            return child;
        }
        if (next_token_type == NAME)
        {
            char* prevvar = new char [strlen(next_token())];
            strcpy(prevvar, next_token());
            child->variable = prevvar;
            child->val = getval(prevvar);
            child->type = 1;
            child->parent = parent;
            return child;

        }

        child->left = childgen(child);
        child->right = childgen(child);
        return child;
    }
    if (next_token_type == NAME)
    {
        char location [strlen(next_token())];
        strcpy(location, next_token());
        child->val = getval(location);
        child->variable = new char[strlen(next_token())];
        strcpy(child->variable, next_token());
        child->parent = parent;
        child->type = 1;
        return child;
    }


} //generates children for expression tree

node* treegen(void) //generates parent for expression tree
{








    char* operationvalue = new char[strlen(next_token())];
    const char* add = "+";
    const char* sub = "-";
    const char* mult = "*";
    const char* div = "/";
    const char* mod = "%";
    const char* nd = "&&";
    const char* orr = "||";
    const char* less = "<";
    const char* great = ">";
    const char* eqq = "==";
    const char* noteq = "!=";
    const char* los = "<=";
    const char* gos = ">=";
    const char* nott = "!";
    const char* neg = "~";
    int istrue;
    node* root = new node;

    if (next_token_type == NUMBER)
    {
        root->type = 1;
        root->val = token_number_value;
        return root;
    }
    else if(next_token_type == NAME) {
        root->type=1;
        char* location = new char[strlen(next_token())];
        strcpy(location, next_token());
        root->variable = location;
        return root;

    }

        //setup root and set its value
    else {
        root->type = 0;
        strcpy(operationvalue, next_token());
        istrue = strcmp(operationvalue, add);
        if (istrue == 0) { root->operation = 0; }
        istrue = strcmp(operationvalue, sub);
        if (istrue == 0) { root->operation = 1; }
        istrue = strcmp(operationvalue, mult);
        if (istrue == 0) { root->operation = 2; }
        istrue = strcmp(operationvalue, div);
        if (istrue == 0) { root->operation = 3; }
        istrue = strcmp(operationvalue, mod);
        if (istrue == 0) { root->operation = 4; }
        istrue = strcmp(operationvalue, nd);
        if (istrue == 0) { root->operation = 5; }
        istrue = strcmp(operationvalue, orr);
        if (istrue == 0) { root->operation = 6; }
        istrue = strcmp(operationvalue, less);
        if (istrue == 0) { root->operation = 7; }
        istrue = strcmp(operationvalue, great);
        if (istrue == 0) { root->operation = 8; }
        istrue = strcmp(operationvalue, eqq);
        if (istrue == 0) { root->operation = 9; }
        istrue = strcmp(operationvalue, noteq);
        if (istrue == 0) { root->operation = 10; }
        istrue = strcmp(operationvalue, los);
        if (istrue == 0) { root->operation = 11; }
        istrue = strcmp(operationvalue, gos);
        if (istrue == 0) { root->operation = 12; }
        istrue = strcmp(operationvalue, nott);
        if (istrue == 0) { root->operation = 13; }
        istrue = strcmp(operationvalue, neg);
        if (istrue == 0) { root->operation = 14; }
        read_next_token;
        if ((root->operation == 13) || (root->operation == 14))
        {
            root->left= childgen(root);
            return root;
        }
        root->left = childgen(root);
        root->right = childgen(root);
        return root;


    }}

void processtext(void) //processes text
{
    read_next_token();
    if (strlen(next_token())!=0) {
        char print2[strlen(next_token())];
        char* print = print2;
        strcpy(print2, next_token());
        cout << print;

    }

}


void processblock(stattype* block)
{
    stattype* current= block;
    while (current->prev != NULL)
    {
        current = current->prev;
    }

    while (current !=NULL) {

        if (current->exptype == 2)     //set variable
        {
            if (current->tree != NULL)
            {
                setval(current->variable, processtree(current->tree));
            }
            else if (current->othervariable != NULL)
            {
                setval(current->variable, getval(current->othervariable));
            }

            else {
                setval(current->variable, current->val);
            }
        }

        else if (current->exptype == 3) {            //output
            if (current->tree != NULL) {
                cout << processtree(current->tree);
            } else if (current->variable != NULL) {
                cout<< getval(current->variable);
            } else {
                cout << current->val;
            }
        }

        else if (current->exptype == 4)              //text
        {
            cout << current->variable;
        }
        else if (current->exptype==1)                //var keyword
        {
            if (current->tree !=NULL)
            {
                createvar(current->variable,processtree(current->tree));
            }
            else if (current->othervariable !=NULL)
            {
                createvar(current->variable,getval(current->othervariable));
            }
            else {
                createvar(current->variable, current->val);
            }
        }
        else if (current->exptype ==5)
        {
            if (processtree(current->condition)==true)
            {
                processblock(current->positive);
            }
            else if (current->negative != NULL)
            {
                processblock(current->negative);
            }
        }
        else if (current->exptype ==6)
        {
            while (processtree(current->condition)==true)
            {
                processblock(current->positive);
            }
        }


        current = current->next;
    }

}


stattype* record(int which) // 0 for do
{
    stattype* temp = NULL;
    const char* var = "var";
    const char* set = "set";
    const char* output = "output";
    const char* text = "text";
    const char* comment = "//";
    const char* ifstat = "if";
    const char* ifend = "fi";
    const char* elsestart = "else";
    const char* dostart = "do";
    if (which == 0)
    {
       const char* end = "od";
       while ((strcmp(end, next_token()) != 0) && (strcmp(ifend, next_token())!=0) && (strcmp(elsestart, next_token())!=0))
       {
           if ( next_token_type == NAME)        //see if first statement is keyword
           {
              char* keyword = new char[strlen(next_token())];
              strcpy (keyword, next_token());
              if (strcmp(keyword, output) == 0) //see if statement is output
              {
                  stattype* statement = new stattype;
                  if (temp != NULL) {temp->next = statement;}
                  statement->prev = temp;
                  statement->exptype = 3;
                  read_next_token();
                  if (next_token_type == NUMBER)        // find out what variable is equal to
                  {
                      statement->val = token_number_value;
                  }
                  else if (next_token_type == SYMBOL)
                  {
                      statement->tree = treegen();
                  }
                  else if (next_token_type == NAME) {

                      statement->variable = new char[strlen(next_token())];
                      strcpy(statement->variable, next_token());

                  }
                  temp = statement;
                  read_next_token();
              }
               else if (strcmp(keyword, text) == 0)     // see if it is text
              {
                  stattype* statement = new stattype;
                  if (temp != NULL) {temp->next = statement;}
                  statement->prev = temp;
                  statement->exptype = 4;
                  read_next_token();
                  statement->variable = new char[strlen(next_token())];
                  strcpy(statement->variable, next_token());
                  temp = statement;
                  read_next_token();
              }
                else if (strcmp(keyword, set)==0) {
                  stattype *statement = new stattype;
                  if (temp != NULL) {temp->next = statement;}
                  statement->prev = temp;
                  statement->exptype = 2;
                  read_next_token();
                  statement->variable = new char[strlen(next_token())];
                  strcpy(statement->variable, next_token()); // get variable name
                  read_next_token();
                  if (next_token_type == NUMBER)        // find out what variable is equal to
                  {
                      statement->val = token_number_value;
                  } else if (next_token_type == SYMBOL) {
                      statement->tree = treegen();
                  } else if (next_token_type == NAME) {
                      strcpy(statement->othervariable, next_token());
                  }
                  temp = statement;
                  read_next_token();
              }
              else if (strcmp(keyword, comment)==0)               //see if comment
              {
                  skip_line();
                  read_next_token();
              }

              else if (strcmp(keyword, var) == 0)     //see if first statement contains a var
              {

                  stattype* statement = new stattype;
                  if (temp != NULL) {temp->next = statement;}
                  statement->prev = temp;
                  statement->exptype = 1;
                  read_next_token();
                  strcpy(statement->variable, next_token()); // get variable name
                  read_next_token();
                  if (next_token_type == NUMBER)        // find out what variable is equal to
                  {
                     statement->val = token_number_value;
                  }
                  else if (next_token_type == SYMBOL)
                  {
                    statement->tree = treegen();
                  }
                  else if (next_token_type == NAME)
                  {

                      strcpy(statement->othervariable, next_token());
                  }
                  temp = statement;   // store it temporary for next on ll
                  read_next_token();

              }
              else if (strcmp(keyword, ifstat)==0)
              {
                  stattype* statement = new stattype;
                  if (temp!=NULL){temp->next = statement;}
                  statement->prev = temp;
                  statement->exptype = 5;
                  read_next_token();
                  statement->condition = treegen();
                  read_next_token();
                  statement->positive =record(0);
                  if (strcmp(elsestart,next_token())== 0)
                  {
                      read_next_token();
                      statement->negative = record(0);
                  }
                  temp = statement;
                  read_next_token();

              }
              else if (strcmp(keyword, dostart) == 0)
              {
                  stattype* statement = new stattype;
                  if (temp!=NULL) {temp->next = statement;}
                  statement->prev = temp;
                  statement->exptype= 6;
                  read_next_token();
                  statement->condition = treegen();
                  read_next_token();
                  statement->positive = record(0);
                  temp = statement;
                  read_next_token();
              }

            delete keyword;
           }
        else {read_next_token();}
       }


       return temp;

    }



}


void processdo(void)       //process do loop
{
    node* condition;
    read_next_token();
    condition = treegen();
    stattype* block;
    read_next_token();
    block = record(0);
    while (processtree(condition) == true) {
        processblock(block);

    }



}


void processifmain(void)   //0 for main loop and 1 for do loop
{

    const char *text = "text";
    const char *output = "output";
    const char *var = "var";
    const char *set = "set";
    const char *comment = "//";
    const char *dostart = "do";
    const char *ifstart = "if";
    int istrue;
    const char* end = "fi";
    const char* elsestat = "else";

    read_next_token();
    node* condition = treegen();
    read_next_token();
    if(processtree(condition)==false)
    {
        while ((strcmp(end,next_token())!=0) && (strcmp(elsestat,next_token())!=0))
        {
            read_next_token();
            if (strcmp(comment, next_token())==0)
            {
                skip_line();
            }
        }
        if (strcmp(elsestat, next_token())==0)
        {
            char  word [strlen(next_token())];
            read_next_token();
            while(strcmp(end,next_token())!=0)
            {
                if (next_token_type == NAME) {
                    strcpy(word, next_token());
                    istrue = strcmp(word, text);  //text
                    if (istrue == 0) {
                        processtext();
                    }
                    istrue = strcmp (word, output); //output
                    if (istrue == 0)
                    {
                        processoutput();
                    }
                    istrue = strcmp (word, comment);  //comment
                    if (istrue ==0)
                    {
                        skip_line();
                    }
                    istrue = strcmp (word, var);         //var
                    if (istrue ==0)
                    {
                        read_next_token();
                        char* varname = new char[strlen(next_token())];
                        strcpy(varname, next_token());
                        int val;
                        read_next_token();
                        if (next_token_type == NUMBER)
                        {
                            val = token_number_value;
                            createvar(varname, val);
                        }
                        else if (next_token_type == SYMBOL)
                        {
                            node* root = treegen();
                            val = processtree(root);
                            createvar(varname, val);

                        }
                        else if (next_token_type == NAME)
                        {
                            char prevvar[strlen(next_token())];
                            strcpy(prevvar, next_token());
                            val = getval(prevvar);
                            createvar(varname, val);

                        }}
                    istrue = strcmp (word, set);               //set
                    if (istrue ==0)
                    {
                        read_next_token();
                        char location [strlen(next_token())];
                        strcpy(location, next_token());
                        read_next_token();
                        if (next_token_type == NUMBER) {
                            int newval = token_number_value;
                            setval(location, newval);
                        }
                        else if (next_token_type == SYMBOL)
                        {
                            node* root = treegen();
                            int newval = processtree(root);
                            setval(location, newval);
                        }
                        else if (next_token_type == NAME)
                        {
                            char prevvar [strlen(next_token())];
                            strcpy(prevvar, next_token());
                            int newval = getval(prevvar);
                            setval(location, newval);
                        }


                    }

                    istrue = strcmp(word, dostart);
                    if (istrue ==0)
                    {
                        processdo();
                    }
                    istrue = strcmp(word, ifstart);
                    if (istrue==0 )
                    {
                        processifmain();
                    }


                }
                read_next_token();

            }
        }
    }
    else                                         //statement is true
    {
        while ((strcmp(elsestat,next_token())!=0) && (strcmp(end,next_token())!=0))
        {
            char  word [strlen(next_token())];
            if (next_token_type == NAME) {
                strcpy(word, next_token());
                istrue = strcmp(word, text);  //text
                if (istrue == 0) {
                    processtext();
                }
                istrue = strcmp (word, output); //output
                if (istrue == 0)
                {
                    processoutput();
                }
                istrue = strcmp (word, comment);  //comment
                if (istrue ==0)
                {
                    skip_line();
                }
                istrue = strcmp (word, var);         //var
                if (istrue ==0)
                {
                    read_next_token();
                    char* varname = new char[strlen(next_token())];
                    strcpy(varname, next_token());
                    int val;
                    read_next_token();
                    if (next_token_type == NUMBER)
                    {
                        val = token_number_value;
                        createvar(varname, val);
                    }
                    else if (next_token_type == SYMBOL)
                    {
                        node* root = treegen();
                        val = processtree(root);
                        createvar(varname, val);

                    }
                    else if (next_token_type == NAME)
                    {
                        char prevvar[strlen(next_token())];
                        strcpy(prevvar, next_token());
                        val = getval(prevvar);
                        createvar(varname, val);

                    }}
                istrue = strcmp (word, set);               //set
                if (istrue == 0)
                {
                    read_next_token();
                    char location [strlen(next_token())];
                    strcpy(location, next_token());
                    read_next_token();
                    if (next_token_type == NUMBER) {
                        int newval = token_number_value;
                        setval(location, newval);
                    }
                    else if (next_token_type == SYMBOL)
                    {
                        node* root = treegen();
                        int newval = processtree(root);
                        setval(location, newval);
                    }
                    else if (next_token_type == NAME)
                    {
                        char prevvar[strlen(next_token())];
                        strcpy(prevvar, next_token());
                        int newval = getval(prevvar);
                        setval(location, newval);
                    }


                }

                istrue = strcmp(word, dostart);
                if (istrue ==0)
                {
                    processdo();
                }
                istrue = strcmp(word, ifstart);
                if (istrue==0 )
                {
                    processifmain();
                }


            }
            read_next_token();
        }
        while(strcmp(end,next_token())!=0)
        {
            read_next_token();
            if (strcmp(ifstart, next_token())==0)
            {
                skipif();
            }
            else if (strcmp(comment, next_token())==0)
            {
                skip_line();
            }
        }
    }

}

void processoutput(void) //processes output
{
    read_next_token();
    if(next_token_type == NUMBER) {
        cout << token_number_value;
    }
    else if (next_token_type == SYMBOL) {
        node *root;
        root = treegen();
        int result = processtree(root);
        cout << result;

    }

    else if (next_token_type==NAME)
    {
        int val;
        char location [strlen(next_token())];
        strcpy(location, next_token());
        val = getval(location);
        cout << val;
    }
}
void run (void)
{
    const char *text = "text";
    const char *output = "output";
    const char *var = "var";
    const char *set = "set";
    const char *comment = "//";
    const char *dostart = "do";
    const char *ifstart = "if";
    int istrue;


    while (next_token_type != END) {
        read_next_token();
        char word[strlen(next_token())];
        if (next_token_type == NAME) {
            strcpy(word, next_token());
            istrue = strcmp(word, text);  //text
            if (istrue == 0) {
                processtext();
            }
            istrue = strcmp (word, output); //output
            if (istrue == 0)
            {
                processoutput();
            }
            istrue = strcmp (word, comment);  //comment
            if (istrue ==0)
            {
                skip_line();
            }
            istrue = strcmp (word, var);         //var
            if (istrue ==0)
            {
                read_next_token();
                char* varname = new char[strlen(next_token())];
                strcpy(varname, next_token());
                int val;
                read_next_token();
                if (next_token_type == NUMBER)
                {
                    val = token_number_value;
                    createvar(varname, val);
                }
                else if (next_token_type == SYMBOL)
                {
                    node* root = treegen();
                    val = processtree(root);
                    deletetree(root);
                    createvar(varname, val);

                }
                else if (next_token_type == NAME)
                {
                    char prevvar[strlen(next_token())];
                    strcpy(prevvar, next_token());
                    val = getval(prevvar);
                    createvar(varname, val);

                }}
                istrue = strcmp (word, set);               //set
            if (istrue ==0)
                {
                    read_next_token();
                    char* location = new char[strlen(next_token())];
                    strcpy(location, next_token());
                    read_next_token();
                    if (next_token_type == NUMBER) {
                        int newval = token_number_value;
                        setval(location, newval);
                    }
                    else if (next_token_type == SYMBOL)
                    {
                        node* root = treegen();
                        int newval = processtree(root);
                        deletetree(root);
                        setval(location, newval);
                    }
                    else if (next_token_type == NAME)
                    {
                        char* prevvar = strcpy(prevvar, next_token());
                        int newval = getval(prevvar);
                        setval(location, newval);
                    }


                }

                istrue = strcmp(word, dostart);
            if (istrue ==0)
            {
                processdo();
            }
                istrue = strcmp(word, ifstart);
            if (istrue==0 )
            {
                processifmain();
            }


        }
        else if (next_token_type == SYMBOL)
        {
            strcpy(word, next_token());
            istrue = strcmp(word, comment);
            if (istrue == 0) {
                skip_line();
            }
        }

    }
}





int main() {
    set_input ("test_grader.blip");
    run();
    return 0;
}