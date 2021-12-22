
# Bookstore-2021
This is an assignment of SJTU ACM class, a bookstore management system. For more information about this assignment, click [here](https://github.com/ACM-Programming-2021/Bookstore-2021).

此爲上海交通大學 ACM 班程式設計大作業之一，一個書店管理系統。如需獲取更多關於此大作業的内容，[點此檢視詳情](https://github.com/ACM-Programming-2021/Bookstore-2021) 。

*P.S.: this project is written with ISOCPP standard.*

*注：此項目使用 ISOCPP 之標準。*



## Abstract 摘要訊息
This program is a bookstore management system using external storage. The system uses unrolled linked list to store the indexes of both book and accounts, accelerating its searching speed.

此程式是一個使用外部磁碟空間之書店管理系統。此系統採用塊狀鏈結串列來儲存書本與賬號數據之索引以加快搜尋速度。



## Functions 功能
This program accepts the following commands.

此系統支援下表所列之指令。

```
# Basic Commands 基礎指令
quit
exit
               
# Commands of the account system 賬戶系統指令
su [User-ID] ([Password])?
logout
register [User-ID] [Password] [User-Name]
passwd [User-ID] ([Old-Password])? [New-Password]
useradd [User-ID] [Password] [Priority] [User-Name]
delete [User-ID]

# Commands of the book system 圖書系統指令
show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]")?
buy [ISBN] [Quantity]
select [ISBN]
modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
import [Quantity] [Total-Cost]

# Commands of the log system 日志系統指令
report myself
show finance ([Time])?
report finance
report employee
log
```
Explanation:

解釋：

* `[A]` means it should be replaced by the corresponding object.
* `[A]` 表示其應被對應之內容替換。
* `(A | B | C)` means there should be only one among A, B and C.
* `(A | B | C)` 表示 A, B, C 中僅可以出現一個，且必須出現一個。
* `(A)?` means A can exist once or not exist.
* `(A)?` 表示 A 僅可出現一次或不出現。
* `(A)+` means A can exist once or more times.
* `(A)+`表示 A 可出現一次或更多次。
