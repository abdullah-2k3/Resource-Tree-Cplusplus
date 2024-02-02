#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;


string to_lower(const string& str) {
    string lower = "";
    for (size_t i = 0; i < str.length(); i++) {
        lower += tolower(str[i]);
    }

    return lower;
}


class skillNode {
        friend class skillTree;
        int level; // it will be used as integer value in node to sort skill tree
        string name;
        skillNode *left;
        skillNode *right;
    public:
        skillNode(int level, string name) {
            this->level = level;
            this->name = name;
            left = nullptr;
            right = nullptr;
        }

        string get_name() {
            return this->name;
        }

        int get_level() {
            return this->level;
        }

        skillNode* get_left() {
            return this->left;
        }

        skillNode* get_right() {
            return this->right;
        }
    };

class skillTree {


    skillNode* root;
public:
    int size;
    skillTree () {
        root = nullptr;
        size = 0;
    }

    skillNode* get_root() {
        return this->root;
    }

    void insert(int level, string name) {
        skillNode* newNode = new skillNode(level, name);

        if (root == nullptr)
            root = newNode;

        else {
            queue<skillNode*> tmp_queue;
            tmp_queue.push(root);

            while (!tmp_queue.empty()) {
                skillNode* curr = tmp_queue.front();
                tmp_queue.pop();

                if (curr->left == nullptr) {
                    curr->left = newNode;
                    break;
                }

                else if (curr->right == nullptr) {
                    curr->right = newNode;
                    break;
                }

                else {
                    tmp_queue.push(curr->left);
                    tmp_queue.push(curr->right);
                }
            }
        }
        size++;

        for (int i = 0; i < size / 2 - 1; i++) {
                max_heap(root);
        }

    }

    void max_heap(skillNode* curr) {

        if (curr == nullptr)
            return;

        if (curr->left && curr->left->level > curr->level) {
            swap(curr->left->level, curr->level);
            swap(curr->left->name, curr->name);
        }
        if (curr->right && curr->right->level > curr->level) {
            swap(curr->right->level, curr->level);
            swap(curr->right->name, curr->name);
        }

        max_heap(curr->left);
        max_heap(curr->right);
    }

    void min_heap(skillNode* curr) {
        if (curr == nullptr)
            return;

        if (curr->left && curr->left->level < curr->level) {
            swap(curr->left->level, curr->level);
            swap(curr->left->name, curr->name);
        }
        if (curr->right && curr->right->level < curr->level) {
            swap(curr->right->level, curr->level);
            swap(curr->right->name, curr->name);
        }

        min_heap(curr->left);
        min_heap(curr->right);
    }

    void inorder(skillNode* curr) {
        if (curr == nullptr)
            return;

        inorder(curr->left);
        cout << curr->name << "," << curr->level << "\t";
        inorder(curr->right);

    }

    friend ostream& operator << (ostream& out, skillTree* obj) {

        obj->inorder(obj->root);

        return out;
    }

};

class Node {
    friend class Tree;
    int id; // it will be used as integer value in node to sort resource tree
    string name;
    skillTree *root;
    Node* left;
    Node* right;
public:
    Node(int id, string name, skillTree* tree) {
        this->id = id;
        this->name = name;
        this->root = tree;
        left = nullptr;
        right = nullptr;
    }

    skillTree* get_root() {
        return root;
    }

    Node* get_left() {
        return this->left;
    }

    Node* get_right() {
        return this->right;
    }

    int get_id () {
        return this->id;
    }

    string get_name() {
        return this->name;
    }
};

class Tree {


    Node* root;

    void inorder_display(Node* curr) {
        if (curr == nullptr)
            return;

        inorder_display(curr->left);
        cout << "ID: " << curr->id << endl << "Name: " << curr->name << endl << "Skills: " << curr->root << endl << endl;
        inorder_display(curr->right);
    }
  public:
    Tree() {
        root = nullptr;
    }

    Node* get_root() {
        return root;
    }

    void insert(int id, string name, vector<string> skills, vector<int> level) {

        skillTree* tmp_tree = new skillTree;

        for (size_t i = 0; i < skills.size(); i++) {
            tmp_tree->insert(level[i], skills[i]);
        }

        Node* newNode = new Node(id, name, tmp_tree);
        if (root == nullptr)
            root = newNode;

        else {
            queue<Node*> tmp_queue;
            tmp_queue.push(root);

            while (!tmp_queue.empty()) {
                Node* curr = tmp_queue.front();
                tmp_queue.pop();

                if (curr->left == nullptr) {
                    curr->left = newNode;
                    break;
                }

                else if (curr->right == nullptr) {
                    curr->right = newNode;
                    break;
                }

                else {
                    tmp_queue.push(curr->left);
                    tmp_queue.push(curr->right);
                }
            }
        }
    }

    void read_file(string filename) {
        ifstream fin(filename);
        string line;
        string name;
        vector<string> skill_name;
        int id;
        vector<int> level;

        while (getline(fin, line)) {
            string word = "";
            name = "";
            skill_name.clear();
            level.clear();

            id = line[0] - '0';

            int c = 2;
            while (line[c] != ',') {
                name += line[c];
                c++;
            }

            for (size_t i = c; i < line.length(); i++) {

                if (line[i] == ',')
                    continue;


                if (line[i] == ':') {
                    skill_name.push_back(word);
                    word = "";
                    i++;
                    if (line[i] == 'b')
                        level.push_back(1);
                    else if (line[i] == 'i')
                        level.push_back(2);
                    else if (line[i] == 'e')
                        level.push_back(3);
                    else if (line[i] >= '0' && line[i] <= '9')
                        level.push_back(line[i] - '0');
                    continue;

                }
                word += line[i];
            }
            insert(id, name, skill_name, level);

        }
        fin.close();
    }

    void display () {
        inorder_display(root);
    }
};

class AllocationNode {
    friend class AllocationTree;
    int id;
    string name;
    struct AllocatedResources {
        int id;
        string name;
    };
    AllocationNode* left;
    AllocationNode* right;
    AllocatedResources resources;
public:
    AllocationNode(int t_id, string t_name, int r_id, string r_name) {
        this->id = t_id;
        this->name = t_name;
        resources.id = r_id;
        resources.name = r_name;
        left = right = nullptr;
    }

};

class AllocationTree {
    AllocationNode* root;

    struct Similarity_index {
        int name_match = 0;
        float level_match = 0;
    };

    Similarity_index compare_skill_tree(skillTree* resourse, skillTree* task) {
        Similarity_index s1;
        vector<skillNode*> matched_resources;

        stack<skillNode*> tsk_stack;
        if (task->get_root()) {
            tsk_stack.push(task->get_root());
        }
        else
            return s1;


        while (!tsk_stack.empty()) {

            skillNode* tsk_curr = tsk_stack.top();
            tsk_stack.pop();

            stack<skillNode*> res_stack;
            if (resourse->get_root()) {
                res_stack.push(resourse->get_root());
            }
            else
                return s1;

            while (!res_stack.empty()) {

                skillNode* res_curr = res_stack.top();
                res_stack.pop();

                string resourse_name = to_lower(res_curr->get_name());
                float resourse_level = res_curr->get_level();

                string task_name = to_lower(tsk_curr->get_name());
                float task_level = tsk_curr->get_level();

                if (resourse_name == task_name) {
                    matched_resources.push_back(res_curr);
                    s1.name_match++;
                    if (resourse_level >= task_level)
                        s1.level_match++;
                    else
                        s1.level_match += (resourse_level/task_level);


                }

                if (res_curr->get_right())
                    if (!is_skill_matched(matched_resources, res_curr->get_right()))
                        res_stack.push(res_curr->get_right());
                if (res_curr->get_left())
                    if (!is_skill_matched(matched_resources, res_curr->get_left()))
                        res_stack.push(res_curr->get_left());

            }

            if (tsk_curr->get_right())
                tsk_stack.push(tsk_curr->get_right());
            if (tsk_curr->get_left())
                tsk_stack.push(tsk_curr->get_left());
        }

        s1.level_match = (s1.level_match/task->size) * 100;
        return s1;
}

    bool is_skill_matched(vector<skillNode*> matched, skillNode* node) {

        for (size_t i = 0; i < matched.size(); i++) {
            if (node == matched[i])
                return true;
        }
        return false;
    }

    void inorder_display(AllocationNode* curr) {
        if (curr == nullptr)
            return;

        inorder_display(curr->left);
        cout << "Task ID: " << curr->id << endl
             << "Task Name: " << curr->name << endl
             << "Resource ID: " << curr->resources.id << endl
             << "Resource Name: " << curr->resources.name << endl << endl;
        inorder_display(curr->right);

    }
public:
    AllocationTree() : root(nullptr) {}

    void insert(int task_id, string task_name, int resource_id, string resource_name) {

        AllocationNode* newNode = new AllocationNode(task_id, task_name, resource_id, resource_name);

        if (root == nullptr) {
            root = newNode;
            return;
        }

        else {
            stack<AllocationNode*> node_stack;
            node_stack.push(root);
            while (!node_stack.empty()) {
                AllocationNode* curr = node_stack.top();
                node_stack.pop();
                if (curr->left == nullptr) {
                    curr->left = newNode;
                    break;
                }
                else if (curr->right == nullptr) {
                    curr->right = newNode;
                    break;
                }
                else {
                    node_stack.push(curr->right);
                    node_stack.push(curr->left);
                }

            }
        }
    }

    void allocate_resources(Tree& r_tree, Tree& t_tree) {

        vector<Similarity_index> similarity_vector;
        Similarity_index s;
        Similarity_index best_match;

        stack<Node*> tsk_stack;
        tsk_stack.push(t_tree.get_root());

        while (!tsk_stack.empty()) {

            Node* curr_task = tsk_stack.top();
            tsk_stack.pop();

            stack<Node*> res_stack;
            res_stack.push(r_tree.get_root());

            best_match = compare_skill_tree(r_tree.get_root()->get_root(), curr_task->get_root());
            Node* best_resource;
            best_resource = r_tree.get_root();

            while (!res_stack.empty()) {
                Node* curr_res = res_stack.top();
                res_stack.pop();

                s = compare_skill_tree(curr_res->get_root(), curr_task->get_root());

                if (s.name_match > best_match.name_match) {
                    best_match = s;
                    best_resource = curr_res;
                }
                else if (s.name_match == best_match.name_match) {
                    if (s.level_match > best_match.level_match) {
                        best_match = s;
                        best_resource = curr_res;
                    }
                }

                similarity_vector.push_back(s);

                if (curr_res->get_right())
                    res_stack.push(curr_res->get_right());

                if (curr_res->get_left())
                    res_stack.push(curr_res->get_left());
            }

            insert(curr_task->get_id(), curr_task->get_name(), best_resource->get_id(), best_resource->get_name());
            cout << "Task: \n";
            cout << "ID: "<< curr_task->get_id() << "\nName: " << curr_task->get_name() << "\nSkill Tree: \n"<< curr_task->get_root() << endl << endl;
            cout << "Best Match: \n";
            cout << "ID: "<< best_resource->get_id() << "\nName: " << best_resource->get_name() << "\nSkill Tree: \n"<< best_resource->get_root() << endl << endl;
            cout << "Accuracy %: " << best_match.level_match << endl << endl;
            cout << "-------------------------------------------------------------\n\n";

            if (curr_task->get_right())
                tsk_stack.push(curr_task->get_right());
            if (curr_task->get_left())
                tsk_stack.push(curr_task->get_left());
        }

    }

    void display() {
        inorder_display(root);
    }
};


int main() {
    Tree r_tree;

    r_tree.read_file("resource.txt");

    cout << "Resource Tree: " << endl << endl;
    r_tree.display();

    Tree task_tree;

    task_tree.read_file("tasks.txt");

    cout << "\n\n------------------------------------------------\n\n";
    cout << "Task Tree: " << endl << endl;
    task_tree.display();

    cout << "\n\n------------------------------------------------\n\n";

    AllocationTree A_tree;

    A_tree.allocate_resources(r_tree, task_tree);

    cout << "Showing Allocation Tree: " << endl << endl;
    A_tree.display();

    return 0;
}
