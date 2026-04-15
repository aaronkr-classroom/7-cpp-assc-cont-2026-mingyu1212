// grammar.cpp
#include <algorithm> // find()
#include <cstdlib> // RAND_MAX 정의
#include <stdexcept> // domain_error
#include <ctime> // time()
// time()의 반환 값은 1970년 1월 1일 00:00 ~ 지금까지의 흐른 시간(UTC)

#include <fstream> // 파일을 읽기 위해
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "C:\Users\com316\Desktop\민규\5-cpp-containers-2026-mingyu1212\Week5\split_str.h" // split_str.h

using namespace std;

typedef vector<string> Rule;
typedef vector<Rule> Rule_collection; // vector<vector<string>>
typedef map<string, Rule_collection> Grammar;
// map<string(키), vector<vector<string>>(값)>

// 주어진 입력 스트림으로 문법 테이블 읽음
Grammar read_grammar(const string& filename) {
	// aaron.kr에 있는 grammar.txt나 grammar-long.txt 필수
	Grammar ret;
	string line;

	ifstream grammar_file(filename); // 수정한 부분

	if (!grammar_file) {
		cerr << "Error opening file: " << filename << endl;
		throw runtime_error("Failed to open file.");
	}

	// 입력 데이터를 읽음
	while (getline(grammar_file, line)) {
		// 입력 데이터를 단어로 나눔
		vector<string> entry = split(line);
		if (!entry.empty()) {
			// 카테고리 및 연관된 규칙을 맵에 저장
			ret[entry[0]].push_back(Rule(entry.begin() + 1, entry.end()));
		}
	}
	return ret;
}

// 꺾쇠호<> 있는지 확인하는 함수
bool bracketed(const string& s) {
	// <....> ? true : false;
	return s.size() > 1
		&& s[0] == '<'
		&& s[s.size() - 1] == '>';
}

//[0,n) 범위에서 임의 정수 하나를 반환
int nrand(int n) { // Python의 random 모듈에서 같은 함수,,
	if (n <= 0 || n > RAND_MAX)
		throw domain_error("nrand out of range!");

	const int bucket_size = RAND_MAX / n;
	int r;
	do r = rand() / bucket_size; // 무조건 한 번 이상 실행
	while (r >= n);
	/*
	do{
		r = rand() / bucket_size;
	} while (r >= n);
	*/
	return r;
}

// 문법 사용하고 문장 생성하기
void gen_aux(const Grammar& g, const string& word, vector<string>& ret) {
	
	if(!bracketed(word)) ret.push_back(word);

	else {	// <...> 있는 단어(문법규칙)
		// word와 연관된 규칙을 찾음
		Grammar::const_iterator it = g.find(word);

		if (it == g.end())
			throw logic_error("Empty rule!");

		// 선택 가능한 규칙을 모아 놓은 벡터를 가져옴
		const Rule_collection& c = it->second;

		// 임의로 하나를 선택
		const Rule& r = c[nrand(c.size())]; // 난수 선택

		// 선택한 규칙을 재귀적으로 확장
		for (Rule::const_iterator i = r.begin(); i != r.end(); i++) {
			gen_aux(g, *i, ret); // 재귀적 호출
		}
	}
}

// 일반적으로 문장 생성하기
vector<string> gen_sent(const Grammar& g) {
	vector<string> ret;
	gen_aux(g, "<sentence>", ret);
	return ret;
}

int main(void) {
	// 문법 파일 읽기... 나중에
	// 현재 시간으로 난수 생성기를 초기화합니다. seed random
	srand(static_cast<unsigned int>(time(0))); // 추가

	// 사용자에게 문법 파일을 선택하도록 요청
	cout << "Choose a grammar file: \n"
		<< "1. grammar.txt\n"
		<< "2. grammar-long.txt\n"
		<< "Enter 1 or 2: ";

	string choice;
	cin >> choice;

	string filename;
	if (choice == "1") {
		filename = "grammar.txt";
	}
	else if (choice == "2") {
		filename = "grammar-long.txt";
	}
	else {
		cout << "Invalid choice! Choose only 1 or 2!" << endl;
		return 1; // 프로그램 종료
	}

	// 문제 없으면, 파일 읽기
	Grammar grammar;
	try {
		grammar = read_grammar(filename);
	}
	catch (const runtime_error& e) {
		cerr << e.what() << endl;
		return 1; // 프로그램 종료
	}

	// 문장 생성
	vector<string> sentence = gen_sent(grammar);

	// 첫번째 단어 출력
	vector<string>::const_iterator it = sentence.begin();
	if (!sentence.empty()) {
		cout << *it;	// 반복자의 가리키는 값을 출력
		++it;			// 반복자를 이동 시킴
	}

	// 공백과 함께 나머지 단어 출력
	while (it != sentence.end()) {
		cout << " " << *it;
		++it;
	}

	cout << endl;

	return 0;
}
