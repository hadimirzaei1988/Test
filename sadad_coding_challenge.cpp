/**
 * @file sadad_coding_challenge.cpp
 * @version 1.1
 * @copyright Copyright (c) 2023 SADAD
 *
 * The goal is to implement the mechanics of a simple game and write test cases for them.
 * The rules of the game are:
 * - There are two types of boxes, green and blue.
 * - Both can absorb tokens of a given weight, which they add to their own total weight.
 * - Both are initialized with a given initial weight.
 * - After a box absorbs a token weight, it outputs a score.
 * - Green and blue boxes calculate the score in different ways:
 * - A green box calculates the score as the square of the mean of the 3 weights that it most recently absorbed (square of mean of all absorbed weights if there are fewer than 3).
 * - A blue box calculates the score as Cantor's pairing function of the smallest and largest weight that it has absorbed so far, i.e. pairing(smallest, largest), where pairing(0, 1) = 2
 * - The game is played with two green boxes with initial weights 0.0 and 0.1, and two blue boxes with initial weights 0.2 and 0.3.
 * - There is a list of input token weights. Each gets used in one turn.
 
 * - There are two players, A and B. Both start with a score of 0.
 * - The players take turns alternatingly. Player A starts.
 * - In each turn, the current player selects one of the boxes with the currently smallest weight, and lets it absorb the next input token weight. Each input weight gets only used once.
 * - The result of the absorption gets added to the current player's score.
 * - When all input token weights have been used up, the game ends, and the player with highest score wins.
 *
 * Task:
 * - Add git to source files and don't commit solution with only one commit!
 * - Implement all missing parts, marked with "TODO", including the test cases.
 * - Make sure the test cases succeed.
 * - Produce clean, readable code.
 * - zip everything(including git files) and send it back to us or upload it into your github and send us the link.
 *
 * Notes:
 * - Feel free to add any build systems like CMake (it's bonus point)
 * - Building and running the executable: g++ --std=c++20 sadad_coding_challenge.cpp -o challenge && ./challenge
 * - This file includes the header-only test framework Catch v2.13.9.
 * - A main function is not required, as it is provided by the test framework.
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <numeric>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

class Box
{
private:
  
     //Both are initialized with a given initial weight.so Delete the copy constructor and assignment operator
      Box(){};
      Box(const Box&) = delete;
      Box& operator=(const Box&) = delete;
public:
  explicit Box(double initial_weight) : weight_(initial_weight) {
  lastWeightList.emplace_back(initial_weight);
  }
  static std::unique_ptr<Box> makeGreenBox(double initial_weight){
  	std::unique_ptr<Box> greenBox(new Box(initial_weight));
 	greenBox->isGreen=true;
  return greenBox;
  }
  static std::unique_ptr<Box> makeBlueBox(double initial_weight){
  	  std::unique_ptr<Box> blueBox(new Box(initial_weight));
  	  blueBox->isGreen=false;
   return blueBox;
  }
  bool operator<(const Box &rhs) const { return weight_ < rhs.weight_; }
  bool isGreen;//we have two types of Box green or blue
  std::vector<double> lastWeightList;//keep all absorbed weight

  // TODO
  double absorb(uint32_t newWeight){
  
  weight_+=newWeight;
  lastWeightList.emplace_back(weight_);
  double score=0;
  if(isGreen)
  {
     int size = lastWeightList.size();
     if(size>=3)//the square of the mean of the 3 weights that it most recently absorbed
     {
       score=(lastWeightList[size-1]+lastWeightList[size-2]+lastWeightList[size-3]);              
     }else{//square of mean of all absorbed weights if there are fewer than 3
         for(size_t i=0;i<size;i++){
           score+=lastWeightList[i];
         }
         
     }
    
     score=(double)(score/size);
    
     score=score*score;
     std::cout<<"\ngreen\t"<<score<<"\n";
  }else
  {//Cantor's pairing function of the smallest and largest weight 
  double max = *max_element(lastWeightList.begin(), lastWeightList.end());
  double min = *min_element(lastWeightList.begin(), lastWeightList.end());
  score=(min + max) * (min + max + 1) / 2.0 + max;
    std::cout<<"\nblue\t"<<score<<"\n";
  
  }

    return score ;
  }
double getWeight(){
return weight_;
}
protected:
  double weight_;
};

// TODO

class Player
{
public:
  void takeTurn(uint32_t input_weight,
                const std::vector<std::unique_ptr<Box>> &boxes)
  {
       //selects one of the boxes with the currently smallest weight
       int smallestIndex=smallestWeight(boxes);
       if(smallestIndex>=0){
       score_+=boxes[smallestIndex]->absorb(input_weight);
         std::cout<<"\n smallest Index "<<smallestIndex<<"\t boxWei"<<boxes[smallestIndex]->getWeight()<<" input "<<input_weight<<"\n";
       }else
       {
         std::cout<<"\n bug\n";
       }
    // TODO
  }
  int smallestWeight(const std::vector<std::unique_ptr<Box>> &boxes){//return the index of the box with smallest weight
  double maxValue= std::numeric_limits<double>::max();
  int smallestIndex=-1;
   for(size_t i=0;i<boxes.size();i++){
      if(boxes[i]->getWeight()<maxValue){
        smallestIndex=i;
        maxValue=boxes[i]->getWeight();
      }
   }
   return smallestIndex;
  }
  double getScore() const { return score_; }

private:
  double score_{0.0};
};

std::pair<double, double> play(const std::vector<uint32_t> &input_weights)
{
  std::vector<std::unique_ptr<Box>> boxes;
  boxes.emplace_back(Box::makeGreenBox(0.0));
  boxes.emplace_back(Box::makeGreenBox(0.1));
  boxes.emplace_back(Box::makeBlueBox(0.2));
  boxes.emplace_back(Box::makeBlueBox(0.3));
 

  // TODO
  Player player_A,player_B;
  for(size_t i=0;i<input_weights.size();i++){
  if(i%2==0)
    player_A.takeTurn(input_weights[i],boxes);
    else 
    player_B.takeTurn(input_weights[i],boxes);
  }
  std::cout << "Scores: player A " << player_A.getScore() << ", player B "
            << player_B.getScore() << std::endl;
  return std::make_pair(player_A.getScore(), player_B.getScore());
}
uint32_t absorption(const std::vector<uint32_t> &input_weights,bool _isGreen)
{

   Box tmpBox(0.0);
   tmpBox.isGreen=_isGreen;
    for(auto i:input_weights)
  {
    tmpBox.absorb(i);
  }
 
  return tmpBox.getWeight();
}
TEST_CASE("Final scores for first 4 Fibonacci numbers", "[fibonacci4]")
{
  std::vector<uint32_t> inputs{1, 1, 2, 3};
  auto result = play(inputs);
  REQUIRE(result.first == 13.0);
  REQUIRE(result.second == 25.0);
}

TEST_CASE("Final scores for first 8 Fibonacci numbers", "[fibonacci8]")
{
  std::vector<uint32_t> inputs{1, 1, 2, 3, 5, 8, 13, 21};
  auto result = play(inputs);
  REQUIRE(result.first == 155.0);
  REQUIRE(result.second == 366.25);
}

TEST_CASE("Test absorption of green box", "[green]")
{
  std::vector<uint32_t> inputs{1, 1, 2, 3, 5, 8, 13, 21};
    bool isGreen=true;
    auto result = absorption(inputs,isGreen);

   REQUIRE(result== 54.0);
  // TODO
}

TEST_CASE("Test absorption of blue box", "[blue]")
{
    std::vector<uint32_t> inputs{1, 1, 2, 3, 5, 8, 13, 21};
    bool isGreen=false;
    auto result = absorption(inputs,isGreen);


   REQUIRE(result== 54.0);
}
