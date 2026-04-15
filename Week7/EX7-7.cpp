// xref.cpp
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "C:\Users\com316\Desktop\민규\5-cpp-containers-2026-mingyu1212\Week5\split_str.h"

using namespace std;

map<string, vector<int> > xref(
	istream& in,
	vector <string> find_words(const string&) = split)
	{
		string line;
		int line_num = 0;
		map <string, vector<int> > ret;

// 다음 행을 읽음
	while (getline(in, line)) {
		++line_num;

		// 입력한 해을 단어로 나눔.
		vector<string> words = find_words(line);

		// 현재 행에 등장한 모든 단어를 저장
		// 같은 줄에서 같은 단어가 여러 번 나와도 한 번만 line_num 저장
		for (vector<string>::const_iterator it = words.begin();
			it != words.end(); ++it) {
			
			//ret[*it].push_back(line_num);

			vector<int>& lines = ret[*it];

			// 아직 저장된 줄 번호가 없거나,
			// 마지막 저장 줄 번호가 현재 줄 번호와 다를 때 추가
			if (lines.empty()) {
				lines.push_back(line_num);
			}
			else if (lines.back() != line_num) {
				lines.push_back(line_num);
			}
		}
	}
	return ret;
}
/*
	연습 문제: xref.cpp에서
	7-7 (한 행 번호만 출력)
	그리고 한 줄에 단어가 두 번 이상 나타나면
	줄 번호를 여러 번 출력하지 않고 한 번만 출력하세요.
*/
int main(void) {
	// 기본 인수인 plit 함수를 사용하여 xref 함수를 호출
	map<string, vector<int> > ret = xref(cin);

	// 결과를 출력
	for (map<string, vector<int> >::const_iterator it = ret.begin();
		it != ret.end(); ++it) {

		// 단어를 출력
		cout << "\"" << it->first << "\" is on line(s): ";

		// 이어서 하나 이상의 행 번호를 출력
		vector<int>::const_iterator line_it = it->second.begin();

		// 해당 단어가 등장한 첫 번째 행 번호를 출력
		if (line_it != it->second.end()) {
			cout << *line_it;
			++line_it;
		}
		//	cout << *line_it;
		//++line_it;

		// 행 번호가 더 있으면 마저 출력
		while (line_it != it -> second.end()) {
			cout << ", "<< * line_it;
			++line_it;
		}
		// 각 단어를 다음 단어와 구분하려고 새로운 행을 출력
		cout << endl;
	}
	return 0;
}
