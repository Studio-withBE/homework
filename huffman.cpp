#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <list>

using namespace std;

typedef struct Node
{
	double prob;
	int TiedProb;
	list<int> symbol;
	string code;
	Node* next;

	Node()
	{
		code = "";
		prob = 0;
		TiedProb = 0;
		next = NULL;
	}

	bool operator<(const Node& temp)
	{
		if (this->prob == temp.prob)
		{
			if (temp.TiedProb == 2)
				return this->prob+1 > temp.prob;
			else
				return this->prob < temp.prob+1;
		}

		return this->prob > temp.prob;
	}

}Node;

bool desc(Node A, Node B)
{
	return A < B;
}

void print_stage(vector<Node> temp)
{
	for (int i = 0; i < temp.size(); i++)
	{
		list<int>::iterator j;
		for (j = temp.at(i).symbol.begin(); j != temp.at(i).symbol.end(); j++)
			cout << *j;
		cout << "(" << temp.at(i).prob << ")";
		if (i != temp.size() - 1)
			cout << ", ";
	}

	cout << endl;
}

void print_code(vector<Node> result)
{
    cout << "Result ternary code : ";

    for (int i = 0; i < result.size(); i++)
    {
        cout << result.at(i).symbol.front() << "(" << result.at(i).code << ")";
        if (i == result.size() - 1)
            break;
        cout << ", ";
    }

    cout << endl;
}

int main()
{
	int sym_num, stage_num;
	double Ex_length = 0, var = 0;
	cin >> sym_num;

	if (sym_num % 2 == 1)
        stage_num = (sym_num + 1) / 2;
    else
        stage_num = sym_num / 2;

	typedef vector<Node> Stage;
	Stage initial;
	vector<Stage> Huffman;

	for (int i = 0; i < sym_num; i++)
	{
		Node temp;
		cin >> temp.prob;
		cin >> temp.TiedProb;
		temp.symbol.push_back(i+1);
		initial.push_back(temp);
	}

    Stage temp_initial(initial);
	Huffman.push_back(temp_initial);

	for (int i = 1; i <= stage_num; i++)
	{
		cout << "Stage " << i << " : ";
		sort(Huffman.at(i - 1).begin(), Huffman.at(i - 1).end(), desc);
		print_stage(Huffman.at(i-1));

        if (i == stage_num)
            break;

		Stage temp(Huffman.at(i-1));
		double sum_prob = temp.at(temp.size() - 1).prob + temp.at(temp.size() - 2).prob + temp.at(temp.size() - 3).prob;

		vector<Node>::iterator str = Huffman.back().end() - 1;
        (*str).code = "2";
        (*(str - 1)).code = "1";
        (*(str - 2)).code = "0";

		Node new_node;
		new_node.prob = sum_prob;

		temp.at(temp.size() - 1).next = &new_node;
		temp.at(temp.size() - 2).next = &new_node;
		temp.at(temp.size() - 3).next = &new_node;

        list<int>::iterator j;

        for (int k = 0; k < 3; k++)
            for (j = temp.at(temp.size() - 1 - k).symbol.begin(); j != temp.at(temp.size() - 1 - k).symbol.end(); j++)
                new_node.symbol.push_back(*j);

        for (int x = 0; x < 3; x++)
            temp.pop_back();

        for (int x = 0; x < temp.size(); x++)
            Huffman.at(i - 1).at(x).next = &(temp.at(x));

        temp.push_back(new_node);
        Huffman.push_back(temp);
	}

    int num = Huffman.at(stage_num - 1).size();

	switch(num)
	{
        case 1:
            Huffman.at(stage_num - 1).at(0).code = "0";
            break;
        case 2:
            Huffman.at(stage_num - 1).at(0).code = "0";
            Huffman.at(stage_num - 1).at(1).code = "1";
            break;
        case 3:
            Huffman.at(stage_num - 1).at(0).code = "0";
            Huffman.at(stage_num - 1).at(1).code = "1";
            Huffman.at(stage_num - 1).at(2).code = "2";
            break;
	}

	for(int i = 0; i < initial.size(); i++)
    {
        Node* temp = &Huffman.at(0).at(i);

        while (temp != NULL)
        {
            initial.at(i).code = temp->code + initial.at(i).code;
            temp = temp->next;
        }
    }

    print_code(initial);

	for (int i = 0; i < initial.size(); i++)
		Ex_length += initial.at(i).prob * initial.at(i).code.length();

	for (int i = 0; i < initial.size(); i++)
		var = initial.at(i).prob * pow((Ex_length - initial.at(i).code.length()), 2);

	cout << "Expected length : " << Ex_length << endl;
	cout << "Variance : " << var << endl;

	return 0;
}
