#include<bits/stdc++.h>
using namespace std;

// Read file and convert to list of transactions
vector<set<int>> read_transactions(string filename) {
    vector<set<int>> transactions;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        //read one line at a time, using set<int> transaction
        set<int> transaction;
        int item;
        stringstream ss(line);
        while (ss >> item) {
            transaction.insert(item);
        }
        transactions.push_back(transaction);
    }
    return transactions;
}

void print_frequent_itemsets(const vector<int>& frequency) {
    for (int i = 0; i < frequency.size(); ++i) {
        cout << "Frequent " << i + 1 << "-itemsets:\t" << frequency[i] << endl;
    }
    cout << "Total number of frequent itemsets: " << accumulate(frequency.begin(), frequency.end(), 0) << endl;
}

vector<int> generate_frequent_itemsets(const vector<set<int>>& transactions, double min_support) {
    // Count the support of each item
    /*
        using map to item_counts
        key(first): items identifier
        value(second): the number of items
    */
    map<int, int> item_counts;
    /*
        the first loop: each transaction in transactions
        tne second loop: each item in one transaction
    */
    for (const auto& transaction : transactions) {
        for (const auto& item : transaction) {
            item_counts[item]++;
        }
    }

    // Generate frequent 1-itemsets.
    /*
        vector<set<int>> frequent_itemsets;
        @param: store all key(first) of frequency items
    */
    vector<set<int>> frequent_itemsets;
    /*
        exam whether the value(number) of item_counts >= min_support
        true: frequent_itemsets.push_back(set<int>{item_count.first});
    */ 

    for (const auto& item_count : item_counts) {
        if (item_count.second >= min_support) {
            frequent_itemsets.push_back(set<int>{item_count.first});
        }
    }

    // Having inited frequent 1-itemsets
    // Generate frequent k-itemsets
    // Init k value for loops (init to 2)
    int k = 2;
    /*
        push the one frequency item size
        vector<int> frequency{static_cast<int>(frequent_itemsets.size())};    
    */ 
    vector<int> frequency{static_cast<int>(frequent_itemsets.size())};
    //loop entil frequent_itemsets is empty
    while (!frequent_itemsets.empty()) {
        // count number of each frequency item cnt and add it to frequency after each loop
        int cnt = 0;
        /*
            Generate candidate itemsets
            set<set<int>> candidate_itemsets;
        */ 
        // Ck
        set<set<int>> candidate_itemsets;
        for(int i=0;i<frequent_itemsets.size();i++){
            for (int j = i + 1 ; j < frequent_itemsets.size(); ++j) {
                set<int> union_set;
                //set_union function
                //inserter(union_set, union_set.begin())
                set_union(frequent_itemsets[i].begin(), frequent_itemsets[i].end(),
                          frequent_itemsets[j].begin(), frequent_itemsets[j].end(),
                          inserter(union_set, union_set.begin()));
                /*
                    if union_set.size() == k && candidate_itemsets.count(union_set) == 0 
                    then insert union_set into candidate_items
                */
                if (union_set.size() == k && candidate_itemsets.count(union_set) == 0) {
                    candidate_itemsets.insert(union_set);
                }
            }
        }

        // Count the support of each candidate itemset
        /*
            loop 1: transaction in transections
            loop 2: itemset in cadidate_itemsets
            function: if includes(tbegin,tend,ibegin,iend) then itemset_counts[itemset]++
        */
        map<set<int>, int> itemset_counts;
        for (const auto& transaction : transactions) {
            for (const auto& itemset : candidate_itemsets) {
                if (includes(transaction.begin(), transaction.end(), itemset.begin(), itemset.end())) {
                    itemset_counts[itemset]++;
                }
            }
        }

        // Generate frequent k-itemsets
        // Lk
        //frequent_itemsets clear first
        frequent_itemsets.clear();
        for (const auto& itemset_count : itemset_counts) {
            if ((double)itemset_count.second >= min_support) {
                frequent_itemsets.push_back(itemset_count.first);
                cnt++;
            }
        }

        frequency.push_back(cnt);
        k++;
    }

    return frequency;
}

int main() {
    //Start timer
    auto start=chrono::steady_clock::now();

    // Read and convert type of list of transactions
    auto transactions = read_transactions("retail.txt");
    cout << "Number of transactions: " << transactions.size() << endl;

    double min_support = 0. * transactions.size();
    auto frequency = generate_frequent_itemsets(transactions, min_support);

    print_frequent_itemsets(frequency);

    //End timer
    auto end=chrono::steady_clock::now();
    auto duration =chrono::duration_cast<chrono::microseconds>(end - start);
    double seconds = duration.count() / 1000000.0;
    //输出运行时间
    cout<<"Time taken to: "<<seconds<<"s"<<endl;

    return 0;
}
