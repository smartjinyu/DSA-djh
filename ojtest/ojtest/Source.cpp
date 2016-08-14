/*
描述
给两个正整数A和B,输出它们的和C = A + B。
输入
共计一行,含两个正整数 A 和 B。
输出
共计一行,含一个正整数,即 A 与 B 之和 C。

输入样例
100 200
输出样例
300
限制
1 <= {a, b} < 10^16
时间：2 sec
空间：256 MB
提示
注意整数类型的范围。
*/

#include <iostream>
using namespace std;
int main() {
	long long a, b,c;
	cin >> a >> b;
	c = a + b;
	cout << c << endl;
	return 0;
}