#include<iostream>
#include<sstream>

using namespace std;
/*template <typename T>
std::string itoa(T i){
   std::stringstream ss;
   ss << i;
   return(ss.str());
}

template <typename T>
  std::string NumberToString ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }

template <typename T>
T atoi(const string &S){
   T number;
   stringstream ss(S);
   ss >> number;
   return(number);
}


template <typename Z>
  Z StringToNumber ( const string &Text )
  {
     istringstream ss(Text);
     Z result;
     return ss >> result ? result : 0;
  }
*/


void reverse(string & s){
	unsigned int pointer = 0;
	while(pointer < ((s.length()-1)/2)){
		char c = s[pointer];
		s[pointer] = s[s.length() - pointer - 1];
		s[s.length() - pointer - 1] = c;
	}
}

 void itoa(int i, string &s){
	bool sign = false;
	if(i < 0){
		sign = true;
		i = -i;
	}
	while(i>=0){
		s+= (char)(i%10);
		i/=10;
	}
	if(sign){
		s+="-";
	}
	cout<<"aaa";
	//reverse(s);
}




int main(int argc, char* argv[]){
  int i = 11;
  string s;
  itoa(i, s);
  std::cout<<"abcs"<<std::endl;
  //std::cout<<NumberToString(i)<<std::endl;
  string S="029.12";
  //double number = atoi<double>("82.2812");
  //std::cout<<number<<std::endl;

  return(0);
}
