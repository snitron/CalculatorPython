#include <iostream>
#include <vector>
#include <string>
#include <pybind11/pybind11.h>

class Ratio {
public:
	int m_a, m_b;

	Ratio() {
		m_a = 0;
		m_b = 1;
	}

	Ratio(int a, int b) {
		m_a = a;
		m_b = b;
	}

	void add(Ratio other) {
		m_a *= other.m_b;
		m_a += other.m_a * m_b;
		m_b *= other.m_b;
	}

	void min(Ratio other) {
		m_a *= other.m_b;
		m_a -= other.m_a * m_b;
		m_b *= other.m_b;
	}

	void mul(Ratio other) {
		m_a *= other.m_a;
		m_b *= other.m_b;
	}

	void div(Ratio other) {
		m_a *= other.m_b;
		m_b *= other.m_a;
	}

	std::string print() {
		return std::to_string(m_a) + "/" + std::to_string(m_b);
	}

	void reduce() {
		int p = m_a, q = m_b;
		int pp = 0;
		if (p != 0 && q != 0) {
			pp = std::abs(p);
			while (pp != q) {
				if (pp > q) pp = pp - q;
				else q = q - pp;
			}
		}

		m_a /= pp;
		m_b /= pp;
	}

};


class Token {
public:
	Ratio m_value;
	char m_kind;

	Token(char kind) {
		m_kind = kind;
	}

	Token(Ratio ratio) {
		m_kind = '@';
		m_value = ratio;
	}

	bool operator ==(const Token& otherToken) const {
		return m_kind == otherToken.m_kind;
	}

};

Ratio bar();
Ratio foo();

std::vector<Token> tokens = std::vector<Token>();
int iter = 0;

Ratio bar() {
	try {
		if (++iter == tokens.size()) {
			throw std::exception("expression_exception");
		}

		Token t = tokens[iter];
		Ratio l = Ratio();

		if (t.m_kind != '@' && t.m_kind != '(') {
			throw std::exception("expression_exception");
		}

		if (t.m_kind == '(') l = foo();
		if (t.m_kind == '@') l = { t.m_value.m_a, t.m_value.m_b };

		while (++iter < tokens.size()) {
			t = tokens[iter];

			if (t.m_kind == '+' || t.m_kind == '-') {
				--iter;
				break;
			}

			if (t.m_kind == ')') {
				--iter;
				break;
			}

			if (t.m_kind == '(') {
				throw std::exception("expression_exception");
			}

			Ratio r = Ratio();

			if (++iter == tokens.size()) {
				throw std::exception("expression_exception");
			}

			Token t2 = tokens[iter];

			if (t2.m_kind != '@' && t2.m_kind != '(') {
				throw std::exception("expression_exception");
			}

			if (t2.m_kind == '(') r = foo();
			if (t2.m_kind == '@') r = { t2.m_value.m_a, t2.m_value.m_b };

			if (t.m_kind == '*') l.mul(r);
			if (t.m_kind == ':') l.div(r);
		}

		return l;
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}
}
Ratio foo() {
	Ratio l = bar();

	while (++iter < tokens.size()) {
		Token t = tokens[iter];
		if (t.m_kind == ')') break;

		if (t.m_kind == '+') l.add(bar());
		if (t.m_kind == '-') l.min(bar());
	}

	return l;
}

static void read(std::string x) {
	std::string integer = "", numerator = "", denominator = "";
	bool isNumber = true, isMixed = false, isLite = false, isEnd = false, isMixedD = false, isLiteD = false, wasD = false, wasSk = false, wasSk2 = false, wasDivZero = false, isDFirst = true, isMFirst = true, isSqrtFirst = true, iF = true, isSt = false, safe = false;
	int c = 0;


	for (int i = 0; i < x.length(); i++) {
		if (x[i] == '(') {
			if (wasDivZero) wasDivZero = false;
			tokens.push_back(Token('('));
			if (x[i + 1] == '-') {
				tokens.push_back(Token(Ratio(0, 1)));
			}
			isLite = false;
			isMixed = false;
			isNumber = true;
			isMixedD = false;
			isLiteD = false;
			isEnd = false;
			numerator = "";
			integer = "";
			denominator = "";
			c++;
			wasSk2 = true;
		}
		if (x[i] == ')') {
			isMFirst = false;
			wasSk = false;
			if (x[i - 1] != ')') {
				if (isNumber)
					tokens.push_back(Token(Ratio(std::stoi(integer), 1)));
				if (isMixedD) {
					if (std::stoi(denominator) != 0) {
						tokens.push_back(Token(Ratio(std::stoi(integer) * std::stoi(denominator) + std::stoi(numerator), std::stoi(denominator))));
					}
					else {
						throw std::exception("arithmetic_exception");
					}
				}
				if (isLiteD) {
					if (std::stoi(denominator) != 0) {
						tokens.push_back(Token(Ratio(std::stoi(numerator), std::stoi(denominator))));
					}
					else {
						throw std::exception("arithmetic_exception");
					}
				}
			}
			tokens.push_back(Token(')'));
			c--;
			if (c < 0) {
				throw std::exception("bracket_exception");
			}
			isLite = false;
			isMixed = false;
			isNumber = true;
			isMixedD = false;
			isLiteD = false;
			isEnd = false;
			numerator = "";
			integer = "";
			denominator = "";
			wasSk = true;
		}
		if (x[i] == '+') {
			isMFirst = false;
			if (wasDivZero) wasDivZero = false;
			if (!wasSk) {

				if (isNumber)
					tokens.push_back(Token(Ratio(std::stoi(integer), 1)));
				if (isMixedD) {
					if (std::stoi(denominator) != 0 && denominator != "") {
						tokens.push_back(Token(Ratio(std::stoi(integer) * std::stoi(denominator) + std::stoi(numerator), std::stoi(denominator))));
					}
					else {
						throw std::exception("arithmetic_exception");
					}
				}
				if (isLiteD) {
					if (std::stoi(denominator) != 0) {
						tokens.push_back(Token(Ratio(std::stoi(numerator), std::stoi(denominator))));
					}
					else {
						throw std::exception("arithmetic_exception");
					}
				}
			}
			else {
				wasSk = false;
			}
			tokens.push_back(Token('+'));
			isLite = false;
			isMixed = false;
			isNumber = true;
			isMixedD = false;
			isLiteD = false;
			isEnd = false;
			numerator = "";
			integer = "";
			denominator = "";
			wasD = true;
		}
		if (x[i] == '-') {
			if (wasDivZero) wasDivZero = false;
			if (!wasSk) {
				if (!isMFirst) {
					if (integer != "" || numerator != "") {
						if (isNumber)
							tokens.push_back(Token(Ratio(std::stoi(integer), 1)));
						if (isMixedD) {
							if (std::stoi(denominator) != 0) {
								tokens.push_back(Token(Ratio(std::stoi(integer) * std::stoi(denominator) + std::stoi(numerator), std::stoi(denominator))));
							}
							else {
								throw std::exception("arithmetic_exception");
							}
						}
						if (isLiteD) {

							if (std::stoi(denominator) != 0) {
								tokens.push_back(Token(Ratio(std::stoi(numerator), std::stoi(denominator))));
							}
							else {
								throw std::exception("arithmetic_exception");
							}
						}
					}
					tokens.push_back(Token('-'));
				}
				else {
					tokens.push_back(Token(Ratio(0, 1)));
					tokens.push_back(Token('-'));
					isMFirst = false;
				}
			}
			else {
				wasSk = false;
				tokens.push_back(Token('-'));
			}

			isLite = false;
			isMixedD = false;
			isLiteD = false;
			isMixed = false;
			isNumber = true;
			isEnd = false;
			numerator = "";
			integer = "";
			denominator = "";
			wasD = true;
		}
		if (x[i] == '*') {
			isMFirst = false;
			if (wasDivZero) wasDivZero = false;
			if (!wasSk) {
				if (isNumber)
					tokens.push_back(Token(Ratio(std::stoi(integer), 1)));
				if (isMixedD) {
					if (std::stoi(denominator) != 0) {
						tokens.push_back(Token(Ratio(std::stoi(integer) * std::stoi(denominator) + std::stoi(numerator), std::stoi(denominator))));
					}
					else {
						throw std::exception("arithmetic_exception");
					}
				}
				if (isLiteD) {
					if (std::stoi(denominator) != 0) {
						tokens.push_back(Token(Ratio(std::stoi(numerator), std::stoi(denominator))));
					}
					else {
						throw std::exception("arithmetic_exception");
					}
				}
			}
			else {
				wasSk = false;
			}
			tokens.push_back(Token('*'));
			isLite = false;
			isMixed = false;
			isMixedD = false;
			isLiteD = false;
			isNumber = true;
			isEnd = false;
			numerator = "";
			integer = "";
			denominator = "";
			wasD = true;

		}

		if (x[i] == ':') {
			isMFirst = false;
			if (wasDivZero) wasDivZero = false;
			if (!wasSk) {
				if (isNumber)
					tokens.push_back(Token(Ratio(std::stoi(integer), 1)));
				if (isMixedD) {
					if (std::stoi(denominator) != 0) {
						tokens.push_back(Token(Ratio(std::stoi(integer) * std::stoi(denominator) + std::stoi(numerator), std::stoi(denominator))));
					}
					else {
						throw std::exception("arithmetic_exception");
					}
				}
				if (isLiteD) {
					if (std::stoi(denominator) != 0) {
						tokens.push_back(Token(Ratio(std::stoi(numerator), std::stoi(denominator))));
					}
					else {
						throw std::exception("arithmetic_exception");
					}
				}
			}
			else {
				wasSk = false;
			}
			tokens.push_back(Token(':'));
			isLite = false;
			isMixed = false;
			isMixedD = false;
			isLiteD = false;
			isNumber = true;
			isEnd = false;
			numerator = "";
			integer = "";
			denominator = "";
			wasD = true;
			wasDivZero = true;
		}


		if (x[i] == '1') {
			wasD = false;
			isMFirst = false;
			if (wasDivZero) wasDivZero = false;
			if (isNumber) {
				integer += "1";
			}
			if (isMixed) {
				numerator += "1";
			}
			if (isEnd) {
				denominator += "1";
			}
		}
		if (x[i] == '2') {
			wasD = false;
			isMFirst = false;
			if (wasDivZero) wasDivZero = false;
			if (isNumber) {
				integer += "2";
			}
			if (isMixed) {
				numerator += "2";
			}
			if (isEnd) {
				denominator += "2";
			}
		}
		if (x[i] == '3') {
			wasD = false;
			isMFirst = false;
			if (wasDivZero) wasDivZero = false;
			if (isNumber) {
				integer += "3";
			}
			if (isMixed) {
				numerator += "3";
			}
			if (isEnd) {
				denominator += "3";
			}
		}
		if (x[i] == '4') {
			wasD = false;
			isMFirst = false;
			if (wasDivZero) wasDivZero = false;
			if (isNumber) {
				integer += "4";
			}
			if (isMixed) {
				numerator += "4";
			}
			if (isEnd) {
				denominator += "4";
			}
		}

		if (x[i] == '5') {
			wasD = false;
			isMFirst = false;
			if (wasDivZero) wasDivZero = false;
			if (isNumber) {
				integer += "5";
			}
			if (isMixed) {
				numerator += "5";
			}
			if (isEnd) {
				denominator += "5";
			}
		}
		if (x[i] == '6') {
			wasD = false;
			isMFirst = false;
			if (wasDivZero) wasDivZero = false;
			if (isNumber) {
				integer += "6";
			}
			if (isMixed) {
				numerator += "6";
			}
			if (isEnd) {
				denominator += "6";
			}
		}
		if (x[i] == '7') {
			wasD = false;
			isMFirst = false;
			if (wasDivZero) wasDivZero = false;
			if (isNumber) {
				integer += "7";
			}
			if (isMixed) {
				numerator += "7";
			}
			if (isEnd) {
				denominator += "7";
			}
		}
		if (x[i] == '8') {
			wasD = false;
			isMFirst = false;
			if (wasDivZero) wasDivZero = false;
			if (isNumber) {
				integer += "8";
			}
			if (isMixed) {
				numerator += "8";
			}
			if (isEnd) {
				denominator += "8";
			}
		}
		if (x[i] == '9') {
			wasD = false;
			isMFirst = false;
			if (wasDivZero) wasDivZero = false;
			if (isNumber) {
				integer += "9";
			}
			if (isMixed) {
				numerator += "9";
			}
			if (isEnd) {
				denominator += "9";
			}
		}
		if (x[i] == '0') {
			wasD = false;
			isMFirst = false;
			if (!wasDivZero) {
				if (isNumber) {
					integer += "0";
				}
				if (isMixed) {
					numerator += "0";
				}
				if (isEnd) {
					denominator += "0";
				}
			}
			else {
				throw std::exception("arithmetic_exception");

			}
		}

		if (x[i] == '_') {
			wasD = false;
			isMFirst = false;
			if (wasDivZero) wasDivZero = false;
			isMixed = true;
			isNumber = false;
		}

		if (x[i] == '/') {
			wasD = false;
			isMFirst = false;
			wasDivZero = true;
			if (isMixed) {
				isMixed = false;
				isNumber = false;
				isEnd = true;
				isMixedD = true;
			}
			else {
				isMixed = false;
				numerator = integer;
				isLiteD = true;
				isNumber = false;
				isEnd = true;
			}

		}
		//:
		if (i == x.length() - 1) {

			if (!wasSk) {
				if (isNumber)
					tokens.push_back(Token(Ratio(std::stoi(integer), 1)));

				if (isMixedD)
					tokens.push_back(Token(Ratio(std::stoi(integer) * std::stoi(denominator) + std::stoi(numerator), std::stoi(denominator))));
				if (isLiteD)
					tokens.push_back(Token(Ratio(std::stoi(numerator), std::stoi(denominator))));

			}
			if (c != 0) {
				throw std::exception("bracket_exception");
			}
			wasSk = false;
		}



	}
}

std::string calculate(std::string data) {
	tokens.clear();

	read("(" + data + ")");

	iter = 0;
	Ratio result = foo();
	result.reduce();
	return result.print();
}


namespace py = pybind11;
PYBIND11_MODULE(napps_cpp_solver, m) {
	m.doc() = "napps_cpp_solver example plugin"; // optional module docstring

	m.def("calculate", &calculate, "A function which adds two numbers");
}

