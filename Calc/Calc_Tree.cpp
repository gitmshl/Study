#include<iostream>
#include<stdlib.h>
#include <cmath>

using namespace std;

const double Sum = 922336.01;
const double Mult = 922337.02;
const double Minus = 922338.03;
const double Division = 922339.04;
const double COS = 922339.05;
const double SIN = 922339.06;
const double Err = -1998;


struct node{
    double val;
    node *left;
    node *right;
};

bool First = false;
node *root = NULL;

int min_prior(char *F, int l, int r){
    int i = l, inf = 999999, pr = inf, ind = -1;
    int claster = 0;
    while (i <= r){
        char c = F[i];
        int key = inf+1;
        switch (c){
            case '+': key = 1 + claster*5; break;
            case '-': key = 1 + claster*5; break;
            case '*': key = 2 + claster*5; break;
            case '/': key = 2 + claster*5; break;
            case '(': claster++;  break;
            case ')': claster--; break;
        }
        //std::cout << "ind: " << i << " key: " << key << "\n";
        if (key <= pr) {pr = key; ind = i;}
        i++;
    }
    return ind;
}

double operation(char c){
    if (c == '+') return Sum;
    if (c == '-') return Minus;
    if (c == '*') return Mult;
    if (c == '/') return Division;
}

double Calc(char *F, int l, int r){
    int i = l;
    for ( ; i <= r; i++)
        if (F[i] >= '0' && F[i] <= '9') break;
    int start = i, finish;
    bool flag = false;
    for (i = start + 1; i <= r; i++)
        if (F[i] != 'e' && F[i] != '.' && !(F[i] >= '0' && F[i] <= '9')){
            finish = i-1; flag = true; break;
        }
    if (!flag) finish = r;
    char D[finish-start+1];
    for (i = start; i <= finish; i++)
        D[i-start] = F[i];
    return atof(D);
}

node* Create_tree(char *F, int l, int r){
    node *p = new node;
    if (l > r){
        p->val = 0;
        p->left = p->right = NULL;
        return p;
    }
    int q = min_prior(F, l, r);
    if (q == -1){

        int start = l;
        while (start <= r)
        {
            if (F[start] != '(') break;
            start++;
        }

        if (F[start] == 'c') /// значит, это косинум
        {
            p->val = COS;
            if (!First) {root = p; First = true;}
            p->left = Create_tree(F, start + 3, r);
            p->right = NULL;
            return p;
        }
        else if (F[start] == 's') /// значит, это синус
        {
            p->val = SIN;
            if (!First) {root = p; First = true;}
            p->left = Create_tree(F, start + 3, r);
            p->right = NULL;
            return p;
        }
        p->val = Calc(F, start, r);
        p->left = p->right = NULL;
        if (!First) {root = p; First = true;}
        return p;
    }
     if (!First) {root = p; First = true;}
     p->val = operation(F[q]);
    p->left = Create_tree(F, l, q-1);
    p->right = Create_tree(F, q+1, r);
    return p;

}

double Ans(node *R){
    if (R != NULL){
            double a, b;
            a = Ans(R->left);
            b = Ans(R->right);
            //if  (a != Err) cout << a << " " << b << "\n";
        if (R->val == Sum) return a+b;
        if (R->val == Mult) return a*b;
        if (R->val == Minus) return a-b;
        if (R->val == Division) { if (b) return a/b; cout << "Error: Division on '0'.\n"; return 0;}
        if (R->val == COS) return cos(a);
        if (R->val == SIN) { return sin(a); }
        return R->val;
    } else return Err;
}

void print(node *R){
    if (R != NULL){
        print(R->left);
        if (R->val == Sum) cout << "+ ";
            else if (R->val == Minus) cout << "- ";
        else if (R->val == Mult) cout << "* ";
        else if (R->val == Division) cout << "/ ";
        else
        cout << R->val << " ";
        print(R->right);
    }
}

int Size(char *F){
    int j = 0;
    while (F[j])
        j++;
    return j;
}

int main()
{
    char F[100];
    cin >> F;
   //double d = atof(F);
   //cout << d * 2;
    Create_tree(F, 0, Size(F));
    //print(root);
    cout << Ans(root);
    return 0;
}
