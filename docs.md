# Bookstore 开发文档

written by 陈永杉 

## 程序功能概述

 本次开发主要目标是实现一个模拟的书店管理系统。涉及到需要实现的功能主要有以下几个方面：

### 一、交互功能

#### 基本功能

此项功能为接下来各项功能的应用方式。

要求支持使用命令行进行读入和输出。读入与输出要求遵循以下的规则，并检测错误输出

>  - 指令合法字符集为标准 ASCII 字符
>
>   - 单个指令最大长度为 `1024` 字节
>
>   - 允许的空白符仅为空格，单个指令被空白符分割为多个部分
>
>     - 多个连续空白符效果与一个空白符等价
>     - 行首行末允许出现多余空白符
>     - 如无特殊说明禁止省略或另增空白符
>
>   - 指令中第一个部分必须为指令关键词，指令中关键词部分必须与指令格式完全匹配
>
>   - `[x]` 表示一串有特定限制的用户自定义字符串
>
>   - `(a|b|c)` 表示此处仅能出现 `a`, `b`, `c` 中其一
>
>   - `(x)?` 表示此处可以出现零次或一次 `x`
>
>   - `(x)+` 表示此处可以出现一次或多次 `x`
>
>   - ```css
>     # 基础指令
>     quit
>     exit
>                    
>     # 账户系统指令
>     su [User-ID] ([Password])?
>     logout
>     register [User-ID] [Password] [User-Name]
>     passwd [User-ID] ([Old-Password])? [New-Password]
>     useradd [User-ID] [Password] [Priority] [User-Name]
>     delete [User-ID]
>                    
>     # 图书系统指令
>     show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]")?
>     buy [ISBN] [Quantity]
>     select [ISBN]
>     modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
>     import [Quantity] [Total-Cost]
>                    
>     # 日志系统指令
>     report myself
>     show finance ([Time])?
>     report finance
>     report employee
>     log
>     ```
>
>  - 在用户输入一条指令后，如果合法则执行对应操作，如果有则输出操作结果；如果指令非法或操作失败则输出 `Invalid\n`
>
>   - 仅有空白符的指令合法且无输出内容
>   - 除非有特殊说明，如果输入指令对应的输出内容非空，则结尾有 `\n` 字符；输出内容为空则不输出任何字符
>
>  - `quit` 和 `exit` 指令功能为正常退出系统

#### Bonus 要求

1、支持不同颜色、由数字选择的界面

2、支持中文、emoji 的输入、输出

 $*$ 由于第2项 bonus 功能易与输入正确性检测发生冲突，建议在程序中设计一个可控制该项功能开关的选项，可以集成到第 $1$ 项 bonus 功能之中。

### 账户管理功能

本系统需要实现支持以账户区分不同使用者的功能。具体要求如下：

1、每个账户需要包含的信息有：账户名、账户ID、权限等级、密码、原密码、登录状态

2、每个账户实现以下操作：创建账户、注册账户、登录账户、修改密码、删除账户、注销账户

3、支持多个账户的嵌套登录，当前权限以最后一个账户为准

##### 特殊说明

初始时创建超级账号

程序结束时所有账号都登出

### 图书系统

1、图书信息包含：ISBN 号、图书名、作者名、关键词（可以有多个）、已购买数量、单价、交易总额

2、实现操作：

基于 ISBN /书名/作者名/关键词检索图书

购买一定数量的指定图书

修改图书信息

以一定总额购入指定数量的指定图书

### 日志管理

储存程序的每一笔交易记录

需实现的功能：

1、查询当前账户所有操作记录

2、查询前一定数量交易的财务总额

3、生成财务记录报告

4、生成员工工作情况报告

5、生成程序日志

### 鲁棒性

程序应当对各类的错误输入进行区分，throw 对应的报错并拒绝执行该操作

## 主体逻辑说明

- 在 main 函数中执行一个循环以读入控制台的每一行输入并作出即时处理

- 使用一个子程序以解析输入指令，并进行第一类错误输入的抛出。建议针对输入指令第一词调用不同类型的指令处理函数。（ exit 与 quit 应单写一类处理函数）

- 针对每一类操作，对对应的类执行操作。对于在操作时发现的错误（如找不到对应的ISBN序号等）执行第二类抛出。

- 在 main 函数的 try-catch 部分，针对抛出的内容在控制台输出对应的报错。

- 在每一类的指令处理程序中，通过文件读写来读入对应数据，并将对应更改储存到对应文件之中。

- 有一个类专门用于储存当前的登录状态的嵌套。

## 代码文件结构

函数主要包含：main 函数、输入语句解析函数 (token scanner)、基础类指令处理函数、账户相关指令处理函数、图书相关指令处理函数、日志相关处理函数、错误类型处理函数。

类主要包含：账户类、图书类、日志类、登录状态类、指令语句基类及其派生类。

文件主要包含：账户相关文件组、图书相关文件组、日志相关文件组、使用方法导引文件

#### main 函数：

输出使用方法导引，后开始执行一个 while 循环，循环内使用 try-catch 语句，依次调用 tokenscanner 类、对应的语句处理函数、错误处理函数。应有一个账户类来存储当前最表层的登录账号。

涉及类：无

涉及文件：使用方法导引文件

#### TokenScanner 类
将一整行输入数据读入，再比对字符串头部，由于所有合法指令开头均是给定单词，故可依次进行判断。再将其作为参数和登录状态类一起输入对应的语句处理函数中。

建议全部使用 substr 函数。

涉及类与函数：无

涉及文件：无

#### 基础类指令处理函数

调用登陆状况的记录依次退出账户

涉及类与函数：登录状态类

涉及文件：无

#### 账户相关指令处理函数

执行对应指令，调用账户类，在对应的存储账户相关内容的文件之中进行读写，并同时将登录状态的变更放到登录状态存储文件中，对操作失败进行抛出

比较当前权限能否执行

涉及类：账户类、登录状态类

涉及文件：登录状态存储文件、账户相关文件组

#### 图书相关指令处理程序

执行对应指令，调用图书类，在对应的存储图书相关内容的文件中进行查找与更改，并对员工、顾客相关操作存到相应的日志文件之中。对操作失败进行抛出。

比较当前权限能否执行

涉及类：图书类、账户类、日志类

涉及文件：图书相关文件组、日志相关文件组

#### 日志相关指令处理程序

执行对应指令，调用日志类，对日志相关文件中的内容按照要求进行输出。对操作失败进行抛出。

比较当前权限能否执行

输出日志时希望以每一行包括操作者ID、操作者姓名、操作类型、涉及图书名、涉及金额的格式进行输出。

涉及类：日志类

涉及文件：日志相关文件组

#### 错误类型处理函数

承接错误抛出，对其进行处理

错误分两类：指令语法错误（第一类抛出）、操作执行失败（第二类抛出）

涉及类：无

涉及文件：无

## 各类接口、成员说明

#### 使用方法导引文件

manual

#### 账户相关文件组

account_index : 存储某一账户在Account_store中的位置

account : 存储所有已经注册的账户的相关信息

#### 图书相关文件组

book_index_ISBN : 以ISBN为关键字存储某一图书在 book 中的位置

book_index_author : 以作者名为关键字存储某一图书在 book 中的位置

book_index_name : 以图书名为关键字存储某一图书在 book 中的位置

book_index_keyword : 以关键词为关键字存储某一图书在 book 中的位置

book : 存储所有图书的相关信息

#### 日志相关文件组

log : 存储所有操作日志

#### token scanner 类

```CPP
class TokenScanner {
private:
    std::string _buffer;
    char _delimiter = ' ';
public:
    TokenScanner();

    ~TokenScanner();

    TokenScanner(const string& input, char delimiter = ' ');
    
    std::string nextToken();

    bool hasMoreToken();
};
```



#### 账户类

```cpp
#include <iostream>
#include <fstream>

#include "Unrolled_linklist.h"

struct UserID {
    char[31] value;

    UserID();

    ~UserID();

    UserID(std::string);
    
    bool operator==(const UserID& rhs) const;

    bool operator<(const UserID& rhs) const;
};

struct Account {
public:
    UserID ID;
    char[31] password;
    char[31] name;
    int priority;
};

class AccountGroup {
private:
    UnrolledLinklist<account, UserID, ID_account_map> _id_index;

public:
    AccountGroup();

    ~AccountGroup();

    void registerUser(TokenScanner& line, const LoggingSituation& logStatus);

    void addUser(TokenScanner& line, const LoggingSituation& logStatus);

    void pop(TokenScanner& line, const LoggingSituation& logStatus);

    Account find(std::string& userID);

    bool count(std::string& userID);

    void changePassword(TokenScanner& line, const LoggingSituation& logStatus);

    void clear();
};
```



#### 登陆状态类

```CPP
class LoggingSituation {
private:
    int _logged_num = 0; // 存储已登录账户数目
    std::vector<ID> _logged_in_ID; // 存储已登录的账户ID
    std::vector<int> logged_priority; // 存储已登录的账户对应权限
    std::vector<int> _selected_book_id
public:
    LogSituation();

    ~LogSituation();

    void logIn(std::string logID); // 添加一个已登录账户

    void logOut(); // 退出最后一个登录的账户

    std::string back() const; // 返回此时账号ID

    int backPriority() const;// 返回此时账号权限
};
```




#### $\bullet$ 图书类

```cpp
#include <iostream>
#include <fstream>
#include <vector>
#include "unrolled_linklist.h"
struct ISBN {
    char[21] isbn;

    ISBN(const std::string& isbn_in);

    bool operator==(const ISBN& rhs) const;

    bool operator<(const ISBN& rhs) const;
};

struct Name {
  char[61] name;

  Name(const std::string& name_in);

  bool operator==(const Name& rhs) const;

  bool operator<(const Name& rhs) const;
};

struct Author {
  char[61] author;

  Author(const std::string& author_in);

  bool operator==(const Author&) const;

  bool operator<(const Author&) const;
};

struct Keyword {
  char[61] keyword;

  Keyword(const std::string& ketword_in);

  bool operator==(const Keyword&) const;

  bool operator<(const Keyword&) const;
};

struct Keywords {
  char[61] keywords;

  Keywords(const std::string& ketword_in);

  bool operator==(const Keywords&) const;

  bool operator<(const Keywords&) const;
};

struct Book {
    int id;

    ISBN isbn;

    Name name;

    Author author;

    Keywords keywords;

    int quantity = -1;

    double price = -1;

    double total_cost = -1;

    Book(int id, const std::string& isbn_in, const std::string& name, const std::string& Author, const std::string& keywords, int quantity, double price, double cost);

    Book(int id, const std::string& isbn_in)

    friend str::ostream& operator<<(std::ostream& os, const Book& book);
};

class BookGroup {
private:
    UnrolledLinklist<book_index_ISBN, ISBN, int> ISBN_book_map;

    UnrolledLinklist<book_index_name, Name, int> name_book_map;

    UnrolledLinklist<book_index_Author, Author, int> author_book_map;

    UnrolledLinklist<book_index_keyword, Keyword, int> keywords_book_map;
    
    int all_book_num = 0;

public:
    BookGroup();

    ~BookGroup();

    Book find(TokenScanner& line, const LoggingSituation& loggingStatus, LogGroup& logGroup);

    void change(TokenScanner& line, const LoggingSituation& loggingStatus, LogGroup& logGroup);

    void output(TokenScanner& line, const LoggingSituation& loggingStatus, LogGroup& logGroup);

    void input(TokenScanner& line, const LoggingSituation& loggingStatus, LogGroup& logGroup);

    void select(TokenScanner& line, LoggingSituation& loggingStatus, LogGroup& logGroup);

    void clear();
};
```



#### $\bullet$ 日志类

```cpp
#include <iostream>
#include <fstream>

#include "Unrolled_Linklist.h"

struct log {
    int num;
    bool seal;
    char[31] operateAccount;    
    char[61] bookName;
};

class LogGroup {
private:
    vector<deal> allDeal;
public:
    LogGroup();

    ~LogGroup();

    void report(TokenScanner& line, const LoggingSituation& loggingStatus);

    void add(log& newLog, const loggingSituation& loggingStatus);

    void show(TokenScanner& line, const LoggingSituation& loggingStatus);

    void reportFinance();

    void reportEmployee();

    void clear();
};
```



#### 块链基类

单键：

```cpp
template <class keyType, class valueType>
class UnrolledLinkedList {
private:
    std::fstream _list;
    int _size = 0;
    
public:
    UnrolledLinkedList(const std::string& fileName, int nodeSize = 316);

    ~UnrolledLinkedList();

    void insert(const keyType& key, const valueType& value);

    void pop(const keyType& key);

    void modify(const keyType& key, const valueType& value);

    void clear();

    valueType get(const keyType& key) const;
    
    std::vector<valueType> traverse() const;
};
```



双键：

```cpp
template <class keyType1, class keyType2, class valueType>
class DoubleUnrolledLinkedList {
private:
    std::fstream _list;
    int _size = 0;
    
public:
    DoubleUnrolledLinkedList(const std::string& fileName, int nodeSize = 316);

    ~DoubleUnrolledLinkedList();

    void insert(const keyType1& key1, const keyType2& key2, const valueType& value);

    void pop(const keyType1& key1, const keyType2& key2, const valueType& value);
    
    void modify(const keyType1& key1, const keyType2& key2, const valueType& value);

    void clear();

    valueType get(const keyType1& key1, const keyType2& key2) const;
    
    std::vector<valueType> traverse() const;
    
    std::vector<valueType> traverse(const keyType1& key1) const;
};
```



## 具体算法说明

所有运行过程中产生的信息都以二进制存储在相关的文本文档中

#### 登录状态

直接顺序存储在 LoggingSituation 中，每次操作直接在最后进行修改

#### 账户

所有账户信息在 account 中顺序存储，在 account_index 中以块链形式存储其在 account 中的对应地址。

#### 图书

所有图书信息在 book 中顺序存储，在book_index_ISBN / book_index_name / book_index_author / book_index_keywords 中以块链形式存储对应的 book 在 book_store 中的相应位置。

#### 日志

所有的操作日志在 log 中顺序存储。

## 其他补充说明：关于bonus

#### 满减

为支持中文输入及 emoji 符号，可能需要更改所有类型中的 char[] 等变量类型并且可能会导致自动语法检测失效。

#### 自动化

希望能够支持 Github Actions。

#### 安全第一

可否考虑通过顺序存储每次输入的指令以实现任意时刻的回溯？
