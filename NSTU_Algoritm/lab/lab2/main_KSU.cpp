#include <iostream>
#include <string>
#include <stack>
#include <memory>
#include <unordered_map>
#include <functional>

using namespace std;

struct TreeNode {
    string value;
    unique_ptr<TreeNode> left, right;
    TreeNode(string val) : value(move(val)), left(nullptr), right(nullptr) {}
};

class ExpressionTree {
public:
    ExpressionTree(const string& expression) {
        size_t pos = 0;
        root = buildTree(expression, pos);
    }

    ~ExpressionTree() {
        clear(root);
    }

    void inorder() const {
        inorder(root.get());
        cout << endl;
    }

    bool isExpressionTree() const {
        return validate(root.get());
    }

    double evaluate() const {
        return evaluate(root.get());
    }

    size_t size() const {
        return size(root.get());
    }

private:
    unique_ptr<TreeNode> root;
    unordered_map<string, function<double(double, double)>> operators = {
        {"+", plus<double>()},
        {"-", minus<double>()},
        {"*", multiplies<double>()},
        {"/", divides<double>()}
    };

    unique_ptr<TreeNode> buildTree(const string& expr, size_t& pos) {
        while (pos < expr.length() && expr[pos] == ' ') ++pos;
        if (pos >= expr.length()) return nullptr;

        size_t next_space = expr.find(' ', pos);
        string token = expr.substr(pos, next_space - pos);
        pos = (next_space == string::npos) ? expr.length() : next_space + 1;

        auto node = make_unique<TreeNode>(token);
        if (operators.count(token)) {
            node->left = buildTree(expr, pos);
            node->right = buildTree(expr, pos);
        }
        return node;
    }

    void inorder(const TreeNode* node) const {
        if (!node) return;
        if (operators.count(node->value)) cout << "( ";
        inorder(node->left.get());
        cout << node->value << " ";
        inorder(node->right.get());
        if (operators.count(node->value)) cout << ") ";
    }

    bool validate(const TreeNode* node) const {
        if (!node) return true;
        if (operators.count(node->value)) {
            return node->left && node->right && validate(node->left.get()) && validate(node->right.get());
        }
        return !node->left && !node->right;
    }

    double evaluate(const TreeNode* node) const {
        if (!node) return 0;
        if (operators.count(node->value)) {
            return operators.at(node->value)(evaluate(node->left.get()), evaluate(node->right.get()));
        }
        return stod(node->value);
    }

    size_t size(const TreeNode* node) const {
        if (!node) return 0;
        return 1 + size(node->left.get()) + size(node->right.get());
    }

    void clear(unique_ptr<TreeNode>& node) {
        if (!node) return;
        node->left.reset();
        node->right.reset();
        node.reset();
    }
};

int main() {
    ExpressionTree tree("- * + 7 3 4 / 10 2");
    cout << "\nИнфиксная нотация: ";
    tree.inorder();
    cout << "Выражение действительно является деревом выражений: " << tree.isExpressionTree() << endl;
    cout << "Результат вычисления выражения: " << tree.evaluate() << endl;
    cout << "Размер дерева: " << tree.size() << endl;
    return 0;
}