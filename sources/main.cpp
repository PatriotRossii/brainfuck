#include <unordered_map>
#include <string_view>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <vector>
#include <stack>

std::unordered_map<std::size_t, std::size_t> build_bracemap(
	std::string_view code
) {
	std::stack<std::size_t> bracestack;
	std::unordered_map<std::size_t, std::size_t> bracemap;

	for (std::size_t position = 0, length = code.size(); position != length; ++position) {
		auto command = code[position];
		if (command == '[') bracestack.push(position);
		else if (command == ']') {
			std::size_t start = bracestack.top();
			bracestack.pop();
			bracemap[start] = position;
			bracemap[position] = start;
		}
	}

	return bracemap;
}

void evaluate(std::string_view code) {
	auto bracemap = build_bracemap(code);
	
	std::vector<char> cells = {0};
	std::size_t code_ptr = 0, cell_ptr = 0;

	while (code_ptr != code.size()) {
		auto command = code[code_ptr];

		if (command == '>') {
			cell_ptr += 1;
			if (cell_ptr == cells.size())
				cells.push_back(0);
		}
		if (command == '<')
			cell_ptr =  (cell_ptr <= 0 ? 0 : cell_ptr - 1);
		if (command == '+') ++cells[cell_ptr];
		if (command == '-') --cells[cell_ptr];
		if (command == '[' && cells[cell_ptr] == 0)
			code_ptr = bracemap[code_ptr];
		if (command == ']' && cells[cell_ptr] != 0)
			code_ptr = bracemap[code_ptr];
		if (command == '.')
			std::cout << cells[cell_ptr];
		if (command == ',')
			std::cin.get(cells[cell_ptr]);

		code_ptr += 1;
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2) return 1;

	std::ifstream ifs(argv[1]);
	std::string code(std::istreambuf_iterator<char>{ifs}, {});
	evaluate(code);

	return 0;
}