#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <bitset>
#include <cmath>


using namespace std;


vector<int> convertBitsToSetIndices(bitset<10> bits, int count) {
  vector<int> returnVector;
  for (int i = 0; i < count; i++) {
    if (bits[i] == 1) {
      returnVector.push_back(i+1);
    }
  }
  return returnVector;
}

class Relation {
  public:
    vector< vector<int> > adjacencyMatrix;

    Relation(int rows, int columns): adjacencyMatrix(rows+1, vector<int>(columns+1, 0)) {}

    bool ownsContent(int user) {
      bool returnBool = false;
      for (int i = 1; i <= adjacencyMatrix[user].size(); i++) {
        if (adjacencyMatrix[user][i] != 0) {
          returnBool = true;
          break;
        }
      }
      return returnBool;
    }


    void parseFriendship(int rows, int columns, map<int, vector<int> > relationMap) {
      for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= columns; j++) {

          bool found_j = ( find(relationMap[i].begin(), relationMap[i].end(), j) != relationMap[i].end() );
          bool found_i = ( find(relationMap[j].begin(), relationMap[j].end(), i) != relationMap[j].end() );

          if (i == j) {
            adjacencyMatrix[i][j] = 1;
          } else if (relationMap.count(i) && found_j) {
            adjacencyMatrix[i][j] = 1;
          } else if (relationMap.count(j) && found_i) {
            adjacencyMatrix[i][j] = 1;
          } else {
            adjacencyMatrix[i][j] = 0;
          }

        }
      }
    }

    void parseContent(int users, int content, map<int, vector<int> > relationMap) {
      for (int i = 1; i <= users; i++) {
        for (int j = 1; j <= content; j++) {
          bool found_content = ( find(relationMap[i].begin(), relationMap[i].end(), j) != relationMap[i].end() );

          if (relationMap.count(i) && found_content) { //the user x owns this content y
            adjacencyMatrix[i][j] = 1;
          }
        }
      }
    }

    void similarity(int users, int content, Relation* friendship, Relation* ownership, Relation* likedContent) {
      for (int i = 1; i <= users; i++) {
        for (int j = 1; j <= users; j++) {
          if (friendship->isRelated(i, j) && ownership->ownsContent(j)) {
            adjacencyMatrix[i][j] = 1;
          }
          else {
            for (int k = 1; k <= content; k++) {
              if (ownership->isRelated(i, k) && likedContent->isRelated(j, k)) {
                adjacencyMatrix[i][j] = 1;
                break;
              }
            }
          }
        }
      }
    }

    bool isRelated(int row, int column) {
      if (adjacencyMatrix[row][column]) return true;
      else return false;
    }

};

int main() {


  int users;
  int content;
  map<int, vector<int> > friendshipMap;
  map<int, vector<int> > ownershipMap;
  map<int, vector<int> > likeMap;

  string input = "";
  while (getline(cin, input)) {
    if (input == "#U") {
      while (getline(cin, input)) {
        if (input == "#C") break;
        users = int(input[0]-48);
      }
    }
    if (input == "#C") {
      while (getline(cin, input)) {
        if (input == "#F") break;
        content = int(input[0]-48);
      }
    }
    if (input == "#F") {
      while (getline(cin, input)) {
        if (input == "#W") break;
        int key = int(input[0]-48);
        int value = int(input[2]-48);
        friendshipMap[key].push_back(value);
      }
    }
    if (input == "#W") {
      while (getline(cin, input)) {
        if (input == "#L") break;
        int key = int(input[0]-48);
        int value = int(input[2]-48);
        ownershipMap[key].push_back(value);
      }
    }
    if (input == "#L") {
      while (getline(cin, input)) {
        int key = int(input[0]-48);
        int value = int(input[2]-48);
        likeMap[key].push_back(value);
      }
    }
  }

  Relation friendship = Relation(users, users);
  friendship.parseFriendship(users, users, friendshipMap);
  Relation* ptr_friend = &friendship;

  Relation ownership = Relation(users, content);
  ownership.parseContent(users, content, ownershipMap);
  Relation* ptr_content = &ownership;

  Relation likeRelation = Relation(users, content);
  likeRelation.parseContent(users, content, likeMap);
  Relation* ptr_liked = &likeRelation;

  Relation similarityRelation = Relation(users, users);
  similarityRelation.similarity(users, content, ptr_friend, ptr_content, ptr_liked);

  const int possibleSubsets = pow(2, users);

  for (int x = 1; x < possibleSubsets; x++) { //loops through X's subsets
    bitset<10> bitsX(x);

    for (int y = 1; y < possibleSubsets; y++) { //loops through Y's subsets
      bitset<10> bitsY(y);
      vector<int> vectorX = convertBitsToSetIndices(bitsX, users);
      vector<int> vectorY = convertBitsToSetIndices(bitsY, users);
      bool isSimilar = false;

      for (int i = 0; i < vectorX.size(); i++) { // loops through, makes sure bitset is opposite of current column
        for (int j = 0; j < vectorY.size(); j++) {
          int user1 = vectorX[i];
          int user2 = vectorY[j];
          if (similarityRelation.isRelated(user1, user2)) {
            isSimilar = true;
            break;
          }
        }
        if (isSimilar) break;
      }
      if (!isSimilar) {
        cout << "{";
        for (int i = 0; i < vectorY.size(); i++) {
          cout << vectorY[i];
          if (i+1 < vectorY.size()) cout << ",";
        }
        cout << "} {";
        for (int i = 0; i < vectorX.size(); i++) {
          cout << vectorX[i];
          if (i+1 < vectorX.size()) cout << ",";
        }
        cout << "}" << endl;
      }
    }
  }


  return 0;
}
