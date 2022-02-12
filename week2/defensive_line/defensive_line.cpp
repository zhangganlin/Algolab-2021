#include <iostream>
#include <vector>
#include<fstream>

using namespace std;

void runTest(){
  int n,m,k;
  cin >> n; cin >> m; cin >> k;
  vector<int> defenders(n,-1);

  for (int i = 0; i < n; i++){
      cin >> defenders[i];
  }
  int defenders_count =n;
  int attackers_count =m;
  int strength = k;

  int dp[attackers_count+1][defenders_count+1];
  std::vector<int> sequence_length(defenders_count);

  int sum = 0;
  int back_position = 0;

  for(int i = 0; i < defenders_count; i++) {
    sum += defenders[i];
    while(sum > strength) {
      sum -= defenders[back_position++];
    }
    
    if (sum == strength) {
      sequence_length[i] = i-back_position+1;
    }
  }

  for(int i = 0; i <= defenders_count; i++) {
    dp[0][i] = 0;
  }
  for(int i = 1; i <= attackers_count; i++) {
    dp[i][0] = -1;
  }

  for(int i = 1; i <= attackers_count; i++) {
    for(int j = 1; j <= defenders_count; j++) {
      const int len = sequence_length[j-1];
      if (len && dp[i-1][j-len] != -1) {
        dp[i][j] = std::max(dp[i-1][j-len] + len, dp[i][j-1]);
      } else {
        dp[i][j] = dp[i][j-1];  
      }
    }
  }

  const int result = dp[attackers_count][defenders_count];
  if (result == -1) {
    std::cout << "fail" << std::endl;
  } else {
    std::cout << result << std::endl;
  }
}


int main() {
    std::ifstream fin("defensive_line_test.txt");
    auto *cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(fin.rdbuf());
    
  int t; cin >> t;
  for (int i = 0; i < t; i++){
      runTest();
  }
  return 0;
}