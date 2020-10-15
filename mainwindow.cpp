#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<QMessageBox>
#include<QFile>
#include<QStringList>
#include<QCoreApplication>
#include<QTextStream>
class node{
    public:
        char info;
        std::string Word;
        class node* ptrs[256];
        node(){
            for(int i=0;i<256;i++){
                ptrs[i]=NULL;
            }
            info=NULL;
            Word="";
        }
};
void insertword(std::string word,int pos,class node * root){
    if(word.length()==pos){
        root->Word=word;
        return;
    }
    if( root-> ptrs[word[pos]]==NULL ){
        node *newnode;
        newnode= new node;
        newnode->info=word[pos];
        root->ptrs[word[pos]]=newnode;
        insertword(word,pos+1,root->ptrs[word[pos]]);
    }
    else
        insertword(word,pos+1,root->ptrs[word[pos]]);
}

bool findword(std::string key,int pos, class node * root){
    if((key != root->Word) && (root->ptrs[key[pos]] != NULL))
        return findword(key,pos+1,root->ptrs[key[pos]]);
    else if(key==root->Word){
        return true;
    }
    return false;
}

std::string suggestions[100];
int i = 0;

void printall(class node * root){
    for(int i=0;i<256;i++)
        if(root->ptrs[i]!=NULL){
            printall(root->ptrs[i]);
        }
    if(root->Word != "") {
        suggestions[i] = root->Word;
        i++;
    }
}

void suggest(std::string key,int pos, class node * root){
    if((key != root->Word) && (root->ptrs[key[pos]] != NULL)){
            suggest(key,pos+1,root->ptrs[key[pos]]);
    }
    else{
            printall(root);
    }
    suggestions[i] = "\0";
}

void clearAndReset() {
    for(auto &str : suggestions) {
        str.clear();
    }
    i = 0;
    suggestions[0] = "\0";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString word = ui->lineEditWord->text();
    std::string toSearch = word.toLocal8Bit().constData();
    node *root = new node;
    QFile words("C:\\Users\\Bhupender\\Desktop\\Practice\\applications\\untitled\\wordlist.txt");
    words.open(QIODevice::ReadOnly);
    QString content = words.readAll();
    QStringList wordList = content.split(" ");
    words.close();
    for(const auto &wordAdd : wordList) {
        std::string toAdd = wordAdd.toLocal8Bit().constData();
        insertword(toAdd, 0, root);
    }
    if(findword(toSearch, 0, root) == true) {
        QMessageBox::information(this, "Search", "Word Found!");
    }
    else {
        suggest(toSearch, 0, root);
        int j = 0;
        std::string toprint = "Possible suggestions are: ";
        while(suggestions[j] != "\0") {
            toprint += suggestions[j];
            toprint += ", ";
            j++;
        }
        toprint = toprint.substr(0, toprint.length() - 2);
        QMessageBox::information(this, "Search", QString::fromStdString(toprint));
        clearAndReset();
    }
}
