#include <iostream>
#include <fstream>
#include <cstring>

#define NUM_OF_LEX 36
#define MAX_STRING_SIZE 255
#define MAX_NUM_OF_ID 100
#define MAX_NUM_OF_STRUCT 10
#define MAX_NUM_OF_GOTO 10
#define SIZE_OF_POLIZ 1000

using namespace std;

//	Отладка
const bool z1 = false;	// Lex
const bool z2 = false;	// TGO
const bool z3 = false;	// POLIZ
const bool z4 = false;	// TST
const bool z5 = false;	// TID
const bool z6 = false;	// TID end



char* newstr(const char* s) {
	if (s == NULL) return NULL;
	char *n = new char[strlen(s) + 1];
	strcpy(n,s);
	return n;
}

char* sumstr(const char* a, const char* b) {
	if (b == NULL) return newstr(a);
	if (a == NULL) return newstr(b);
	int i = strlen(a);
	char *n = new char[i + strlen(b) + 1];
	strcpy(n,a);
	strcpy(n+i,b);
	return n;
}



enum type_of_lex
{
	
	LEX_NULL, 		// 0
	
	LEX_AND,		// 1
	LEX_BOOL,		// 2
	LEX_BREAK,		// 3
	LEX_ELSE,		// 4
	LEX_IF,			// 5
	LEX_FALSE,		// 6
	LEX_FOR,		// 7
	LEX_GOTO,		// 8
	LEX_INT,		// 9
	LEX_NOT,		// 10
	LEX_OR,			// 11
	LEX_PROGRAM,	// 12
	LEX_READ,		// 13
	LEX_STRING,		// 14
	LEX_STRUCT,		// 15
	LEX_TRUE,		// 16
	LEX_WHILE,		// 17
	LEX_WRITE,		// 18
	
	LEX_SEMICOLON,	// 19
	LEX_COMMA,		// 20
	LEX_COLON,		// 21
	LEX_ASSIGN,		// 22
	LEX_LBRACE,		// 23
	LEX_RBRACE,		// 24
	LEX_LPAREN,		// 25
	LEX_RPAREN,		// 26
	LEX_EQ,			// 27
	LEX_LSS,		// 28
	LEX_GTR,		// 29
	LEX_PLUS,		// 30
	LEX_MINUS,		// 31
	LEX_TIMES,		// 32
	LEX_SLASH,		// 33
	LEX_NEQ,		// 34
	LEX_LEQ,		// 35
	LEX_GEQ, 		// 36
	
	LEX_ID,			// 37
	LEX_IDS,		// 38
	LEX_NUM,		// 39
	LEX_STR,		// 40
	LEX_UMINUS, 	// 41
	POLIZ_ADDRESS,	// 42
	POLIZ_GO, 		// 43
	POLIZ_FGO, 		// 44
	LEX_EOF			// 45
	
};

const char * TL[] =
{
	
	"НЕОПОЗНАННАЯ_ЛЕКСЕМА",	// 0
	
	"and",		// 1
	"bool", 	// 2
	"break",	// 3
	"else",		// 4
	"if",		// 5
	"false",	// 6
	"for",		// 7
	"goto",		// 8
	"int",		// 9
	"not",		// 10
	"or",		// 11
	"program",	// 12
	"read",		// 13
	"string",	// 14
	"struct",	// 15
	"true",		// 16
	"while",	// 17
	"write",	// 18

	";",		// 19
	",",		// 20
	":",		// 21
	"=",		// 22
	"{",		// 23
	"}",		// 24
	"(",		// 25
	")",		// 26
	"==",		// 27
	"<",		// 28
	">",		// 29
	"+",		// 30
	"-",		// 31
	"*",		// 32
	"/",		// 33
	"!=",		// 34
	"<=",		// 35
	">=",		// 36
	
	"ИДЕНТИФИКАТОР",			// 37
	"ИДЕНТИФИКАТОР_С_ТОЧКОЙ",	// 38
	"ЧИСЛОВАЯ_КОНСТАНТА",		// 39
	"СТРОКОВАЯ_КОНСТАНТА",		// 40
	"у-",		// 41
	"ADDRESS",	// 42
	"GO",		// 43
	"FGO",		// 44
	"EOF"		// 45
	
};



struct Lex {
	
	type_of_lex type;
	int value;
	char *string;
	
	Lex (type_of_lex t = LEX_NULL, int v = 0) : type(t), value(v), string(NULL) {}
	Lex (type_of_lex t, const char *s) : type(t), value(0) { string = newstr(s); }
	Lex (const Lex & l) {
		type = l.type;
		value = l.value;
		string = newstr(l.string);
	}
	Lex & operator=(const Lex & l) {
		if (this != &l) {
			type = l.type;
			value = l.value;
			if (string != NULL) delete[] string;
			string = newstr(l.string);
		}
		return *this;
	}
	~Lex () {
		if (string != NULL) delete[] string;
	}
	friend ostream& operator << (ostream &s, Lex &l) {
		s << TL[l.type];
	}
	void print () {
		cout << TL[type] << '[' << value;	
		if (string != NULL) cout << "|" << string ;
		cout << ']';
	}
	
};



void err(int n = -1, int i = -1, const char* s = NULL, Lex l = Lex()) {
	
	cout << endl << "ОШИБКА" << endl;
	if (i != -1) cout << "Строка " << i << ": ";
	
	switch (n) {
	
	case 0: cout << "Ошибка открытия файла"; break;
	case 1: cout << "Превышен максимальный размер строки"; break;
	case 2: cout << "Превышено максимально допустимое количество переменных"; break;
	case 3: cout << "Превышено максимально допустимое количество структур"; break;
	case 4: cout << "Превышено максимально допустимое количество меток"; break;
	case 5: cout << "ПОЛИЗ переполнен"; break;
	case 6: cout << "Попытка деления на ноль"; break;
	case 7: cout << "Тип константы не соответствует типу переменной"; break;
	case 8: cout << "Попытка присвоить значение структуре"; break;
	case 9: cout << "Тип второго выражения в for должен быть bool"; break;
	case 10: cout << "Тип выражения в while должен быть bool"; break;
	case 11: cout << "Тип выражения в if должен быть bool"; break;
	case 12: cout << "break вне цикла"; break;
	case 13: cout << "Слева от = не переменная"; break;
	case 14: cout << "Попытка присвоения переменной значения другого типа"; break;
	case 15: cout << "Попытка присвоения переменной другой структуры"; break;
	case 16: cout << "Идентификатор " << s << " уже описан"; break;
	case 17: cout << "Идентификатор " << s << " не описан"; break;
	case 18: cout << "Метка " << s << " не описана"; break;
	case 19: cout << "Значение " << s << " не определено"; break;
	case 20: cout << "Значения разных типов при операции " << l; break;
	case 21: cout << "Ошибка в записи выражения, получено " << l; break;
	case 22: cout << "Операция " << l << " применима только к типу " << s; break;
	case 23: cout << "Ожидалось " << s << " ,но получено " << l; break;
	default: cout << "Непредвиденная ошибка работы интерпретатора";
	
	}
	
	cout << endl;
	throw n;
	
}



class Scanner {
	
	enum state { H, IDENT, NUMB, STR, CMP, COM };
	FILE * fp;
	char c;
	char buf[MAX_STRING_SIZE + 1];
	int buf_top, count;
	
	void clear () { buf_top = 0; *buf = '\0'; }
	void add () {
		if (buf_top > MAX_STRING_SIZE - 1) err(1);
		buf[buf_top] = c;
		buf[++buf_top] = '\0';
	}
	type_of_lex search_buf () {
		for (int i = 1; i <= NUM_OF_LEX; ++i)
			if (!strcmp(buf, TL[i])) return (type_of_lex) i;
		return LEX_NULL;
	}
	void gc () { c = fgetc (fp); }
	
	public:
	Scanner (const char * program) : count(1) {
		fp = fopen (program, "r");
		if (fp == NULL) err(0);
		clear();
		gc();
	}
	~Scanner () { fclose(fp); }
	
	int get_count() { return count; }
	
	Lex get_lex () {
		int d;
		bool p;
		type_of_lex t;
		state CS = H;
		while (true) {
			switch (CS) {
			
			case H:
				if (c == EOF) return Lex(LEX_EOF);
				if (c == '\t' || c == ' ' || c== '\r') { if (z1) cout << c; gc(); break; }
				if (c == '\n') { ++count; if (z1) cout << c; gc(); break; }
				if (isalpha(c) || c == '_') {
					clear();
					add();
					gc();
					CS = IDENT;
					p = false;
					break;
				}
				if (isdigit(c)) {
					d = c - '0';
					gc();
					CS = NUMB;
					break;
				}
				if (c == '"') {
					clear();
					gc();
					CS = STR;
					break;
				}
				if (c == '<' || c == '>' || c == '=' || c == '!') {
					clear();
					add();
					gc();
					CS = CMP;
					break;
				}
				clear();
				add();
				gc();
				if (*buf == '/' && c == '*') {
					gc();
					CS = COM;
					break;
				}
				return Lex(search_buf());
			
			case IDENT:
				if (isalpha(c) || isdigit(c) || c == '_' || c == '.') {
					if (c == '.') p = true;
					add();
					gc();
					break;
				}
				if (p) return Lex(LEX_IDS, buf);
				if ((t = search_buf()) != LEX_NULL) return Lex(t);
				return Lex(LEX_ID, buf);
				break;
			
			case NUMB:
				if (isdigit(c)) {
					d = d * 10 + (c - '0');
					gc();
				} else return Lex(LEX_NUM, d);
				break;
			
			case STR:
				if (c == EOF) err(23,count,"\"",Lex(LEX_EOF));
				if (c == '\n') ++count;
				if (c == '"') {
					gc();
					return Lex(LEX_STR, buf);
					break;
				}
				if (buf[buf_top-1] == '\\') {
					switch (c) {
					case 'n': buf[buf_top-1] = '\n'; break;
					case 'r': buf[buf_top-1] = '\r'; break;
					case 't': buf[buf_top-1] = '\t'; break;
					default: add();
					}
					gc();
					break;
				}
				add();
				gc();
				break;
			
			case CMP:
				if (c == '=') {
					add();
					gc();
				}
				return Lex(search_buf());
				break;
			
			case COM:
				if (c == EOF) err(23,count,"*/",Lex(LEX_EOF));
				if (c == '\n') ++count;
				if (*buf == '*' && c == '/') {
					gc();
					CS = H;
					break;
				}
				*buf = c;
				gc();
				break;
			
			}
		}
	}
	
};



class List_dec {
	
	struct block {
		char *name;
		Lex ident;
		bool flag;
		block *next;
	};
	
	block *b;
	
	public:
	List_dec () : b(NULL) {}
	~List_dec () {
		block *p = b;
		while (b != NULL) {
			p = b->next;
			delete[] b->name;
			delete b;
			b = p;
		}
	}
	List_dec & operator=(List_dec & l) {
		if (this != &l) {
			b = l.b;
			l.b = NULL;
		}
		return *this;
	}
	void put (Lex &, Lex &, Lex);
	void dec (const char*);
	void print () {
		block *p = b;
		while (p != NULL) {
			cout << endl << '\t' << p->name << '\t' << p->flag << "   ";
			(p->ident).print();
			p = p->next;
		}
	}
	
};



template <class T, int max_size > class Stack {
	T s[max_size];
	int top;
	
	public:
	Stack() : top(0) {}
	void reset() { top = 0; }
	void push(T i) {
		if (!is_full()) {
			s[top] = i;
			++top;
		} else err();
	}
	T pop() {
		if (!is_empty()) {
			--top;
			return s[top];
		} else err();
	} 
	void print() {
		for (int i = 0; i < top; ++i)
			cout << s[i] << ' ';
	}
	bool is_empty() { return top == 0; }
	bool is_full() { return top == max_size; }
};



template <class A, class B, B s, int E> class Tabl {
	
	A *a;
	B *b;
	char **name;
	int size;
	int top;
	
	public:
	Tabl (int max_size) : size(max_size), top(0) {
		a = new A[size];
		b = new B[size];
		name = new char*[size];
	}
	~Tabl () {
		delete[] a;
		delete[] b;
		for (int i = 0; i < top; ++i)
			if (name[i] != NULL) delete[] name[i];
		delete[] name;
	}
	A & operator[] (int i) { return a[i]; }
	int search (const char *n) {
		for (int i = 0; i < top; ++i)
			if (!strcmp(n, name[i])) return i;
		return -1;
	}
	int put (const char *n) {
		int i;
		if ((i = search(n)) != -1) return i;
		if (top >= size) err(E);
		name[top] = newstr(n);
		b[top] = s;
		return top++;
	}
	void set (int i, B x) { b[i] = x; }
	B get (int i) { return b[i]; }
	char* get_name (int i) { return name[i]; }
	int get_size () { return top; }
	void print () {
		for (int i = 0; i < top; ++i) {
			cout << i << '\t' << name[i] << '\t' << b[i] << "   ";
			a[i].print();
			cout << endl;
		}
	}
	
};



Tabl < Lex, bool, false, 2 > TID(MAX_NUM_OF_ID);
Tabl < List_dec, int, 0, 3 > TST(MAX_NUM_OF_STRUCT);
Tabl < Stack < int, MAX_NUM_OF_GOTO >, int, -1, 4 > TGO(MAX_NUM_OF_GOTO);



bool id_is_dec (const char* n) {
	
	if (TID.search(n) != -1) return true;
	if (TST.search(n) != -1) return true;
	int i;
	if ((i = TGO.search(n)) != -1)
		if (TGO.get(i) != -1) return true;
	return false;
	
}



void List_dec::put (Lex &T, Lex &ID, Lex H = Lex()) {
		block *p = b;
		b = new block;
		b->name = newstr(ID.string);
		b->next = p;
		if (T.type == LEX_ID) {
			(b->ident).type = LEX_STRUCT;
			(b->ident).value = TST.search(T.string);
			b->flag = true;
			return;
		}
		b->ident = T;
		switch (H.type) {
		case LEX_NUM:
			(b->ident).value = H.value; break;
		case LEX_STR:
			(b->ident).string = newstr(H.string); break;
		case LEX_TRUE:
			(b->ident).value = 1; break;
		case LEX_FALSE:
			(b->ident).value = 0; break;
		default:
			b->flag = false;
			return;
		}
		b->flag = true;
}

void List_dec::dec (const char* str = NULL) {
	
	char *buf = new char[MAX_STRING_SIZE + 1];
	int i = 0, j, k;
	if (str != NULL) { 
		i = strlen(str);
		strcpy(buf, str);
		buf[i] = '.';
		i++;
	}
	block *p = b;
	while (p != NULL) {
		strcpy(buf+i, p->name);
		if (id_is_dec(buf)) err(16,-1,buf);
		j = TID.put(buf);
		TID[j] = p->ident;
		TID.set(j, p->flag);
		if (TID[j].type == LEX_STRUCT) {
			j = TID[j].value;
			TST[j].dec(buf);
		}
		p = p->next;
	}
	delete[] buf;
	
}



class Poliz {
	Lex *p;
	int size;
	int top;
	
	public:
	Poliz (int max_size) : size(max_size), top(0) {
		p = new Lex[size];
	}
	~Poliz () { delete []p; }
	Lex & operator[] (int k) { return p[k]; }
	int put (const Lex & l) { 
		if (top >= size) err(5);
		p[top] = l;
		return top++;
	}
	int get () { return top; }
	void address () { p[top-1].type = POLIZ_ADDRESS; }
	void print () {
		for (int i = 0; i < top; ++i) {
			cout << i << '\t'; 
			p[i].print();
			cout << endl;
		}
	}
};



class Parser {
	
	Scanner scan;
	Lex lex, e_info;
	int loops, n;
	bool ident;
	
	void P();
	void C();
	void D(List_dec &);
	void S(int);
	void E();
	void E1();
	void E2();
	void E3();
	void E4();
	void T();
	void M();
	void F();
	
	int size_of_type();
	void fill_goto (Poliz &);
	
	void gl () {
		lex = scan.get_lex();
		if (z1) lex.print();
	}
	
	public:
	Poliz prog;
	Parser (const char *program) : scan(program), prog(SIZE_OF_POLIZ), loops(0) {}
	void analyze ();
	
};

void Parser::analyze () {
	gl();
	P();
	if (z2) cout << endl << "----------TGO----------" << endl;
	if (z2) TGO.print();
	fill_goto(prog);
	if (z3) cout << endl << "---------POLIZ---------" << endl;
	if (z3) prog.print();
	if (z4) cout << endl << "----------TST----------" << endl;
	if (z4) TST.print();
	if (z5) cout << endl << "----------TID----------" << endl;
	if (z5) TID.print();
}

void Parser::P () {
	
	if (lex.type != LEX_PROGRAM) err(23, scan.get_count(), TL[LEX_PROGRAM], lex);
	gl();
	if (lex.type != LEX_LBRACE) err(23, scan.get_count(), TL[LEX_LBRACE], lex);
	gl();
	C();
	if (lex.type != LEX_RBRACE) err(23, scan.get_count(), TL[LEX_RBRACE], lex);
	
}

void Parser::C () {
	
	List_dec LD;
	int i;
	while (lex.type != LEX_RBRACE) {
		if (size_of_type() != -1) {
			D(LD);
			LD.dec();
			LD.~List_dec();
			continue;
		}
		if (lex.type == LEX_STRUCT) {
			gl();
			if (lex.type != LEX_ID) err(23, scan.get_count(), TL[LEX_ID], lex);
			if (id_is_dec(lex.string)) err(16, scan.get_count(), lex.string);
			i = TST.put(lex.string);
			gl();
			if (lex.type != LEX_LBRACE) err(23, scan.get_count(), TL[LEX_LBRACE], lex);
			gl();
			D(LD);
			TST[i] = LD;
			TST.set(i,n);
			if (lex.type != LEX_RBRACE) err(23, scan.get_count(), TL[LEX_RBRACE], lex);
			gl();
			if (lex.type != LEX_SEMICOLON) err(23, scan.get_count(), TL[LEX_SEMICOLON], lex);
			gl();
			continue;
		}
		S(-1);
	}
	
}

void Parser::D (List_dec & L) {
	
	Lex t, id;
	int i, j;
	n = 0;
	while ((i = size_of_type()) != -1) {
		t = lex;
		j = 0;
		do {
			++j;
			gl();
			if (lex.type != LEX_ID) err(23, scan.get_count(), TL[LEX_ID], lex);
			id = lex;
			gl();
			if (lex.type == LEX_ASSIGN) {
				gl();
				switch (t.type) {
				case LEX_INT:
					switch (lex.type) {
					case LEX_NUM: break;
					case LEX_PLUS:
						gl(); 
						if (lex.type != LEX_NUM) err(23, scan.get_count(), TL[LEX_ID], lex);
						break;
					case LEX_MINUS:
						gl();
						if (lex.type != LEX_NUM) err(23, scan.get_count(), TL[LEX_ID], lex);
						lex.value = -lex.value;
						break;
					default: err(7, scan.get_count());
					}
					break;
					
				case LEX_STRING:	
					if (lex.type != LEX_STR) err(7, scan.get_count());
					break;
					
				case LEX_BOOL:	
					if (lex.type != LEX_TRUE && lex.type != LEX_FALSE) err(7, scan.get_count());
					break;
					
				default: err(8, scan.get_count());
				}
				L.put(t,id,lex);
				gl();
			} else L.put(t,id);
		} while (lex.type == LEX_COMMA);
		if (lex.type != LEX_SEMICOLON) err(23, scan.get_count(), TL[LEX_SEMICOLON], lex);
		gl();
		n += i*j;
	}
	
}

void Parser::S (int k) {
	
	int i, j;
	char *s;
	
	switch (lex.type) {
		
	case LEX_IF:
		gl();
		if (lex.type != LEX_LPAREN) err(23, scan.get_count(), TL[LEX_LPAREN], lex);
		gl();
		E();
		if (lex.type != LEX_RPAREN) err(23, scan.get_count(), TL[LEX_RPAREN], lex);
		if (e_info.type != LEX_BOOL) err(11, scan.get_count());
		i = prog.put(Lex(POLIZ_FGO));
		gl();
		S(k);
		if (lex.type != LEX_ELSE) err(23, scan.get_count(), TL[LEX_ELSE], lex);
		j = prog.put(Lex(POLIZ_GO));
		prog[i].value = j + 1;
		gl();
		S(k);
		prog[j].value = prog.get();
		break;
		
	case LEX_FOR:
		gl();
		if (lex.type != LEX_LPAREN) err(23, scan.get_count(), TL[LEX_LPAREN], lex);
		gl();
		if (lex.type != LEX_SEMICOLON) {
			E();
			if (lex.type != LEX_SEMICOLON) err(23, scan.get_count(), TL[LEX_SEMICOLON], lex);
		}
		i = prog.get();
		gl();
		if (lex.type != LEX_SEMICOLON) {
			E();
			if (lex.type != LEX_SEMICOLON) err(23, scan.get_count(), TL[LEX_SEMICOLON], lex);
			if (e_info.type != LEX_BOOL) err(9, scan.get_count());
		} else prog.put(Lex(LEX_BOOL, 1));
		j = prog.put(Lex(POLIZ_FGO));
		prog.put(Lex(POLIZ_GO));
		gl();
		if (lex.type != LEX_RPAREN) {
			E();
			if (lex.type != LEX_RPAREN) err(23, scan.get_count(), TL[LEX_RPAREN], lex);
		}
		i = prog.put(Lex(POLIZ_GO, i));
		prog[j+1].value = ++i;
		s = new char[10];
		sprintf(s, "#F_%d", loops++);
		k = TGO.put(s);
		delete[] s;
		gl();
		S(k);
		i = prog.put(Lex(POLIZ_GO, j+2));
		prog[j].value = ++i;
		TGO.set(k, i);
		break;
		
	case LEX_WHILE:
		gl();
		if (lex.type != LEX_LPAREN) err(23, scan.get_count(), TL[LEX_LPAREN], lex);
		i = prog.get();
		gl();
		E();
		if (lex.type != LEX_RPAREN) err(23, scan.get_count(), TL[LEX_RPAREN], lex);
		if (e_info.type != LEX_BOOL) err(10, scan.get_count());
		j = prog.put(Lex(POLIZ_FGO));
		s = new char[10];
		sprintf(s, "#W_%d", loops++);
		k = TGO.put(s);
		delete[] s;
		gl();
		S(k);
		i = prog.put(Lex(POLIZ_GO, i));
		TGO.set(k, ++i);
		prog[j].value = i;
		break;
		
	case LEX_BREAK:
		if (k == -1) err(12, scan.get_count());
		gl();
		if (lex.type != LEX_SEMICOLON) err(23, scan.get_count(), TL[LEX_SEMICOLON], lex);
		gl();
		i = prog.put(Lex(POLIZ_GO));
		TGO[k].push(i);
		break;
		
	case LEX_GOTO:
		gl();
		if (lex.type != LEX_ID) err(23, scan.get_count(), TL[LEX_ID], lex);
		i = TGO.put(lex.string);
		j = prog.put(Lex(POLIZ_GO));
		TGO[i].push(j);
		gl();
		if (lex.type != LEX_SEMICOLON) err(23, scan.get_count(), TL[LEX_SEMICOLON], lex);
		gl();
		break;
		
	case LEX_READ:
		gl();
		if (lex.type != LEX_LPAREN) err(23, scan.get_count(), TL[LEX_LPAREN], lex);
		gl();
		if (lex.type != LEX_ID && lex.type != LEX_IDS) err(23, scan.get_count(), TL[LEX_ID], lex);
		if ((i = TID.search(lex.string)) == -1) err(17, scan.get_count(), lex.string);
		if (TID[i].type == LEX_STRUCT) err(8, scan.get_count());
		prog.put(Lex(POLIZ_ADDRESS,i));
		prog.put(Lex(LEX_READ));
		gl();
		if (lex.type != LEX_RPAREN) err(23, scan.get_count(), TL[LEX_RPAREN], lex);
		gl();
		if (lex.type != LEX_SEMICOLON) err(23, scan.get_count(), TL[LEX_SEMICOLON], lex);
		gl();
		break;
		
	case LEX_WRITE:
		gl();
		if (lex.type != LEX_LPAREN) err(23, scan.get_count(), TL[LEX_LPAREN], lex);
		do {
			gl();
			E();
			prog.put(Lex(LEX_WRITE));
		} while (lex.type == LEX_COMMA);
		if (lex.type != LEX_RPAREN) err(23, scan.get_count(), TL[LEX_RPAREN], lex);
		gl();
		if (lex.type != LEX_SEMICOLON) err(23, scan.get_count(), TL[LEX_SEMICOLON], lex);
		gl();
		break;
		
	case LEX_LBRACE:
		gl();
		while (lex.type != LEX_RBRACE) S(k);
		gl();
		break;
		
	case LEX_ID:
		if (!id_is_dec(lex.string)) {
			s = newstr(lex.string);
			i = TGO.put(s);
			j = prog.get();
			TGO.set(i, j);
			gl();
			if (lex.type != LEX_COLON) err(17, scan.get_count(), s);
			delete[] s;
			gl();
			S(k);
			break;
		}
		
	case LEX_IDS:
	case LEX_PLUS:
	case LEX_MINUS:
	case LEX_NUM:
	case LEX_STR:
	case LEX_FALSE:
	case LEX_TRUE:
	case LEX_NOT:
	case LEX_LPAREN:
		E();
		if (lex.type != LEX_SEMICOLON) err(23, scan.get_count(), TL[LEX_SEMICOLON], lex);
		gl();
		break;
		
	default:
		err(23, scan.get_count(), "ОПЕРАТОР", lex);
		
	}
	
}

void Parser::E () {
	
	E1();
	Lex l = e_info;
	if (lex.type == LEX_ASSIGN) {
		if (!ident) err(13, scan.get_count());
		prog.address();
		gl();
		E();
		if (e_info.type != l.type) err(14, scan.get_count());
		if (e_info.type == LEX_STRUCT) {
			prog.address();
			if (e_info.value != l.value) err(15, scan.get_count());
		} 
		prog.put(Lex(LEX_ASSIGN));
	}
	
}

void Parser::E1 () {
	
	E2();
	if (lex.type == LEX_OR && e_info.type != LEX_BOOL) err(22, scan.get_count(), "bool", Lex(LEX_OR));
	while (lex.type == LEX_OR) {
		gl();
		E2();
		if (e_info.type != LEX_BOOL) err(22, scan.get_count(), "bool", Lex(LEX_OR));
		prog.put(Lex(LEX_OR));
		ident = false;
	}
	
}

void Parser::E2 () {
	
	E3();
	if (lex.type == LEX_AND && e_info.type != LEX_BOOL) err(22, scan.get_count(), "bool", Lex(LEX_AND));
	while (lex.type == LEX_AND) {
		gl();
		E3();
		if (e_info.type != LEX_BOOL) err(22, scan.get_count(), "bool", Lex(LEX_AND));
		prog.put(Lex(LEX_AND));
		ident = false;
	}
	
}

void Parser::E3 () {
	
	E4();
	type_of_lex e = e_info.type, t = lex.type;
	switch (t) {
	case LEX_LSS:
	case LEX_GTR:
	case LEX_EQ:
	case LEX_NEQ:
		if (e != LEX_INT && e != LEX_STRING) err(22, scan.get_count(), "int и string", Lex(t));
		gl();
		E4();
		if (e_info.type != e) err(20, scan.get_count(), NULL, Lex(t));
		prog.put(Lex(t));
		e_info.type = LEX_BOOL;
		ident = false;
		break;
	case LEX_LEQ:
	case LEX_GEQ:
		if (e != LEX_INT) err(22, scan.get_count(), "int", Lex(t));
		gl();
		E4();
		if (e_info.type != e) err(20, scan.get_count(), NULL, Lex(t));
		prog.put(Lex(t));
		e_info.type = LEX_BOOL;
		ident = false;
		break;
	}
	
}

void Parser::E4 () {
	
	T();
	type_of_lex e = e_info.type, t;
	while (lex.type == LEX_PLUS || lex.type == LEX_MINUS) { 
		t = lex.type;
		switch (t) {
		case LEX_PLUS:
			if (e != LEX_INT && e != LEX_STRING) err(22, scan.get_count(), "int и string", Lex(t));
			break;
		case LEX_MINUS:
			if (e != LEX_INT) err(22, scan.get_count(), "int", Lex(t));
			break;
		}
		gl();
		T();
		if (e_info.type != e) err(20, scan.get_count(), NULL, Lex(t));
		prog.put(Lex(t));
		ident = false;		
	}
	
}

void Parser::T () {
	
	M();
	type_of_lex e = e_info.type, t;
	while (lex.type == LEX_TIMES || lex.type == LEX_SLASH) {
		t = lex.type;
		if (e != LEX_INT) err(22, scan.get_count(), "int", Lex(t));
		gl();
		M();
		if (e_info.type != e) err(20, scan.get_count(), NULL, Lex(t));
		prog.put(Lex(t));
		ident = false;
	}
	
}

void Parser::M () {
	
	switch (lex.type) {
	case LEX_PLUS:
		gl();
		F();
		if (e_info.type != LEX_INT) err(22, scan.get_count(), "int", Lex(LEX_PLUS));
		ident = false;
		break;
	case LEX_MINUS:
		gl();
		F();
		if (e_info.type != LEX_INT) err(22, scan.get_count(), "int", Lex(LEX_MINUS));
		prog.put(Lex(LEX_UMINUS));
		ident = false;
		break;
	default:
		F();
	}
	
}

void Parser::F () {
	
	int i;
	char *s;
	switch (lex.type) {
	
	case LEX_ID:
	case LEX_IDS:
		s = newstr(lex.string);
		if ((i = TID.search(s)) == -1) err(17, scan.get_count(), s);
		prog.put(Lex(LEX_ID,i));
		e_info = TID[i];
		ident = true;
		gl();
		if (lex.type == LEX_COLON) err(16, scan.get_count(), s);
		delete[] s;
		break;
		
	case LEX_NUM:
		prog.put(Lex(LEX_INT,lex.value));
		e_info.type = LEX_INT;
		ident = false;
		gl();
		break;
		
	case LEX_STR:
		prog.put(Lex(LEX_STRING,lex.string));
		e_info.type = LEX_STRING;
		ident = false;
		gl();
		break;
	
	case LEX_FALSE:
		prog.put(Lex(LEX_BOOL,0));
		e_info.type = LEX_BOOL;
		ident = false;
		gl();
		break;
		
	case LEX_TRUE:
		prog.put(Lex(LEX_BOOL,1));
		e_info.type = LEX_BOOL;
		ident = false;
		gl();
		break;
	
	case LEX_NOT:
		gl();
		F();
		if (e_info.type != LEX_BOOL) err(22, scan.get_count(), "bool", Lex(LEX_NOT));
		prog.put(Lex(LEX_NOT));
		ident = false;
		break;
	
	case LEX_LPAREN:
		gl();
		E();
		ident = false;
		if (lex.type != LEX_RPAREN) err(23, scan.get_count(), TL[LEX_RPAREN], lex);
		gl();
		break;
	
	default: err(21, scan.get_count(), NULL, lex);
	}
	
}

int Parser::size_of_type () {
	
	if (lex.type == LEX_INT || lex.type == LEX_STRING || lex.type == LEX_BOOL) return 1;
	if (lex.type != LEX_ID) return -1;
	int i = TST.search(lex.string);
	if (i == -1) return -1;
	return TST.get(i) + 1;
	
}

void Parser::fill_goto (Poliz & p) {
	
	int j, s = TGO.get_size();
	for (int i = 0; i < s; ++i) {
		if (TGO.get(i) == -1) err(18, -1, TGO.get_name(i));
		while (!TGO[i].is_empty()) {
			j = TGO[i].pop();
			p[j].value = TGO.get(i);
		}
	}
	
}



void execute (Poliz & prog) {
	Stack <Lex, SIZE_OF_POLIZ> args;
	Lex l1, l2;
	int i = 0, j, x, y, size = prog.get();
	char buf[MAX_STRING_SIZE];
	
	cout << "Выполнение..." << endl;
	while (i < size) {
		switch(prog[i].type) {
			
		case LEX_INT:
		case LEX_STRING:
		case LEX_BOOL:
		case POLIZ_ADDRESS:
			args.push(prog[i]);
			break;
			
		case LEX_ID:
			j = prog[i].value;
			if (!TID.get(j)) err(19, -1, TID.get_name(j));
			args.push(TID[j]);
			break;
			
		case LEX_NOT:
			l1 = args.pop();
			j = !l1.value;
			args.push(Lex(LEX_BOOL,j));
			break;
			
		case LEX_OR:
			l2 = args.pop();
			l1 = args.pop();
			j = (l1.value || l2.value);
			args.push(Lex(LEX_BOOL,j));
			break;
			
		case LEX_AND:
			l2 = args.pop();
			l1 = args.pop();
			j = (l1.value && l2.value);
			args.push(Lex(LEX_BOOL,j));
			break;
			
		case LEX_PLUS:
			l2 = args.pop();
			l1 = args.pop();
			if (l1.type == LEX_STRING) args.push(Lex(LEX_STRING, sumstr(l1.string, l2.string)));
				else args.push(Lex(LEX_INT, l1.value + l2.value));
			break;
			
		case LEX_MINUS:
			l2 = args.pop();
			l1 = args.pop();
			args.push(Lex(LEX_INT, l1.value - l2.value));
			break;
			
		case LEX_UMINUS:
			l1 = args.pop();
			j = -l1.value;
			args.push(Lex(LEX_INT,j));
			break;
			
		case LEX_TIMES:
			l2 = args.pop();
			l1 = args.pop();
			args.push(Lex(LEX_INT, l1.value * l2.value));
			break;
			
		case LEX_SLASH:
			l2 = args.pop();
			l1 = args.pop();
			if (!l2.value) err(6);
			args.push(Lex(LEX_INT, l1.value / l2.value));
			break;
			
		case LEX_EQ:
			l2 = args.pop();
			l1 = args.pop();
			if (l1.type == LEX_STRING) args.push(Lex(LEX_BOOL, !strcmp(l1.string, l2.string)));
				else args.push(Lex(LEX_BOOL, l1.value == l2.value));
			break;
			
		case LEX_NEQ:
			l2 = args.pop();
			l1 = args.pop();
			if (l1.type == LEX_STRING) args.push(Lex(LEX_BOOL, strcmp(l1.string, l2.string)));
				else args.push(Lex(LEX_BOOL, l1.value != l2.value));
			break;
			
		case LEX_LSS:
			l2 = args.pop();
			l1 = args.pop();
			if (l1.type == LEX_STRING) args.push(Lex(LEX_BOOL, strcmp(l1.string, l2.string) < 0 ));
				else args.push(Lex(LEX_BOOL, l1.value < l2.value));
			break;
			
		case LEX_GTR:
			l2 = args.pop();
			l1 = args.pop();
			if (l1.type == LEX_STRING) args.push(Lex(LEX_BOOL, strcmp(l1.string, l2.string) > 0 ));
				else args.push(Lex(LEX_BOOL, l1.value > l2.value));
			break;
			
		case LEX_LEQ:
			l2 = args.pop();
			l1 = args.pop();
			args.push(Lex(LEX_BOOL, l1.value <= l2.value));
			break;
			
		case LEX_GEQ:
			l2 = args.pop();
			l1 = args.pop();
			args.push(Lex(LEX_BOOL, l1.value >= l2.value));
			break;
			
		case LEX_WRITE:
			l1 = args.pop();
			switch(l1.type) {
			case LEX_INT: cout << l1.value; break;
			case LEX_STRING: cout << l1.string; break;
			case LEX_BOOL: if (l1.value) cout << "true"; else cout << "false";
			}
			args.reset();
			break;
			
		case LEX_READ:
			l1 = args.pop();
			j = l1.value;
			cout << endl;
			switch(TID[j].type) {
			case LEX_INT:
				cout << "Введите значение для " << TID.get_name(j) << " (int): ";
				cin >> x;
				TID[j].value = x;
				TID.set(j,true);
				break;
			case LEX_STRING:
				cout << "Введите значение для " << TID.get_name(j) << " (string): ";
				buf[x = 0] = getchar(); y = 0;
				while ((buf[x] != '\n') || y) {
					++x;
					if (x >= MAX_STRING_SIZE) err(1);
					buf[x] = getchar();
					y = 0;
					if (buf[x-1] == '\\')
						switch (buf[x]) {
						case 'n': buf[--x] = '\n'; y = 1; break;
						case 'r': buf[--x] = '\r'; break;
						case 't': buf[--x] = '\t';
						}
				}
				buf[x] = '\0';
				if (TID.get(j)) delete[] TID[j].string;
				TID[j].string = newstr(buf);
				TID.set(j,true);
				break;
			case LEX_BOOL:
				while (true) {
					cout << "Введите значение для " << TID.get_name(j) << " (true/false): ";
					cin >> buf;
					if (!strcmp(buf,"true")) { TID[j].value = 1; break; }
					if (!strcmp(buf,"false")) { TID[j].value = 0; break; }
				}
				TID.set(j,true);
				break;
			}
			args.reset();
			break;
			
		case POLIZ_GO:
			i = prog[i].value - 1;
			args.reset();
			break;
			
		case POLIZ_FGO:
			l1 = args.pop();
			if (!l1.value) i = prog[i].value - 1;
			args.reset();
			break;
			
		case LEX_ASSIGN:
			l2 = args.pop();
			l1 = args.pop();
			args.push(l2);
			j = l1.value;
			switch(TID[j].type) {
			case LEX_INT:
			case LEX_BOOL:
				TID[j].value = l2.value;
				TID.set(j,true);
				break;
			case LEX_STRING:
				if (TID.get(j)) delete[] TID[j].string;
				TID[j].string = newstr(l2.string);
				TID.set(j,true);
				break;
			case LEX_STRUCT:
				x = j;
				y = l2.value;
				j = TID[j].value;
				j = TST.get(j);
				while (j) {
					TID[x+j] = TID[y+j];
					TID.set(x+j, TID.get(y+j));
					--j;
				}
				break;
			}
			break;
			
		}
		++i;
	}
	cout << endl << "Успешно завершено!" << endl;
}



int main(int argc, char **argv) {
	
	if (argc < 2) {
		cout << "Пример вызова: " << argv[0] << " program.txt" << endl;
		return 0;
	}
	
	try {
	
		Parser PAR(argv[1]);
		PAR.analyze();
		execute(PAR.prog);
	
	}
	catch (...) { cout << endl << "Аварийно завершено" << endl; }
	
	if (z6) cout << endl << "----------TID----------" << endl;
	if (z6) TID.print();
		
}
