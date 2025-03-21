#include <iostream>
#include <string>
#include <stack>
#include <memory>
#include <unordered_map>
#include <functional>

using namespace std;

struct TreeNode
{
    string value;
    unique_ptr<TreeNode> left, right;

    TreeNode(string val) : value(move(val)), left(nullptr), right(nullptr)
    {
    }
};

class ExpressionTree
{
public:
    ExpressionTree(const string& expression, int& count)
    {
        size_t pos = 0;
        root = buildTree(expression, pos, count);
    }

    ~ExpressionTree()
    {
        clear(root);
    }

    void postorder() const
    {
        postorder(root.get());
        cout << endl;
    }

    bool isExpressionTree() const
    {
        return validate(root.get());
    }

    double evaluate(int& count) const
    {
        return evaluate(root.get(), count);
    }

    size_t size() const
    {
        return size(root.get());
    }

    bool search(std::string token, int& count)
    {
        return search(root.get(), token, count);
    }

private:
    unique_ptr<TreeNode> root;
    unordered_map<string, function<double(double, double)>> operators = {
        {"+", plus<double>()},
        {"-", minus<double>()},
        {"*", multiplies<double>()},
        {"/", divides<double>()}
    };

    unique_ptr<TreeNode> buildTree(const string& expr, size_t& pos, int& count)
    {
        count++;
        while (pos < expr.length() && expr[pos] == ' ') ++pos;
        if (pos >= expr.length()) return nullptr;

        const size_t next_space = expr.find(' ', pos);
        string token = expr.substr(pos, next_space - pos);
        // pos = (next_space == string::npos) ? expr.length() : next_space + 1;
        pos = next_space + 1;

        unique_ptr<TreeNode> node = make_unique<TreeNode>(token);
        if (operators.count(token))
        {
            node->left = buildTree(expr, pos, count);
            node->right = buildTree(expr, pos, count);
        }
        return node;
    }

    void postorder(const TreeNode* node) const
    {
        if (!node) return;
        postorder(node->left.get());
        postorder(node->right.get());
        cout << node->value << " ";
    }

    bool search(const TreeNode* node, std::string token, int& count)
    {
        count++;
        if (node)
        {
            if (node->value == token) return true;
            if (search(node->left.get(), token, count)) return true;
            if (search(node->right.get(), token, count)) return true;
        }
        return false;
    }

    bool validate(const TreeNode* node) const
    {
        if (!node) return true;
        if (operators.count(node->value))
        {
            return node->left && node->right && validate(node->left.get()) && validate(node->right.get());
        }
        return !node->left && !node->right;
    }

    double evaluate(const TreeNode* node, int& count) const
    {
        count++;
        if (!node) return 0;
        if (operators.count(node->value))
        {
            return operators.at(node->value)(evaluate(node->left.get(), count), evaluate(node->right.get(), count));
        }
        return stod(node->value);
    }

    void clear(unique_ptr<TreeNode>& node)
    {
        if (!node) return;
        node->left.reset();
        node->right.reset();
        node.reset();
    }

    size_t size(const TreeNode* node) const
    {
        if (!node) return 0;
        return 1 + size(node->left.get()) + size(node->right.get());
    }
};

void mesuare(std::string test_data)
{
    cout << test_data << endl;
    int count = 0;
    ExpressionTree tree = ExpressionTree(test_data, count);
    if (!tree.isExpressionTree())
    {
        return;
    }
    cout << "insert: "<< count<< " " << count / (test_data.length()/2)<< " ";

    count = 0;
    tree.evaluate(count);
    cout << "evaluate:" << count<< " " << count / (test_data.length()/2)<< " ";
    count = 0;
    tree.search("*", count);
    cout << "search:"<< count<< " " << count / (test_data.length() / 2) << endl;
}


int main()
{
    // int count = 0;
    // ExpressionTree tree("/ + * 5 10 9 - 3 2", count);
    // tree.postorder();
    // tree.isExpressionTree()
    //     ? cout << "\nВыражение является деревом выражений\n"
    //     : cout << "\nВыражение не является деревом выражений\n";
    // cout << "Значение выражения: " << tree.evaluate(count) << endl;
    // cout << "Размер дерева: " << tree.size() << endl;
    mesuare("+ 5 5");
    mesuare("+ + 5 5 5");
    mesuare("+ + + 5 5 5 5");
    mesuare("+ + + + 5 5 5 5 5");
    mesuare("+ + + + + 5 5 5 5 5 5");
    mesuare("+ + + + + + 5 5 5 5 5 5 5");

    mesuare("+ 5 5");
    mesuare("+ + 5 5 + 5 5");
    mesuare("+ + + 5 5 + 5 5 + + 5 5 + 5 5");
    mesuare("+ + + + 5 5 + 5 5 + + 5 5 + 5 5 + + + 5 5 + 5 5 + + 5 5 + 5 5");
    return 0;
}
