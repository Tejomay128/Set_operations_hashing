#include<iostream>
#include<string>
#include<stdlib.h>
#define MAX 101

using namespace std;

typedef enum{FAILURE,SUCCESS} statuscode;

typedef struct Node_tag         //node in a link list
{
    string str;
    Node_tag *next;
}Node;               

typedef struct hash_table      //hash table definition, chaining concept
{
    Node *lists[MAX];
    int size;
}Set;

/*------------------Link list functions-------------------------*/

Node *createNode(string s)      //creates a link list node
{
    Node *ptr = new Node;
    ptr->str = s;
    ptr->next = NULL;
    return ptr;
}

Node *insertNode(Node *lptr,string s)    //link list insert at start
{
    Node *ptr;
    ptr = createNode(s);
    ptr->next = lptr;
    lptr = ptr;
    return lptr;    
}

statuscode deleteNode(Node **lptr, string s)   //searches and deletes the node, if not found, no operation takes place
{
    statuscode sc = FAILURE;
    Node *ptr,*prev;
    if((*lptr)!=NULL)
    {
        if(s.compare((*lptr)->str)==0)
        {
            sc = SUCCESS;
            ptr = *lptr;
            *lptr = (*lptr)->next;
            delete ptr;
        }
        else
        {
            prev = *lptr;
            ptr = (*lptr)->next;
            bool found = false;
            while(ptr!=NULL && !found)
            {
                if(s.compare(ptr->str)==0)
                {
                    sc = SUCCESS;
                    found = true;
                    prev->next = ptr->next;
                    delete ptr;
                }
                else
                {
                    prev = ptr;
                    ptr = ptr->next;
                }
            }
        }
    }
    return sc;
}

void displayList(Node *lptr)    //displays a link list
{
    Node *iptr = lptr;
    while(iptr!=NULL)
    {
        cout<<"\n"<<iptr->str;
        iptr = iptr->next;
    }
    cout<<"\n";
}

/*------------------Link list functions-------------------------*/

int HashFunction(string str)     //converts a string into respective hash value
{
    long long int add;
    int retval,len;
    len = str.length();
    add = 0;
    for(int i=0;i<len;i++)
    {
        add = add + str[i]*(i+1)*(i+1);
    }
    retval = ((add)*len) % MAX;
    return retval;
}
/*------------------STATIC---------------------------------*/

bool isElementOf(Set h, string s)   //checks whether s in present in Set h
{
    // cout<<"!!!!!!";
    int index = HashFunction(s);
    Node *iptr = h.lists[index];
    bool retval = false;
    while(iptr!=NULL && !retval)
    {
        if(s.compare(iptr->str) == 0)
            retval = true;
        iptr = iptr->next;
    }
    return retval;
}

bool isEmpty(Set set)       //check whether set is empty
{
    bool retval = true;
    for(int i=0;i<MAX && retval;i++)
    {
        if(set.lists[i] != NULL)
            retval = false;
    }
    return retval;
}

int size(Set set)
{
    return set.size;
}

Node *enumerate(Set set)     //returns a single link list comprising of all the elements in set
{
    Node *lptr = NULL;
    Node *iptr;
    for(int i=0;i<MAX;i++)
    {
        iptr = set.lists[i];
        while(iptr!=NULL)
        {
            lptr = insertNode(lptr,iptr->str);
            iptr = iptr->next;
        }
    }
    return lptr;
}

/*------------------STATIC---------------------------------*/

/*------------------DYNAMIC---------------------------------*/

void initSet(Set* h)       //initializes set
{
    for(int i=0;i<MAX;i++)
    {
        h->lists[i] = NULL;
    }
    h->size = 0;
}

Set* create()           //dynamic memory allocation for set
{
    Set* set = (Set*)malloc(sizeof(Set));
    initSet(set);
    return set;
}

void addInSet(Set *h, string s)    //adds a string s in set
{
    // cout<<"@@@@";
    int index;
    if(!isElementOf(*h,s))
    {
        index = HashFunction(s);
        h->lists[index] = insertNode(h->lists[index], s);
        h->size += 1;
    }
}

void removeInSet(Set *set, string s)     //removes a string s from set
{
    int index;
    index = HashFunction(s);
    statuscode sc = deleteNode(&(set->lists[index]),s);
    if(sc == SUCCESS)
        set->size -= 1;
}

/*------------------DYNAMIC---------------------------------*/


Set build(string s[],int n)       //given an array of strings, a set is built from those elements
{
    Set set;
    initSet(&set);
    for(int i=0;i<n;i++)
    {
         addInSet(&set,s[i]);
    }
    return set;
}

Set unionOfSets(Set set1,Set set2)    //returns the union of two sets
{
    Node *iptr;
    Set u;
    initSet(&u);
    u = set2;
    for(int i=0;i<MAX;i++)
    {
        iptr = set1.lists[i];
        while(iptr!=NULL)
        {
            if(!isElementOf(u,iptr->str))
                addInSet(&u,iptr->str);
            iptr = iptr->next;
        }
    }
    return u;
}   

Set intersectionOfSets(Set set1, Set set2)    //returns the intersection of two sets
{
    Set in;
    initSet(&in);
    for(int i=0;i<MAX;i++)
    {
        Node *iptr = set1.lists[i];
        while(iptr!=NULL)
        {
            if(isElementOf(set2,iptr->str))
            {
                addInSet(&in,iptr->str);
            }   
            iptr = iptr->next;
        }
    }
    return in;
}

Set differenceOfSets(Set set1, Set set2)   //returns set1-set2
{
    for(int i=0;i<MAX;i++)
    {
        Node *iptr = set2.lists[i];
        while(iptr!=NULL)
        {
            removeInSet(&set1,iptr->str);
            iptr = iptr->next;
        }
    }
    return set1;
}

bool subset(Set set1,Set set2)      //checks whether set1 is a subset of set2
{
    bool retval = true;
    Node *iptr;
    if(set1.size > set2.size)
        retval = false;
    else    
    {
        for(int i=0;i<MAX;i++)
        {
            iptr = set1.lists[i];
            while(iptr!=NULL && retval)
            {
                if(!isElementOf(set2,iptr->str))
                    retval = false;
                iptr = iptr->next;
            }
        }
    }    
    return retval;
}

void displaySet(Set *set)    //displays the whole set
{
    cout<<"{ ";
    for(int i=0;i<MAX;i++)
    {
        Node *iptr = set->lists[i];
        while(iptr!=NULL)
        {
            cout<<iptr->str<<" ";
            iptr = iptr->next;
        }
    }
    cout<<"} size:"<<set->size<<"\n";
}

int main()
{
    Set set1,set2;
    int m,n,i,opt;
    char choice;
    cout<<"Enter initial size of set 1:";
    cin>>m;
    cout<<"\nEnter initial size of set 2:";
    cin>>n;
    string a[m],b[n];
    cout<<"\nInput for set 1:";
    for(i=0;i<m;i++)
    {
        cin>>a[i];
        cin.ignore();
    }
    cout<<"\nInput for set 2:";
    for(i=0;i<n;i++)
    {
        cin>>b[i];
        cin.ignore();
    }

    set1 = build(a,m);
    set2 = build(b,n);
    displaySet(&set1);
    displaySet(&set2);   

    do{
        cout<<"\nSelect option :\n1)Add a string to set\n2)Remove a string from set\n3)Union\n4)Intersection\n5)difference\n6)Subset\n7)Enumerate\n";
        cout<<"Enter :";
        cin>>opt;
        switch(opt)
        {
            case 1:
                {
                    char no;
                    string str;
                    cout<<"\nEnter set no.(1 or 2): ";
                    cin>>no;
                    cout<<"Enter string:";
                    cin>>str;
                    if(no == '1')
                        addInSet(&set1,str);
                    else if(no == '2')
                        addInSet(&set2,str);
                    else
                        cout<<"\nNot a valid no.\n";   
                    displaySet(&set1);
                    displaySet(&set2);         
                    break;
                }
            case 2:
                {
                    char no;
                    string str;
                    cout<<"\nEnter set no.(1 or 2): ";
                    cin>>no;
                    cout<<"Enter string(String will only be removed if it is present):";
                    cin>>str;
                    if(no == '1')
                        removeInSet(&set1,str);
                    else if(no == '2')
                        removeInSet(&set2,str);
                    else
                        cout<<"\nNot a valid no.";  
                    displaySet(&set1);
                    displaySet(&set2);          
                    break;
                } 
            case 3:
                {
                    Set u;
                    u = unionOfSets(set1,set2);
                    cout<<"\nUnion of sets is: ";
                    displaySet(&u);
                    break;
                }
            case 4:
                {
                    Set in;
                    in = intersectionOfSets(set1,set2);
                    cout<<"\nintersection of sets is: ";
                    displaySet(&in);
                    break;
                }    
            case 5:
                {
                    Set diff;
                    diff = differenceOfSets(set1,set2);
                    cout<<"\nDifference of sets is: ";
                    displaySet(&diff);
                    break;
                }
            case 6:
                {
                    if(subset(set1,set2))
                        cout<<"\nSet1 is a subset of set2";
                    else
                        cout<<"\nSet1 is not a subset of set2";    
                    break;    
                }   
            case 7:
                {
                    Node *lptr1,*lptr2;
                    lptr1 = enumerate(set1);
                    lptr2 = enumerate(set2);
                    cout<<"List from set1 :";
                    displayList(lptr1);
                    cout<<"\nList from set2 :";
                    displayList(lptr2);
                    break;
                } 
            default:
                {
                    cout<<"\nWrong choice!!!";
                    break;
                }   
        }
        cout<<"\nPress Y to continue, N to exit: ";
        cin.ignore();
        cin>>choice;
    }while(choice=='Y' || choice=='y');

    return 0;   
}
