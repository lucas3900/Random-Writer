/* ##########################################################################
# #
# randwrite.cc #
# Lucas Barusek #
# 4/13/19 #
# #
# This program creates randomly generated sentences based on the
probability that a character will follow a seed of characters. The seed
will be randomly generated from k consecutive letters in the input file.
The longer the seed, the more legible the sentences will be since
there will be fewer characters proceeding a longer seed.
# #
########################################################################## */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "list.h"

void error(std::string msg){
  /* Produces an error message and exits the program */
  std::cerr << msg << std::endl;
  exit(1);
}

size_t cstring_to_size_t(char * s){
  /* converts a cstring to a size_t */

	size_t result;
	std::stringstream stream(s);
	stream >> result;
	return result;
}

size_t get_random(size_t mod){
  /* returns a random number from 0 to mod */
	return rand() % mod;
}

bool check_is_seed(int loc, std::string seed, const list & full_list, int k){
  /* checks if the seed has been found at a specific location */

  // sets count equal to one since we already know the first letter
  // of the seed has been found
	size_t count = 1;

  // iterates through the next k consecutive letters and checks if the
  // next k letters are equal to the seed. If they aren't, return false.
	for (int i = loc + 1; i < loc + k; i++){
		if (full_list.get(i) != seed[count])
			return false;
		count++;
	}

  // returns true if the seed is found
	return true;
}

void create_seed(std::string & seed, const list & lst, int k){
  /* creates a seed of k consecutive letters starting at a random
  location within the input text */

  // makes it so we get a new random seed
	srand(time(NULL));

  // resets seed to an empty string
  seed = "";

  // compiles a string of k consecutive letters starting at a random
  // start location
	int start_loc = get_random(lst.size() - k - 1);
	for (int i = start_loc; i < (start_loc + k); i++)
		seed += lst.get(i);
}

int main(int argc, char ** argv){
  /* The main program. Compiles the output text */

  // sends error message and exits program if not enough arguments are given
  if (argc != 5)
		error(std::string("Please enter 4 arguments: "
											"A seed length, an output length, an input file,"
											  " and an output file."));

  // sends error message and exits code if k or length is negative
  if (argv[1][0] == '-' or argv[2][0] == '-')
    error(std::string("The seed length and output file length"
                      " cannot be negative."));

  list full_list; // list to store input file
  list proceeding; // list to store characters proceeding the seed
  std::string final_; // creates a string to keep track of the ouput

  // cite: http://www.cplusplus.com/forum/beginner/31141/
  // descr: how to check if a string only contains numbers
  for(size_t i = 0; i < std::strlen(argv[1]); i++)
      if (!(argv[1][i] >= '0' and argv[1][i] <= '9'))
        error(std::string("Error: Seed lenth must be a number"));

  for(size_t i = 0; i < std::strlen(argv[2]); i++)
      if (!(argv[2][i] >= '0' and argv[2][i] <= '9'))
        error(std::string("Error: Output length must be a number"));

  // converts the input strings to size_ts
	size_t k = cstring_to_size_t(argv[1]);
  size_t length = cstring_to_size_t(argv[2]);

  // creates pointers to the input and output files
  char * source = argv[3];
	char * result = argv[4];

  // creates streams to connect us to the input and output text files
  std::ifstream textstream(source);
	std::ofstream result_text(result);

  // sends error and exits code if input file cannot be oppened
	if (!textstream)
		error(std::string("Error: Trouble opening file: ") + argv[3]);

  // sends error and exits code if output file cannot be oppened
	if (!result_text)
		error(std::string("Error: Trouble opening file: ") + argv[4]);

  // loop and a half to append all characters in the input file to a list
	int v;
  while(textstream){
    v = textstream.get();
    if (textstream) full_list.add(v, full_list.size());
	}

  // sends error and exits code if the length of the seed if bigger
  // than the input file
	if (full_list.size() < k)
		error(std::string("Error: Seed length is bigger than the input file's "
		                  "charaters"));

  // checks for the no seed edge case
  if (k!= 0){

    // creates the adds the seed to the output file
	   std::string seed;
	   create_seed(seed, full_list, k);
	   for (size_t i = 0; (i < k) and (i < length); i++){
       final_ += seed[i];
		   result_text.put(seed[i]);
     }

     // runs for as long as the lenght of the output text is not equal to length
	   while (final_.size() != length){

       // iterates through the entire list starting at the first instance
       // of the first letter of the seed
		   for (size_t i = full_list.find(seed[0]); i < full_list.size() - k - 1; i++){

         // looks for the first letter in the seed in the list
		     if (full_list.get(i) == seed[0]){

           /* if (i != full_list.size() - k - 2) this statement was used
           when using a input file from atom b/c of the two addiontal
           characters atom files create. However that doesn't work for
           an input smaller than 3.*/

           // checks if the seed is the last character(s) in the input
				   if (i != full_list.size() - k){

             // checks if we found the seed in the list. If so appends
             // the letter after the seed to the proceeding list
					   if (check_is_seed(i, seed, full_list, k))
						   proceeding.add(full_list.get(i + k), 0);
				   }
			   }
		   }

       // checks if there are no proceeding characters (this will only happen
       // if the only instance of the seed is the very end of the input file)
       if(proceeding.size() != 0){

         // gets a random letter from the proceeding characters after the
         // seed and adds it to the output file
		     char character = proceeding.get(get_random(proceeding.size()));
		     final_ += character;
		     result_text.put(character);

         // adds the random character to the end of the seed and deletes
         // the first character
		     seed += character;
		     seed.erase(seed.begin());

         // deletes all the nodes in the proceeding list to avoid
         // a memory leak
         while (proceeding.size() != 0) proceeding.remove_at(0);
       }
       else{

         // if the current seed has only one instance at the very end
         // of the input file, create a new seed and run again
         create_seed(seed, full_list, k);
         for (size_t i = 0; (i < k) and (i < length); i++)
    		   result_text.put(seed[i]);
      }
	  }
  }
  else{

    // if there is no seed, then randomly pick characters from the
    // entire input and add them to the output
    srand(time(NULL));
    while (final_.size() != length){
      char letter = full_list.get(get_random(full_list.size()));
      final_ += letter;
      result_text.put(letter);
    }
  }
  // closes the streams
  result_text.close();
  textstream.close();
  
}
