#include <iostream>
#include <sstream>
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
        istringstream iss(expression);
        root = buildTree(iss);
    }

    void postorder() const {
        postorder(root.get());
        cout << endl;
    }

    bool isExpressionTree() const {
        return validate(root.get());
    }

    double evaluate() const {
        return evaluate(root.get());
    }

private:
    unique_ptr<TreeNode> root;
    unordered_map<string, function<double(double, double)>> operators = {
        {"+", plus<double>()},
        {"-", minus<double>()},
        {"*", multiplies<double>()},
        {"/", divides<double>()}
    };

    unique_ptr<TreeNode> buildTree(istringstream& iss) {
        string token;
        if (!(iss >> token)) return nullptr;
        auto node = make_unique<TreeNode>(token);
        if (operators.count(token)) {
            node->left = buildTree(iss);
            node->right = buildTree(iss);
        }
        return node;
    }

    void postorder(const TreeNode* node) const {
        if (!node) return;
        postorder(node->left.get());
        postorder(node->right.get());
        cout << node->value << " ";
    }

    bool validate(const TreeNode* node) const {
        if (!node) return true;
        if (operators.count(node->value)) {
            return node->left && node->right;
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
};

int main() {
    ExpressionTree tree("/ + * 5 10 9 - 3 2");
    tree.postorder();
    cout << "\nВыражение действительно является деревом выражений: " << tree.isExpressionTree() << endl;
    cout << "Результат вычисления выражения: " << tree.evaluate() << endl;
    return 0;
}
